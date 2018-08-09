/*

   calin/unit_tests/util/test_vcl.cpp -- Stephen Fegan -- 2018-08-08

   Unit tests for vcl

   Copyright 2018, Stephen Fegan <sfegan@llr.in2p3.fr>
   LLR, Ecole Polytechnique, CNRS/IN2P3

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

#include <util/vcl.hpp>
#include <math/rng_vcl.hpp>

using namespace calin::util::vcl;
using namespace calin::math::rng;

TEST(TestVCL, Print) {
  Vec8f v(1.234);
  std::cout << v << '\n';
  Vec8i vi(-101);
  std::cout << vi << '\n';
  std::cout << to_float(vi) << '\n';
  std::cout << to_float(vi) + 0.5 << '\n';
}

TEST(TestVCL, RNG128) {
  uint64_t seeds[2] = { 1, 2 };
  NR3_VCLRNGCore<VCL128Architecture> rng(seeds);
  NR3RNGCore rng0(seeds[0]);
  NR3RNGCore rng1(seeds[1]);
  std::cout << rng.uniform_uint64() << ' '
    << rng0.uniform_uint64() << ' '
    << rng1.uniform_uint64() << '\n';

  std::cout << rng.uniform_uint64() << ' '
    << rng0.uniform_uint64() << ' '
    << rng1.uniform_uint64() << '\n';
}

TEST(TestVCL, RNG256) {
  uint64_t seeds[4] = { 1, 2, 3, 4 };
  NR3_VCLRNGCore<VCL256Architecture> rng(seeds);
  NR3RNGCore rng0(seeds[0]);
  NR3RNGCore rng1(seeds[1]);
  NR3RNGCore rng2(seeds[2]);
  NR3RNGCore rng3(seeds[3]);
  std::cout << rng.uniform_uint64() << ' '
    << rng0.uniform_uint64() << ' '
    << rng1.uniform_uint64() << ' '
    << rng2.uniform_uint64() << ' '
    << rng3.uniform_uint64() << '\n';

  std::cout << rng.uniform_uint64() << ' '
    << rng0.uniform_uint64() << ' '
    << rng1.uniform_uint64() << ' '
    << rng2.uniform_uint64() << ' '
    << rng3.uniform_uint64() << '\n';
}

TEST(TestVCL, RNG512) {
  uint64_t seeds[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
  NR3_VCLRNGCore<VCL512Architecture> rng(seeds);
  NR3RNGCore rng0(seeds[0]);
  NR3RNGCore rng1(seeds[1]);
  NR3RNGCore rng2(seeds[2]);
  NR3RNGCore rng3(seeds[3]);
  NR3RNGCore rng4(seeds[4]);
  NR3RNGCore rng5(seeds[5]);
  NR3RNGCore rng6(seeds[6]);
  NR3RNGCore rng7(seeds[7]);
  std::cout << rng.uniform_uint64() << ' '
    << rng0.uniform_uint64() << ' '
    << rng1.uniform_uint64() << ' '
    << rng2.uniform_uint64() << ' '
    << rng3.uniform_uint64() << ' '
    << rng4.uniform_uint64() << ' '
    << rng5.uniform_uint64() << ' '
    << rng6.uniform_uint64() << ' '
    << rng7.uniform_uint64() << '\n';

  std::cout << rng.uniform_uint64() << ' '
    << rng0.uniform_uint64() << ' '
    << rng1.uniform_uint64() << ' '
    << rng2.uniform_uint64() << ' '
    << rng3.uniform_uint64() << ' '
    << rng4.uniform_uint64() << ' '
    << rng5.uniform_uint64() << ' '
    << rng6.uniform_uint64() << ' '
    << rng7.uniform_uint64() << '\n';
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
