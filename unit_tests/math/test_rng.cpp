/* 

   calin/unit_tests/math/test_rng.cpp -- Stephen Fegan -- 2015-11-22

   Unit tests for RNG

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

#include <iostream>
#include <iomanip>
#include <gtest/gtest.h>
#include <vector>
#include <tuple>

#include <math/rng.hpp>

using namespace calin::math::rng;

template<typename T> std::tuple<double, double, double>
calc_moments(const T& generator, bool print = false, unsigned N = 1000000)
{
  RNG rng;
  double sum_x = 0;
  double sum_xx = 0;
  double sum_xxx = 0;
  for(unsigned i=0; i<N; i++) {
    double x;
    generator(rng,x);
    sum_x += x;
    sum_xx += x*x;
    sum_xxx += x*x*x;
  }
  double m1 = sum_x/double(N);
  double m2 = sum_xx/double(N) - m1*m1;
  double m3 = sum_xxx/double(N) - 3*sum_xx/double(N)*m1 + 2*m1*m1*m1;
  if(print)std::cout << m1 << ' ' << m2 << ' ' << m3 << '\n';
  return std::make_tuple(m1,m2,m3);
}

TEST(TestRNG, UniformMoments) {
  double m1, m2, m3;
  std::tie(m1,m2,m3) = calc_moments([](RNG& rng,double& x){ x=rng.uniform(); });
  EXPECT_NEAR(m1, 0.5, 0.01);
  EXPECT_NEAR(m2, 1.0/12.0, 0.01);
  EXPECT_NEAR(m3, 0.0, 0.001);
}

TEST(TestRNG, ExponentialMoments) {
  double m1, m2, m3;
  std::tie(m1,m2,m3) = calc_moments([](RNG& rng,double& x){
      x=rng.exponential(); });
  EXPECT_NEAR(m1, 1.0, 0.01);
  EXPECT_NEAR(m2, 1.0, 0.01);
  EXPECT_NEAR(m3, 2.0, 0.05);
}

class ExponentialWithMean : public testing::TestWithParam<double> {
 public:
};

TEST_P(ExponentialWithMean, Moments) {
  double mean = GetParam();
  double m1, m2, m3;
  std::tie(m1,m2,m3) = calc_moments([mean](RNG& rng,double& x){
      x=rng.exponential(mean); });
  EXPECT_NEAR(m1, mean, 0.01*mean);
  EXPECT_NEAR(m2, mean*mean, 0.01*mean*mean);
  EXPECT_NEAR(m3, 2.0*mean*mean*mean, 0.05*mean*mean*mean);
}

INSTANTIATE_TEST_CASE_P(TestRNG,
                        ExponentialWithMean,
                        ::testing::Values(1.0, 2.0, 3.0));

TEST(TestRNG, NormalMoments) {
  double m1, m2, m3;
  std::tie(m1,m2,m3) = calc_moments([](RNG& rng,double& x){
      x=rng.normal(); });
  EXPECT_NEAR(m1, 0.0, 0.01);
  EXPECT_NEAR(m2, 1.0, 0.01);
  EXPECT_NEAR(m3, 0.0, 0.05);
}

class NormalWithMeanAndSigma :
    public testing::TestWithParam<std::pair<double,double>> {
 public:
};

TEST_P(NormalWithMeanAndSigma, Moments) {
  double mean = GetParam().first;
  double sigma = GetParam().second;
  double m1, m2, m3;
  std::tie(m1,m2,m3) = calc_moments([mean,sigma](RNG& rng,double& x){
      x=rng.normal(mean,sigma); });
  EXPECT_NEAR(m1, mean, 0.01*std::abs(mean));
  EXPECT_NEAR(m2, sigma*sigma, 0.01*sigma*sigma);
  EXPECT_NEAR(m3, 0, 0.05*sigma*sigma*sigma);
}

INSTANTIATE_TEST_CASE_P(TestRNG,
                        NormalWithMeanAndSigma,
                        ::testing::Values(std::make_pair(1.0, 2.0),
                                          std::make_pair(-1.0, 2.0)));

class PoissonWithMean : public testing::TestWithParam<double> {
 public:
};

TEST_P(PoissonWithMean, Moments) {
  double mean = GetParam();
  double m1, m2, m3;
  std::tie(m1,m2,m3) = calc_moments([mean](RNG& rng,double& x){
      x=double(rng.poisson(mean)); });
  EXPECT_NEAR(m1, mean, 0.01*mean);
  EXPECT_NEAR(m2, mean, 0.01*mean);
  EXPECT_NEAR(m3, mean, 0.1*mean);
}

// Make sure to test values <5 and >5 since the generator uses
// different algorithms for these cases
INSTANTIATE_TEST_CASE_P(TestRNG,
                        PoissonWithMean,
                        ::testing::Values(1.0, 2.0, 3.0, 4.99999, 5.0,
                                          5.00001, 10.0, 100.0));

class GammaByAlphaAndBeta :
    public testing::TestWithParam<std::pair<double,double>> {
 public:
};

TEST_P(GammaByAlphaAndBeta, Moments) {
  double alpha = GetParam().first;
  double beta = GetParam().second;
  double m1, m2, m3;
  std::tie(m1,m2,m3) = calc_moments([alpha,beta](RNG& rng,double& x){
      x=rng.gamma_by_alpha_and_beta(alpha,beta); });
  EXPECT_NEAR(m1, alpha/beta, 0.01*alpha/beta/beta);
  EXPECT_NEAR(m2, alpha/(beta*beta), 0.05*alpha/(beta*beta*beta));
  EXPECT_NEAR(m3, 2.0*alpha/(beta*beta*beta), 0.1*alpha/(beta*beta*beta));
}

// Test alpha <1 and alpha>1
INSTANTIATE_TEST_CASE_P(TestRNG,
                        GammaByAlphaAndBeta,
                        ::testing::Values(std::make_pair(0.1, 1.0),
                                          std::make_pair(0.99999, 1.0),
                                          std::make_pair(1.0, 1.0),
                                          std::make_pair(1.00001, 1.0),
                                          std::make_pair(1.0, 2.0),
                                          std::make_pair(2.0, 1.0)));

class GammaByMeanAndSigma :
    public testing::TestWithParam<std::pair<double,double>> {
 public:
};

TEST_P(GammaByMeanAndSigma, Moments) {
  double mean = GetParam().first;
  double sigma = GetParam().second;
  double m1, m2, m3;
  std::tie(m1,m2,m3) = calc_moments([mean,sigma](RNG& rng,double& x){
      x=rng.gamma_by_mean_and_sigma(mean,sigma); });
  EXPECT_NEAR(m1, mean, 0.01*mean);
  EXPECT_NEAR(m2, sigma*sigma, 0.01*sigma*sigma);
  EXPECT_NEAR(m3, 2.0*(sigma*sigma*sigma*sigma)/mean,
              0.1*(sigma*sigma*sigma*sigma)/mean);
}

INSTANTIATE_TEST_CASE_P(TestRNG,
                        GammaByMeanAndSigma,
                        ::testing::Values(std::make_pair(1.0, 0.1),
                                          std::make_pair(1.0, 1.0),
                                          std::make_pair(2.0, 1.0)));

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
