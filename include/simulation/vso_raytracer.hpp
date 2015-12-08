/* 

   calin/simulation/vso_raytracer.hpp -- Stephen Fegan -- 2015-11-30

   Class for raytracing on a VSOTelescope

   Copyright 2015, Stephen Fegan <sfegan@llr.in2p3.fr>
   LLR, Ecole polytechnique, CNRS/IN2P3, Universite Paris-Saclay

   This file is part of "calin"
   
   "calin" is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License version 2 or
   later, as published by the Free Software Foundation.
    
   "calin" is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

*/

/*! \file VSORayTracer.hpp

  Ray tracing class header file

  \author  Stephen Fegan               \n
           UCLA                        \n
           sfegan@astro.ucla.edu       \n

  \author  Maciej Nicewicz             \n
           UCLA                        \n
           nicewicz@physics.ucla.edu   \n

  \date    12/05/2004
  \version 0.2
  \note
*/

#pragma once

#include <math/vs_vec3d.hpp>
#include <math/vs_vec4d.hpp>
#include <math/vs_particle.hpp>
#include <math/rng.hpp>
#include <simulation/vso_array.hpp>

namespace calin { namespace simulation { namespace vs_optics {
  
enum VSOTraceStatus { TS_NONE,                               // 0
                      TS_DOES_INTERSECT_GROUND,              // 1
                      TS_NO_SCOPE,                           // 2
                      TS_OUTSIDE_REFLECTOR_IP,               // 3
                      TS_MISSED_REFLECTOR_SPHERE,            // 4
                      TS_NO_MIRROR,                          // 5
                      TS_MIRROR_REMOVED,                     // 6
                      TS_MISSED_MIRROR_SPHERE,               // 7
                      TS_OBSCURED_BEFORE_MIRROR,             // 8
                      TS_MISSED_MIRROR_EDGE,                 // 9
                      TS_ABSORBED_AT_MIRROR,                 // 10
                      TS_TRAVELLING_AWAY_FROM_FOCAL_PLANE,   // 11
                      TS_OBSCURED_BEFORE_FOCAL_PLANE,        // 12
                      TS_NO_PIXEL,                           // 13
                      TS_ABSORBED_AT_CONCENTRATOR,           // 14
                      TS_PE_GENERATED };                     // 15

class VSOTraceInfo
{
 public:					
  const VSOArray*     array;
  VSOTraceStatus      status;
  double              ground_x;
  double              ground_y;
  double              ground_dx;
  double              ground_dy;
  int                 scope_id;
  const VSOTelescope* scope;
  double              reflec_x;
  double              reflec_z;
  double              hex_reflec_x;
  double              hex_reflec_z;
  double              hex_reflec_dx;
  double              hex_reflec_dz;
  int                 mirror_hexid_nominal;
  int                 mirror_hexid;
  const VSOMirror*    mirror;
  double              mirror_x;
  double              mirror_y;
  double              mirror_z;
  math::vs_physics::Vec3D   mirror_normal;
  double              mirror_normal_dispersion;
  math::vs_physics::Vec3D   mirror_scattered;
  double              mirror_reflection_angle;
  double              fplane_x;
  double              fplane_z;
  double              fplane_dx;
  double              fplane_dz;
  double              fplane_t;
  int                 pixel_hexid;
  const VSOPixel*     pixel;
  double              pixel_dist;
  bool                concentrator_hit;
  unsigned            obscuration_id;
  const VSOObscuration* obscuration;
  
  void reset();
  std::ostream& write(std::ostream& stream = std::cout,
                      bool convert_to_physical_units=false) const;
    
  bool rayWasReflected() const 
  { return (int)status >= (int)TS_TRAVELLING_AWAY_FROM_FOCAL_PLANE; }
  bool rayHitFocalPlane() const 
  { return (int)status >= (int)TS_NO_PIXEL; }
    
};

class VSOPSFInfo
{
 public:					
  unsigned            nhit;
  std::vector<double> r_tan;
  std::vector<double> r_sag;
  std::vector<double> t;
  double              mean_tan;
  double              mean_sag;
  double              mean_t;
  double              rms_tan;
  double              rms_sag;
  double              cov_tan_sag;
  double              rms_t;
  double              median_tan;
  double              median_sag;
  double              median_t;
  double              r80;
  double              t80;
  
  void reset() { *this = VSOPSFInfo(); }
};

class VSORayTracer
{
 public:
  VSORayTracer(const VSOArray& array, math::rng::RNG& rng): 
      fArray(array), fRNG(rng) { }
  virtual ~VSORayTracer();
  
  typedef VSOTraceStatus Status;
  typedef VSOTraceInfo TraceInfo;
    
  const VSOPixel* trace(math::vs_physics::Particle& ray, TraceInfo& info);
  const VSOPixel* trace(math::vs_physics::Particle& ray, TraceInfo& info,
                        const VSOTelescope* scope_hint);
    
  bool beam(math::vs_physics::Particle& photon,
            const math::vs_physics::Vec3D& origin,
            const math::vs_physics::Vec3D& direction, 
            double beam_start, double beam_stop, 
            double beam_radius_in, double beam_radius_out,
            double beam_angle_lo, double beam_angle_hi,
            double lambda_nm = 400);
    
  bool laserBeam(math::vs_physics::Particle& photon, 
                 const math::vs_physics::Vec3D& center,
                 const math::vs_physics::Vec3D& direction, 
                 double d0, double sampling_radius,
                 double lambda_nm = 400);
    
  bool fanBeam(math::vs_physics::Particle& photon,
               const math::vs_physics::Vec3D& origin, 
               const math::vs_physics::Vec3D& direction, 
               double half_angle_spread, double lambda_nm = 400);
    
  bool muonBeam(math::vs_physics::Particle& photon,
                const math::vs_physics::Vec3D& origin, 
                const math::vs_physics::Vec3D& direction, 
                double muon_travel_distance, double opening_angle, 
                double lambda_nm = 400);

  bool testBeam(math::vs_physics::Particle& photon,
                const VSOTelescope* scope,
                double theta, double phi = 0,
                double U = std::numeric_limits<double>::infinity(),
                double lambda_nm = 400);

  void calcPSF(class VSOPSFInfo& psf, const VSOTelescope* scope,
               double theta, double phi = 0,
               double U = std::numeric_limits<double>::infinity(),
               unsigned nsim = 1000000, bool save_image = false);
  
 private:
  bool findMirror(math::vs_physics::Particle& ray, TraceInfo& info);

  const VSOArray&          fArray;
  math::rng::RNG&          fRNG;

  const VSOPixel* scope_trace(math::vs_physics::Particle& ray,
                              TraceInfo& info);
};
  
#ifndef SWIG
std::ostream& operator <<(std::ostream& stream, 
                          const VSORayTracer::TraceInfo& o);
#endif

} } } // namespace calin::simulations::vs_optics