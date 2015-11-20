/* 

   calin/math/rng.cpp -- Stephen Fegan -- 2015-11-19

   Random number generators. Based on RandomNumbers_TNG written
   by the author while at UCLA in 2007.

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

/*! \file RandomNumbrs_TNG.hpp

  The next generation random number generator. Features NR3 generators,
  RanLux and old NR2 generator.

  \author     Stephen Fegan               \n
              UCLA                        \n
              sfegan@astro.ucla.edu       \n

  \version    $Revision: 1.5 $
  \date       10/31/2007

  $Id: RandomNumbers_TNG.hpp 5429 2013-06-27 13:40:55Z sfegan $

*/

#pragma once

namespace calin { namespace math { namespace rng {

// Decision : should this be a template or a base class. As a base
// class the code is simpler and more flexable. As a template it's
// faster. In most cases I guess the amount of time spent generating
// and processing of the deviates by the caller should outweigh the
// time spent doing the indirect call so we'll go with a base class
// and potentially reevaluate if profiling shows this assumption is
// wrong.

class RNGCore
{
 public:
  virtual ~RNGCore();
  virtual double uniform_double() = 0;
  virtual uint64_t uniform_uint64() = 0;
  virtual uint32_t uniform_uint32() = 0;
  virtual void save_to_proto() const = 0;
  static RNGCore* create_from_proto();
};

class RNG
{
 public:
  RNG(uint64_t seed = 0);
  RNG(RNGCore* core, bool adopt_core = false);
  ~RNG() { if(adopt_core_)delete core_; }
  
  double uniform_double() { return core_->uniform_double(); }
  double uniform_uint64() { return core_->uniform_uint64(); }
  double uniform_uint32() { return core_->uniform_uint32(); }
  
  double uniform() { return core_->uniform_double(); }
  double exponential() { return -std::log(uniform()); }
  double exponential(double mean) { return -mean*std::log(uniform()); }
  double normal();
  double normal(double mean, double sigma) { return mean+normal()*sigma; }
  double gamma(int ialpha);
  double gamma(double alpha, double beta=1.0);
  double gamma_by_mean_and_stddev(const double mean, const double stddev);
  uint32_t poisson(double lambda);
  int binomial(double pp, int n);
  inline double inverse_cdf(const std::vector< Pair > &inv_cdf);

  static void generate_inverse_cdf(std::vector< Pair > &cdf, unsigned nbins = 0);

 private:
  RNGCore core_;
  bool adopt_core_;
  
  bool bm_hascached_ = false;
  double m_bm_cachedval_ = 0.0;
};

} } } // namespace calin::math::rng