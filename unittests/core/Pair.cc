/*
 *  unittests/core/Pair.cc
 *  Apto
 *
 *  Created by David on 2/14/11.
 *  Copyright 2011 David Michael Bryson. All rights reserved.
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

#include "apto/core/Pair.h"

#include "gtest/gtest.h"


TEST(CorePair, Construction) {
  Apto::Pair<int, int> p1; 
  
  Apto::Pair<int, int> p2(3);
  EXPECT_EQ(3, p2.Value1());
  
  Apto::Pair<int, int> p3(4, 5);
  EXPECT_EQ(4, p3.Value1());
  EXPECT_EQ(5, p3.Value2());
  
  Apto::Pair<int, int> p4(p3);
  EXPECT_EQ(4, p4.Value1());
  EXPECT_EQ(5, p4.Value2());
}

TEST(CorePair, Assignment) {
  Apto::Pair<int, int> p1(4, 5);
  EXPECT_EQ(4, p1.Value1());
  EXPECT_EQ(5, p1.Value2());
  

  Apto::Pair<int, int> p2;
  p2 = p1;
  EXPECT_EQ(4, p2.Value1());
  EXPECT_EQ(5, p2.Value2());  
}

TEST(CorePair, Set) {
  Apto::Pair<int, int> p1(4, 5);
  EXPECT_EQ(4, p1.Value1());
  EXPECT_EQ(5, p1.Value2());
  
  p1.Set(8, 10);
  EXPECT_EQ(8, p1.Value1());
  EXPECT_EQ(10, p1.Value2());
  
  p1.Value1() = 18;
  p1.Value2() = 20;
  EXPECT_EQ(18, p1.Value1());
  EXPECT_EQ(20, p1.Value2());
}
