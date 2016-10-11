/*

   calin/simulation/quadrature_iact_array_integration.cpp
                                        -- Stephen Fegan -- 2016-07-28

   IACT array tracker that does quadrature integration over the cherenkov
   cone.

   Copyright 2016, Stephen Fegan <sfegan@llr.in2p3.fr>
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

#include <limits>

#include <simulation/iact_array_tracker.hpp>
#include <simulation/quadrature_iact_array_integration.hpp>
#include <math/special.hpp>
#include <math/vs_particle.hpp>
#include <io/log.hpp>

using namespace calin::simulation::quadrature_iact_array_integration;
using namespace calin::io::log;
using calin::math::special::SQR;
using calin::simulation::iact_array_tracker::IACTDetectorSphere;
using namespace calin::simulation::vs_optics;
//using namespace calin::math::vs_physics;

//#define ENABLE_RAYTRACER_STATUS

QuadratureIACTArrayPEProcessor::~QuadratureIACTArrayPEProcessor()
{
  // nothing to see here
}

void QuadratureIACTArrayPEProcessor::visit_event(
  const calin::simulation::tracker::Event& event,
  bool& kill_event)
{
  // nothing to see here
}

void QuadratureIACTArrayPEProcessor::visit_cherenkov_track(
  const calin::simulation::air_cherenkov_tracker::AirCherenkovTrack& cherenkov_track,
  bool& kill_track)
{
  // nothing to see here
}

void QuadratureIACTArrayPEProcessor::process_pe(unsigned scope_id,
  unsigned pixel_id, double x, double y, double t0, double pe_weight)
{
  // nothing to see here
}

void QuadratureIACTArrayPEProcessor::leave_cherenkov_track()
{
  // nothing to see here
}

void QuadratureIACTArrayPEProcessor::leave_event()
{
  // nothing to see here
}

SimpleImagePEProcessor::
SimpleImagePEProcessor(unsigned nscope, unsigned npix):
  QuadratureIACTArrayPEProcessor(),
  images_(nscope, std::vector<double>(npix))
{
  // nothing to see here
}

SimpleImagePEProcessor::
SimpleImagePEProcessor(const std::vector<unsigned> npix):
  QuadratureIACTArrayPEProcessor(), images_()
{
  for(auto inpix : npix)images_.emplace_back(inpix);
}

SimpleImagePEProcessor::~SimpleImagePEProcessor()
{
  // nothing to see here
}

void SimpleImagePEProcessor::visit_event(
  const calin::simulation::tracker::Event& event, bool& kill_event)
{
  clear_all_images();
}

void SimpleImagePEProcessor::
process_pe(unsigned scope_id, unsigned pixel_id, double x, double y,
  double t0, double pe_weight)
{
  if(scope_id >= images_.size())
    throw std::out_of_range("SimpleImagePEProcessor::process_pe: scope_id out "
      "of range");
  if(pixel_id >= images_[scope_id].size())
    throw std::out_of_range("SimpleImagePEProcessor::process_pe: pixel_id out "
      "of range");
  images_[scope_id][pixel_id] += pe_weight;
}

const std::vector<double>
SimpleImagePEProcessor::scope_image(unsigned iscope) const
{
  if(iscope >= images_.size())
    throw std::out_of_range("SimpleImagePEProcessor::scope_image: iscope out "
      "of range");
  return images_[iscope];
}

void SimpleImagePEProcessor::clear_all_images()
{
  for(auto& image : images_)std::fill(image.begin(), image.end(), 0.0);
}

VSO_IACTDetectorSphereHitProcessor::VSO_IACTDetectorSphereHitProcessor(
    VSO_QuadratureIACTArrayIntegrationHitVisitor* quadrature,
    calin::simulation::vs_optics::VSOTelescope* scope):
  IACTDetectorSphereHitProcessor(), quadrature_(quadrature), scope_(scope)
{
  // nothing to see here
}

VSO_IACTDetectorSphereHitProcessor::~VSO_IACTDetectorSphereHitProcessor()
{
  // nothing to see here
}

void VSO_IACTDetectorSphereHitProcessor::
process_hit(
  const calin::simulation::iact_array_tracker::IACTDetectorSphereHit& hit)
{
  quadrature_->process_hit(hit, scope_);
}


VSO_QuadratureIACTArrayIntegrationHitVisitor::
VSO_QuadratureIACTArrayIntegrationHitVisitor(double test_ray_spacing,
    calin::simulation::vs_optics::VSOArray* array,
    QuadratureIACTArrayPEProcessor* visitor,
    calin::math::rng::RNG* rng,
    bool adopt_array, bool adopt_visitor, bool adopt_rng):
  calin::simulation::iact_array_tracker::HitIACTVisitor(),
  test_ray_spacing_(test_ray_spacing),
  array_(array), adopt_array_(adopt_array),
  visitor_(visitor), adopt_visitor_(adopt_visitor),
  rng_(rng ? rng : new calin::math::rng::RNG),
  adopt_rng_(rng ? true : adopt_rng),
  ray_tracer_(new calin::simulation::vs_optics::VSORayTracer(array_, rng_)),
  num_hit_(array_->numTelescopes()), num_miss_(array_->numTelescopes()),
  ray_tracer_status_(array_->numTelescopes())
{
  // nothing to see here
}

VSO_QuadratureIACTArrayIntegrationHitVisitor::
~VSO_QuadratureIACTArrayIntegrationHitVisitor()
{
  if(adopt_array_)delete array_;
  if(adopt_visitor_)delete visitor_;
  delete ray_tracer_;
  if(adopt_rng_)delete rng_;
}

std::vector<IACTDetectorSphere>
VSO_QuadratureIACTArrayIntegrationHitVisitor::spheres()
{
  std::vector<IACTDetectorSphere> s;
  for(unsigned iscope=0; iscope<array_->numTelescopes(); iscope++)
  {
    auto* scope = array_->telescope(iscope);
    s.emplace_back(scope->pos(), SQR(0.5*scope->reflectorIP()),
      new VSO_IACTDetectorSphereHitProcessor(this, scope));
  }
  return s;
}

void VSO_QuadratureIACTArrayIntegrationHitVisitor::
visit_event(const calin::simulation::tracker::Event& event, bool& kill_event)
{
  std::fill(num_hit_.begin(), num_hit_.end(), 0);
  std::fill(num_miss_.begin(), num_miss_.end(), 0);
  visitor_->visit_event(event, kill_event);
}

void VSO_QuadratureIACTArrayIntegrationHitVisitor::visit_cherenkov_track(
  const calin::simulation::air_cherenkov_tracker::AirCherenkovTrack& cherenkov_track,
  bool& kill_track)
{
  visitor_->visit_cherenkov_track(cherenkov_track, kill_track);
}

void VSO_QuadratureIACTArrayIntegrationHitVisitor::leave_cherenkov_track()
{
  visitor_->leave_cherenkov_track();
}

void VSO_QuadratureIACTArrayIntegrationHitVisitor::leave_event()
{
  visitor_->leave_event();
}

unsigned
VSO_QuadratureIACTArrayIntegrationHitVisitor::num_hit(unsigned iscope) const
{
  if(iscope >= num_hit_.size())
    throw std::out_of_range(
      "VSO_QuadratureIACTArrayIntegrationHitVisitor::num_hit: iscope out "
      "of range");
  return num_hit_[iscope];
}

unsigned
VSO_QuadratureIACTArrayIntegrationHitVisitor::num_miss(unsigned iscope) const
{
  if(iscope >= num_miss_.size())
    throw std::out_of_range(
      "VSO_QuadratureIACTArrayIntegrationHitVisitor::num_miss: iscope out "
      "of range");
  return num_miss_[iscope];
}

const std::vector<unsigned> VSO_QuadratureIACTArrayIntegrationHitVisitor::
raytracer_status(unsigned iscope) const
{
  if(iscope >= ray_tracer_status_.size())
    throw std::out_of_range("iscope out of range");

#ifdef ENABLE_RAYTRACER_STATUS
  return ray_tracer_status_[iscope];
#else
  throw std::runtime_error("Ray tracer status not enabled at compile time");
#endif
}

void VSO_QuadratureIACTArrayIntegrationHitVisitor::process_test_ray(
  const calin::simulation::iact_array_tracker::IACTDetectorSphereHit& hit,
  calin::simulation::vs_optics::VSOTelescope* scope,
  double cos_phi, double sin_phi, double weight)
{
  Eigen::Vector3d p_hat = hit.u * hit.cherenkov_track->cos_thetac +
    (hit.v * cos_phi + hit.w * sin_phi)*hit.cherenkov_track->sin_thetac;

  math::vs_physics::Vec4D r0 { hit.cherenkov_track->t_mid,
    hit.cherenkov_track->x_mid };
  math::vs_physics::Vec4D p0 { 1, p_hat };
  math::vs_physics::Particle ray { r0, p0, 0 };
  VSOTraceInfo trace_info;
  const VSOPixel* pixel = ray_tracer_->trace(ray, trace_info, scope);

#if 0
  if(sin_phi==0) {
    LOG(INFO) << trace_info.status << ' ' << trace_info.mirror_hexid << ' '
      << ray.Position().r << ' ' << p_hat.norm() << " [ "
      << hit.u.transpose() << " ] [ "
      << hit.v.transpose() << "] [ "
      << hit.w.transpose() << "] " << hit.cherenkov_track->cos_thetac << ' '
      << hit.cherenkov_track->sin_thetac;
  }
#endif

#ifdef ENABLE_RAYTRACER_STATUS
  if(trace_info.status >= ray_tracer_status_[scope->id()].size())
    ray_tracer_status_[scope->id()].resize(trace_info.status+1);
  ray_tracer_status_[scope->id()][trace_info.status]++;
#endif

  if(trace_info.status==TS_PE_GENERATED and pixel!=nullptr) {
    num_hit_[scope->id()]++;
    visitor_->process_pe(scope->id(), pixel->id(),
      ray.Position().r.x, ray.Position().r.z, ray.Position().r0, weight);
  } else {
    num_miss_[scope->id()]++;
  }
}

void VSO_QuadratureIACTArrayIntegrationHitVisitor::process_hit(
  const calin::simulation::iact_array_tracker::IACTDetectorSphereHit& hit,
  calin::simulation::vs_optics::VSOTelescope* scope)
{
  double dphi = test_ray_spacing_*hit.cherenkov_track->cos_thetac /
    std::abs(2.0*M_PI*hit.rxu*hit.cherenkov_track->sin_thetac);
  unsigned n = std::floor(2.0*hit.phimax / dphi);
  if(n%2 == 0)++n; // always choose odd number of integration points
  dphi = 2.0*hit.phimax/double(n);
  const double cos_dphi = std::cos(dphi);
  const double sin_dphi = std::sin(dphi);
  double cos_phi = 1.0;
  double sin_phi = 0.0;
  unsigned n_half = (n+1)/2;
  double weight = dphi/(2.0*M_PI)*hit.cherenkov_track->yield_density;
  // Adapt weight for track length / QE / number of cone reflections etc...

#if 0
  LOG(INFO) << hit.x0.transpose() << ' '
    << "[ " << hit.u.dot(hit.u) << ' ' << hit.u.dot(hit.v) << ' ' << hit.u.dot(hit.w) << " ], "
    << "[ " << hit.v.dot(hit.u) << ' ' << hit.v.dot(hit.v) << ' ' << hit.v.dot(hit.w) << " ], "
    << "[ " << hit.w.dot(hit.u) << ' ' << hit.w.dot(hit.v) << ' ' << hit.w.dot(hit.w) << " ]";
#endif

  for(unsigned i=0; i<n_half ; i++)
  {
    if(i==0) {
      process_test_ray(hit, scope, cos_phi, sin_phi, weight);
    } else {
      process_test_ray(hit, scope, cos_phi, sin_phi, weight);
      process_test_ray(hit, scope, cos_phi, -sin_phi, weight);
    }
    const double sin_phi_ = sin_phi * cos_dphi + cos_phi * sin_dphi;
    const double cos_phi_ = cos_phi * cos_dphi - sin_phi * sin_dphi;
    sin_phi = sin_phi_;
    cos_phi = cos_phi_;
  }
}
