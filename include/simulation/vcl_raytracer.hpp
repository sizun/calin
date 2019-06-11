/*

   calin/simulation/vcl_raytracer.hpp -- Stephen Fegan -- 2018-09-10

   Class for raytracing on a single VSOTelescope using VCL

   Copyright 2018, Stephen Fegan <sfegan@llr.in2p3.fr>
   LLR, CNRS, Ecole Polytechnique, Institut Polytechnique de Paris

   This file is part of "calin"

   "calin" is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License version 2 or
   later, as published by the Free Software Foundation.

   "calin" is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

*/

#pragma once

#include <algorithm>

#include <util/memory.hpp>
#include <util/vcl.hpp>
#include <math/special.hpp>
#include <math/ray_vcl.hpp>
#include <math/rng_vcl.hpp>
#include <math/hex_array_vcl.hpp>
#include <math/geometry_vcl.hpp>
#include <simulation/vso_telescope.hpp>
#include <simulation/vso_obscuration.hpp>

namespace calin { namespace simulation { namespace vcl_raytracer {

enum ScopeTraceStatus {
  STS_MASKED_ON_ENTRY,
  STS_TRAVELLING_AWAY_REFLECTOR,
  STS_MISSED_REFLECTOR_SPHERE,
  STS_OUTSIDE_REFLECTOR_APERTURE,
  STS_NO_MIRROR,
  STS_MISSED_MIRROR_SPHERE,
  STS_MISSED_MIRROR_EDGE,
  STS_OBSCURED_BEFORE_MIRROR,
      TS_MISSED_WINDOW,
      TS_OBSCURED_BEFORE_FOCAL_PLANE,
  STS_TRAVELLING_AWAY_FROM_FOCAL_PLANE,
  STS_OUTSIDE_FOCAL_PLANE_APERTURE,
  STS_TS_NO_PIXEL,
  STS_TS_FOUND_PIXEL
};

template<typename VCLRealType> class ScopeTraceInfo: public VCLRealType
{
public:
  using typename VCLRealType::real_t;
  using typename VCLRealType::real_vt;
  using typename VCLRealType::bool_vt;
  using typename VCLRealType::int_vt;
  using typename VCLRealType::uint_vt;
  using typename VCLRealType::vec3_vt;
  using typename VCLRealType::mat3_vt;

  int_vt              status;

  real_vt             reflec_x;
  real_vt             reflec_z;

  int_vt              mirror_hexid;
  int_vt              mirror_id;
  vec3_vt             mirror_pos;
  vec3_vt             mirror_dir;
  real_vt             mirror_r;
  vec3_vt             mirror_normal;
  real_vt             mirror_normal_dispersion;
  vec3_vt             mirror_scattered;

  real_vt             fplane_x;
  real_vt             fplane_z;
  real_vt             fplane_t;
  real_vt             fplane_uy;

  int_vt              pixel_hexid;
  int_vt              pixel_id;
};

template<typename VCLRealType> class Obscuration: public VCLRealType
{
public:
  using typename VCLRealType::real_vt;
  using typename VCLRealType::bool_vt;
  using typename VCLRealType::vec3_vt;
  using typename VCLRealType::mat3_vt;
  using Ray = calin::math::ray::VCLRay<VCLRealType>;

  virtual ~Obscuration() {
    // nothing to see here
  }
  virtual bool_vt doesObscure(const Ray& p_in, Ray& p_out, real_vt n) = 0;
  virtual Obscuration<VCLRealType>* clone() const = 0;
};

template<typename VCLRealType> class ScopeRayTracer: public VCLRealType
{
public:
  using typename VCLRealType::real_t;
  using typename VCLRealType::int_t;
  using typename VCLRealType::uint_t;
  using typename VCLRealType::bool_int_vt;
  using typename VCLRealType::mat3_t;
  using typename VCLRealType::vec3_t;
  using typename VCLRealType::real_vt;
  using typename VCLRealType::bool_vt;
  using typename VCLRealType::vec3_vt;
  using typename VCLRealType::mat3_vt;
  using Ray = calin::math::ray::VCLRay<VCLRealType>;
  using TraceInfo = ScopeTraceInfo<VCLRealType>;
  using RNG = calin::math::rng::VCLRealRNG<VCLRealType>;

  ScopeRayTracer(const calin::simulation::vs_optics::VSOTelescope* scope,
      real_t refractive_index = 1.0,
      RNG* rng = nullptr, bool adopt_rng = false):
    VCLRealType(), rng_(rng==nullptr ? new RNG(__PRETTY_FUNCTION__) : rng),
    adopt_rng_(rng==nullptr ? true : adopt_rng)
  {
    using calin::math::special::SQR;

    global_to_reflector_rot_ = scope->rotationGlobalToReflector().cast<real_t>();
    ref_index_               = refractive_index;

    reflec_curvature_radius_ = scope->curvatureRadius();
    reflec_aperture2_        = 0;
    reflec_crot_             = scope->cosReflectorRotation();
    reflec_srot_             = scope->sinReflectorRotation();
    reflec_scaleinv_         = 1.0/scope->facetSpacing();
    reflec_shift_x_          = scope->facetGridShiftX();
    reflec_shift_z_          = scope->facetGridShiftZ();
    reflec_cw_               = scope->mirrorParity();

    mirror_hexid_end_        = scope->numMirrorHexSites();
    mirror_id_end_           = scope->numMirrors();

    calin::util::memory::aligned_calloc(mirror_id_lookup_, mirror_hexid_end_+1);
    calin::util::memory::aligned_calloc(mirror_nx_lookup_, mirror_id_end_+1);
    calin::util::memory::aligned_calloc(mirror_nz_lookup_, mirror_id_end_+1);
    calin::util::memory::aligned_calloc(mirror_ny_lookup_, mirror_id_end_+1);
    calin::util::memory::aligned_calloc(mirror_r_lookup_, mirror_id_end_+1);
    calin::util::memory::aligned_calloc(mirror_x_lookup_, mirror_id_end_+1);
    calin::util::memory::aligned_calloc(mirror_z_lookup_, mirror_id_end_+1);
    calin::util::memory::aligned_calloc(mirror_y_lookup_, mirror_id_end_+1);
    calin::util::memory::aligned_calloc(mirror_normdisp_lookup_, mirror_id_end_+1);

    for(int ihexid=0; ihexid<mirror_hexid_end_; ihexid++) {
      auto* mirror = scope->mirrorByHexID(ihexid);
      mirror_id_lookup_[ihexid] =
        (mirror==nullptr or mirror->removed()) ? mirror_id_end_ : mirror->id();
    }
    mirror_id_lookup_[mirror_hexid_end_] = mirror_id_end_;

    for(int iid=0; iid<mirror_id_end_; iid++) {
      auto* mirror = scope->mirror(iid);
      mirror_nx_lookup_[iid] = mirror->align().x();
      mirror_nz_lookup_[iid] = mirror->align().z();
      mirror_ny_lookup_[iid] = mirror->align().y();
      mirror_r_lookup_[iid] = 2.0*mirror->focalLength();
      mirror_x_lookup_[iid] = mirror->pos().x();
      mirror_z_lookup_[iid] = mirror->pos().z();
      mirror_y_lookup_[iid] = mirror->pos().y();
      mirror_normdisp_lookup_[iid] = 0.25*mirror->spotSize()/mirror->focalLength();
      reflec_aperture2_ = std::max(reflec_aperture2_, real_t(SQR(mirror->pos().x())+SQR(mirror->pos().z())));
    }
    mirror_nx_lookup_[mirror_id_end_] = 0.0;
    mirror_nz_lookup_[mirror_id_end_] = 0.0;
    mirror_ny_lookup_[mirror_id_end_] = 1.0;
    mirror_r_lookup_[mirror_id_end_] = 100000.0; // 1km - should be enough!
    mirror_x_lookup_[mirror_id_end_] = 0.0;
    mirror_z_lookup_[mirror_id_end_] = 0.0;
    mirror_y_lookup_[mirror_id_end_] = 0.0;
    mirror_normdisp_lookup_[mirror_id_end_] = 0.0;
    mirror_dhex_max_ = 0.5*scope->facetSize();
    reflec_aperture2_ = SQR(std::sqrt(reflec_aperture2_) + scope->facetSpacing());

    fp_pos_                  = scope->focalPlanePosition().cast<real_t>();
    fp_has_rot_              = scope->hasFPRotation();
    fp_rot_                  = scope->rotationReflectorToFP().cast<real_t>();
    fp_aperture2_            = 0;

    pixel_crot_              = scope->cosPixelRotation();
    pixel_srot_              = scope->sinPixelRotation();
    pixel_scaleinv_          = 1.0/scope->pixelSpacing();
    pixel_shift_x_           = scope->pixelGridShiftX();
    pixel_shift_z_           = scope->pixelGridShiftZ();
    pixel_cw_                = scope->pixelParity();

    pixel_hexid_end_         = scope->numPixelHexSites();
    pixel_id_end_            = scope->numPixels();

    calin::util::memory::aligned_calloc(pixel_id_lookup_, pixel_hexid_end_+1);
    for(int ihexid = 0; ihexid<pixel_hexid_end_; ihexid++) {
      const auto* pixel = scope->pixelByHexID(ihexid);
      if(pixel==nullptr or pixel->removed()) {
        pixel_id_lookup_[ihexid] = pixel_hexid_end_;
      } else {
        pixel_id_lookup_[ihexid] = pixel->id();
        fp_aperture2_ =
          std::max(fp_aperture2_, real_t(SQR(pixel->pos().x())+SQR(pixel->pos().z())));
      }
    }
    pixel_id_lookup_[pixel_hexid_end_] = pixel_hexid_end_;
    fp_aperture2_ = SQR(std::sqrt(fp_aperture2_) + scope->pixelSpacing());

#if 0
    std::cout << pixel_crot_ << ' ' << pixel_srot_ << ' ' << pixel_scaleinv_ << ' '
      << pixel_shift_x_ << ' ' << pixel_shift_z_ << ' ' << pixel_cw_ << ' '
      << pixel_hexid_end_ << ' ' << pixel_id_end_ << ' '
      << fp_aperture2_ << ' ' << std::sqrt(fp_aperture2_) << '\n';
#endif
  }

  ~ScopeRayTracer()
  {
    free(mirror_id_lookup_);
    free(mirror_nx_lookup_);
    free(mirror_nz_lookup_);
    free(mirror_ny_lookup_);
    free(mirror_r_lookup_);
    free(mirror_x_lookup_);
    free(mirror_z_lookup_);
    free(mirror_y_lookup_);
    free(mirror_normdisp_lookup_);
    free(pixel_id_lookup_);
    if(adopt_rng_)delete rng_;
  }

  bool_vt trace_scope_centered_global_frame(bool_vt mask, Ray& ray, TraceInfo& info)
  {
    // *************************************************************************
    // *************** RAY STARTS IN SCOPE CENTERED GLOBAL FRAME ***************
    // *************************************************************************

    ray.rotate(global_to_reflector_rot_.template cast<real_vt>());
    mask = trace_reflector_frame(mask, ray, info);
    ray.derotate(global_to_reflector_rot_.template cast<real_vt>());
    return mask;
  }

//#define DEBUG_STATUS

  bool_vt trace_reflector_frame(bool_vt mask, Ray& ray, TraceInfo& info)
  {
    info.status = STS_MASKED_ON_ENTRY;
#ifdef DEBUG_STATUS
    std::cout << mask[0] << '/' << info.status[0];
#endif

    info.status = select(bool_int_vt(mask), STS_TRAVELLING_AWAY_REFLECTOR, info.status);
    mask &= ray.uy() < 0;
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    // *************************************************************************
    // ****************** RAY STARTS IN RELECTOR COORDINATES *******************
    // *************************************************************************

    // Test for obscuration - to come

    // Propagate to intersection with the reflector sphere
    info.status = select(bool_int_vt(mask), STS_MISSED_REFLECTOR_SPHERE, info.status);
    mask = ray.propagate_to_y_sphere_2nd_interaction_fwd_only_with_mask(mask,
      reflec_curvature_radius_, 0, ref_index_);
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    info.reflec_x     = select(mask, ray.position().x(), 0);
    info.reflec_z     = select(mask, ray.position().z(), 0);

    // Test aperture
    info.status = select(bool_int_vt(mask), STS_OUTSIDE_REFLECTOR_APERTURE, info.status);
    mask &= (info.reflec_x*info.reflec_x + info.reflec_z*info.reflec_z) <= reflec_aperture2_;
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    // Assume mirrors on hexagonal grid - use hex_array routines to find which hit
    info.status = select(bool_int_vt(mask), STS_NO_MIRROR, info.status);
    info.mirror_hexid = calin::math::hex_array::VCLReal<VCLRealType>::
      xy_trans_to_hexid_scaleinv(info.reflec_x, info.reflec_z,
        reflec_crot_, reflec_srot_, reflec_scaleinv_, reflec_shift_x_, reflec_shift_z_,
        reflec_cw_);

    // Test we have a valid mirror hexid
    mask &= typename VCLRealType::bool_vt(info.mirror_hexid < mirror_hexid_end_);
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    info.mirror_hexid = select(bool_int_vt(mask), info.mirror_hexid, mirror_hexid_end_);

    // Find the mirror ID
    info.mirror_id = vcl::lookup<0x40000000>(info.mirror_hexid, mirror_id_lookup_);

    // Test we have a valid mirror id
    mask &= typename VCLRealType::bool_vt(info.mirror_id < mirror_id_end_);
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    info.mirror_dir.x() = vcl::lookup<0x40000000>(info.mirror_id, mirror_nx_lookup_);
    info.mirror_dir.z() = vcl::lookup<0x40000000>(info.mirror_id, mirror_nz_lookup_);
#if 1
    // Is it faster to use lookup table than to compute ?
    info.mirror_dir.y() = vcl::lookup<0x40000000>(info.mirror_id, mirror_ny_lookup_);
#else
    info.mirror_dir.y() = sqrt(nmul_add(info.mirror_dir.z(),info.mirror_dir.z(),
      nmul_add(info.mirror_dir.x(),info.mirror_dir.x(),1.0)));
#endif

    info.mirror_r = vcl::lookup<0x40000000>(info.mirror_id, mirror_r_lookup_);

    info.mirror_pos.x() = vcl::lookup<0x40000000>(info.mirror_id, mirror_x_lookup_);
    info.mirror_pos.z() = vcl::lookup<0x40000000>(info.mirror_id, mirror_z_lookup_);
    info.mirror_pos.y() = vcl::lookup<0x40000000>(info.mirror_id, mirror_y_lookup_);

    vec3_vt mirror_center = info.mirror_pos + info.mirror_dir * info.mirror_r;

    ray.translate_origin(mirror_center);

    // *************************************************************************
    // ******************* RAY IS NOW IN MIRROR COORDINATES ********************
    // *************************************************************************

    // Propagate to intersection with the mirror sphere
    info.status = select(bool_int_vt(mask), STS_MISSED_MIRROR_SPHERE, info.status);
    mask = ray.propagate_to_y_sphere_2nd_interaction_fwd_bwd_with_mask(mask,
      info.mirror_r, -info.mirror_r, ref_index_);
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    // Impact point relative to facet attchment point
    vec3_vt ray_pos = ray.position() + mirror_center - info.mirror_pos;
    calin::math::geometry::VCL<VCLRealType>::
      derotate_in_place_Ry(ray_pos, reflec_crot_, reflec_srot_);

    vec3_vt mirror_dir = info.mirror_dir;
    calin::math::geometry::VCL<VCLRealType>::
      derotate_in_place_Ry(mirror_dir, reflec_crot_, reflec_srot_);

    calin::math::geometry::VCL<VCLRealType>::
      derotate_in_place_y_to_u_Ryxy(ray_pos, mirror_dir);

    // Verify that ray impacts inside of hexagonal mirror surface
    const real_vt cos60 = 0.5;
    const real_vt sin60 = 0.5*CALIN_HEX_ARRAY_SQRT3;

    const real_vt x_cos60 = ray_pos.x() * cos60;
    const real_vt z_sin60 = ray_pos.z() * sin60;

    const real_vt dhex_pos60 = abs(x_cos60 - z_sin60);
    const real_vt dhex_neg60 = abs(x_cos60 + z_sin60);

    info.status = select(bool_int_vt(mask), STS_MISSED_MIRROR_EDGE, info.status);
    mask &= max(max(dhex_pos60, dhex_neg60), abs(ray_pos.x())) < mirror_dhex_max_;
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    // Calculate mirror normal at impact point
    info.mirror_normal = -ray.position() * (1.0/info.mirror_r);

    // Scatter the normal to account for the spot size ot the focal length of the
    // radius. The spot size is given as the DIAMETER at the focal distance.
    // Must divide by 2 (for reflection) and another 2 for diameter -> radius
    info.mirror_normal_dispersion =
      vcl::lookup<0x40000000>(info.mirror_id, mirror_normdisp_lookup_);

    info.mirror_scattered = info.mirror_normal;
    calin::math::geometry::VCL<VCLRealType>::scatter_direction_in_place(
      info.mirror_scattered, info.mirror_normal_dispersion, *rng_);

    // Reflect ray
    ray.reflect_from_surface_with_mask(mask, info.mirror_scattered);

    // Translate back to reflector frame
    ray.untranslate_origin(mirror_center);

    // *************************************************************************
    // *************** RAY IS NOW BACK IN REFLECTOR COORDINATES ****************
    // *************************************************************************

    // Check obsucrations

    // Refract in window

    ray.translate_origin(fp_pos_.template cast<real_vt>());
    if(fp_has_rot_)ray.rotate(fp_rot_.template cast<real_vt>());

    // *************************************************************************
    // ***************** RAY IS NOW IN FOCAL PLANE COORDINATES *****************
    // *************************************************************************

    // Propagate to focal plane
    info.status = select(bool_int_vt(mask), STS_TRAVELLING_AWAY_FROM_FOCAL_PLANE, info.status);
    mask = ray.propagate_to_y_plane_with_mask(mask, 0, false, ref_index_);
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    // Test for interaction with obscuration before focal plane was hit

    // We good, record position on focal plane etc
    info.fplane_x = select(mask, ray.x(), 0);
    info.fplane_z = select(mask, ray.z(), 0);
    info.fplane_t = select(mask, ray.ct(), 0) * math::constants::cgs_1_c;
    info.fplane_uy = select(mask, ray.uy(), 0);

    info.status = select(bool_int_vt(mask), STS_OUTSIDE_FOCAL_PLANE_APERTURE, info.status);
    mask &= (info.fplane_x*info.fplane_x + info.fplane_z*info.fplane_z) <= fp_aperture2_;
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    info.pixel_hexid =
    calin::math::hex_array::VCLReal<VCLRealType>::
      xy_trans_to_hexid_scaleinv(info.fplane_x, info.fplane_z,
        pixel_crot_, pixel_srot_, pixel_scaleinv_, pixel_shift_x_, pixel_shift_z_,
        pixel_cw_);

    // Test we have a valid mirror hexid
    info.status = select(bool_int_vt(mask), STS_TS_NO_PIXEL, info.status);
    mask &= typename VCLRealType::bool_vt(info.pixel_hexid < pixel_hexid_end_);
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    // Find the mirror ID
    info.pixel_id =
      vcl::lookup<0x40000000>(select(bool_int_vt(mask), info.pixel_hexid, pixel_hexid_end_),
        pixel_id_lookup_);

    mask &= typename VCLRealType::bool_vt(info.pixel_id < pixel_id_end_);
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    info.status = select(bool_int_vt(mask), STS_TS_FOUND_PIXEL, info.status);
#ifdef DEBUG_STATUS
    std::cout << ' ' << mask[0] << '/' << info.status[0];
#endif

    if(fp_has_rot_)ray.derotate(fp_rot_.template cast<real_vt>());
    ray.untranslate_origin(fp_pos_.template cast<real_vt>());

    // *************************************************************************
    // ************ RAY IS NOW BACK IN REFLECTOR COORDINATES AGAIN *************
    // *************************************************************************

#ifdef DEBUG_STATUS
    std::cout << '\n';
#endif
    return mask;
  }

 private:
   mat3_t          global_to_reflector_rot_;
   real_t          ref_index_;

   real_t          reflec_curvature_radius_;
   real_t          reflec_aperture2_;
   real_t          reflec_crot_;
   real_t          reflec_srot_;
   real_t          reflec_scaleinv_;
   real_t          reflec_shift_x_;
   real_t          reflec_shift_z_;
   bool            reflec_cw_;

   int_t           mirror_hexid_end_;
   int_t           mirror_id_end_;
   int_t*          mirror_id_lookup_ = nullptr;
   real_t*         mirror_nx_lookup_ = nullptr;
   real_t*         mirror_nz_lookup_ = nullptr;
   real_t*         mirror_ny_lookup_ = nullptr;
   real_t*         mirror_r_lookup_ = nullptr;
   real_t*         mirror_x_lookup_ = nullptr;
   real_t*         mirror_z_lookup_ = nullptr;
   real_t*         mirror_y_lookup_ = nullptr;
   real_t          mirror_dhex_max_;
   real_t*         mirror_normdisp_lookup_ = nullptr;

   vec3_t          fp_pos_;
   bool            fp_has_rot_;
   mat3_t          fp_rot_;
   real_t          fp_aperture2_;

   real_t          pixel_crot_;
   real_t          pixel_srot_;
   real_t          pixel_scaleinv_;
   real_t          pixel_shift_x_;
   real_t          pixel_shift_z_;
   bool            pixel_cw_;

   int_t           pixel_hexid_end_;
   int_t           pixel_id_end_;
   int_t*          pixel_id_lookup_ = nullptr;

   RNG* rng_ = nullptr;
   bool adopt_rng_ = false;
};

template<typename VCLRealType> class AlignedBoxObscuration:
  public Obscuration<VCLRealType>
{
public:
  using typename Obscuration<VCLRealType>::real_vt;
  using typename Obscuration<VCLRealType>::bool_vt;
  using typename Obscuration<VCLRealType>::vec3_vt;
  using typename Obscuration<VCLRealType>::mat3_vt;
  using typename Obscuration<VCLRealType>::Ray;

  AlignedBoxObscuration(const vec3_vt& max_corner, const vec3_vt& min_corner):
    Obscuration<VCLRealType>(), min_corner_(min_corner), max_corner_(max_corner)
  {
    // nothing to see here
  }
  AlignedBoxObscuration(const calin::simulation::vs_optics::VSOAlignedBoxObscuration& o):
    Obscuration<VCLRealType>(),
    min_corner_(o.min_corner().template cast<real_vt>()),
    max_corner_(o.max_corner().template cast<real_vt>())
  {
    // nothing to see here
  }
  virtual ~AlignedBoxObscuration()
  {
    // nothing to see here
  }
  bool_vt doesObscure(const Ray& p_in, Ray& p_out, real_vt n) override
  {
    real_vt tmin;
    real_vt tmax;
    bool_vt mask = p_in.box_has_future_intersection(tmin, tmax, min_corner_, max_corner_);
    p_out = p_in;
    p_out.propagate_dist_with_mask(mask & (tmin>0), tmin, n);
    return mask;
  }
  virtual AlignedBoxObscuration<VCLRealType>* clone() const override
  {
    return new AlignedBoxObscuration<VCLRealType>(*this);
  }
private:
  vec3_vt min_corner_;
  vec3_vt max_corner_;
};



// std::ostream& operator <<(std::ostream& stream, const VSORayTracer::TraceInfo& o);

} } } // namespace calin::simulations::vs_optics
