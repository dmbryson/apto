/*
 *  unittests/stat/Accumulator.cc
 *  Apto
 *
 *  Created by David on 2/16/12.
 *  Copyright 2012 David Michael Bryson. All rights reserved.
 *  http://programerror.com/software/apto
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *  
 *  1.  Redistributions of source code must retain the above copyright notice, this list of conditions and the
 *      following disclaimer.
 *  2.  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *      following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3.  Neither the name of David Michael Bryson, nor the names of contributors may be used to endorse or promote
 *      products derived from this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY DAVID MICHAEL BRYSON AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL DAVID MICHAEL BRYSON OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  Authors: David M. Bryson <david@programerror.com>
 *
 */

#include "apto/stat/Accumulator.h"

#include "apto/platform.h"

#include "gtest/gtest.h"


#if APTO_PLATFORM(WINDOWS)
#include <float.h>
namespace std {
  bool isnan(double x) { return _isnan(x); }
}
#endif


TEST(StatAccumulator, Count) {
  Apto::Stat::Accumulator<int> ac;
  ac.Add(1);
  EXPECT_EQ(1u, ac.Count());
  
  ac.Add(2);
  EXPECT_EQ(2u, ac.Count());
  
  ac.Add(3);
  EXPECT_EQ(3u, ac.Count());
  
  ac.Clear();
  EXPECT_EQ(0u, ac.Count());
  
  ac.Add(4);
  EXPECT_EQ(1u, ac.Count());
}

TEST(StatAccumulator, Sum) {
  Apto::Stat::Accumulator<int> ac;
  
  ac.Add(1);
  EXPECT_EQ(1, ac.Sum());
  
  ac.Add(2);
  EXPECT_EQ(3, ac.Sum());
  
  ac.Add(3);
  EXPECT_EQ(6, ac.Sum());
  
  ac.Clear();
  EXPECT_EQ(0, ac.Sum());
  
  ac.Add(4);
  EXPECT_EQ(4, ac.Sum());
}

TEST(StatAccumulator, Mean) {
  Apto::Stat::Accumulator<int> ac;
  
  ac.Add(1);
  EXPECT_EQ(1, ac.Mean());
  
  ac.Add(2);
  EXPECT_EQ(1.5, ac.Mean());
  
  ac.Add(3);
  EXPECT_EQ(2, ac.Mean());
  
  ac.Clear();
  EXPECT_TRUE(std::isnan(ac.Mean()));
  
  ac.Add(4);
  EXPECT_EQ(4, ac.Mean());
}


TEST(StatAccumulator, Min) {
  Apto::Stat::Accumulator<int> ac;
  
  ac.Add(1);
  EXPECT_EQ(1, ac.Min());
  
  ac.Add(2);
  EXPECT_EQ(1, ac.Min());
  
  ac.Add(-1);
  EXPECT_EQ(-1, ac.Min());
}


TEST(StatAccumulator, Max) {
  Apto::Stat::Accumulator<int> ac;
  
  ac.Add(1);
  EXPECT_EQ(1, ac.Max());
  
  ac.Add(2);
  EXPECT_EQ(2, ac.Max());
  
  ac.Add(-1);
  EXPECT_EQ(2, ac.Max());
}


TEST(StatAccumulator, Variance) {
  Apto::Stat::Accumulator<int> ac;
  
  ac.Add(1);
  ac.Add(2);
  ac.Add(3);
  ac.Add(4);
  ac.Add(5);
  ac.Add(6);
  EXPECT_LT(2.91666666, ac.Variance());
  EXPECT_GT(2.91666667, ac.Variance());
}


TEST(StatAccumulator, StdError) {
  Apto::Stat::Accumulator<int> ac;
  
  ac.Add(1);
  ac.Add(2);
  ac.Add(3);
  ac.Add(4);
  ac.Add(5);
  ac.Add(6);
  EXPECT_LT(0.76376261, ac.StdError());
  EXPECT_GT(0.76376262, ac.StdError());
}

