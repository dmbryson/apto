/*
 *  unittests/core/StringUtils.cc
 *  Apto
 *
 *  Created by David on 3/15/11.
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

#include "apto/core/String.h"
#include "apto/core/StringUtils.h"

#include "gtest/gtest.h"


TEST(CoreStringUtils, StrAs) {
  // Conversion to bool
  EXPECT_TRUE(Apto::StrAs("1"));
  EXPECT_TRUE(Apto::StrAs("t"));
  EXPECT_TRUE(Apto::StrAs("T"));
  EXPECT_TRUE(Apto::StrAs("true"));
  EXPECT_FALSE(Apto::StrAs("0"));
  EXPECT_FALSE(Apto::StrAs("f"));
  EXPECT_FALSE(Apto::StrAs("F"));
  EXPECT_FALSE(Apto::StrAs("false"));
  EXPECT_FALSE(Apto::StrAs("foo"));

  EXPECT_TRUE(Apto::StrAs(Apto::String("1")));
  EXPECT_TRUE(Apto::StrAs(Apto::String("t")));
  EXPECT_TRUE(Apto::StrAs(Apto::String("T")));
  EXPECT_TRUE(Apto::StrAs(Apto::String("true")));
  EXPECT_FALSE(Apto::StrAs(Apto::String("0")));
  EXPECT_FALSE(Apto::StrAs(Apto::String("f")));
  EXPECT_FALSE(Apto::StrAs(Apto::String("F")));
  EXPECT_FALSE(Apto::StrAs(Apto::String("false")));
  EXPECT_FALSE(Apto::StrAs(Apto::String("foo")));
  
  // Conversion to int
  EXPECT_EQ((int)1, (int)Apto::StrAs("1"));
  EXPECT_EQ((int)-1, (int)Apto::StrAs("-1"));
  EXPECT_EQ((int)2147483647, (int)Apto::StrAs("2147483647"));
  EXPECT_EQ((int)(-2147483647-1), (int)Apto::StrAs("-2147483648"));
  EXPECT_EQ((int)0, (int)Apto::StrAs("0foo"));
  EXPECT_EQ((int)0, (int)Apto::StrAs("foo"));  
  
  // Conversion to long
  EXPECT_EQ((long)1, (long)Apto::StrAs("1"));
  EXPECT_EQ((long)-1, (long)Apto::StrAs("-1"));
  EXPECT_EQ((long)2147483647, (long)Apto::StrAs("2147483647"));
  EXPECT_EQ((long)(-2147483647-1), (long)Apto::StrAs("-2147483648"));
  EXPECT_EQ((long)0, (long)Apto::StrAs("0foo"));
  EXPECT_EQ((long)0, (long)Apto::StrAs("foo"));  
  
  // Conversion to long long
  EXPECT_EQ((long long)1, (long long)Apto::StrAs("1"));
  EXPECT_EQ((long long)-1, (long long)Apto::StrAs("-1"));
  EXPECT_EQ((long long)2147483647, (long long)Apto::StrAs("2147483647"));
  EXPECT_EQ((long long)(-2147483647-1), (long long)Apto::StrAs("-2147483648"));
  EXPECT_EQ((long long)22147483647ll, (long long)Apto::StrAs("22147483647"));
  EXPECT_EQ((long long)-22147483648ll, (long long)Apto::StrAs("-22147483648"));
  EXPECT_EQ((long long)0, (long long)Apto::StrAs("0foo"));
  EXPECT_EQ((long long)0, (long long)Apto::StrAs("foo"));  
  
  // Conversion to unsigned int
  EXPECT_EQ((unsigned int)1, (unsigned int)Apto::StrAs("1"));
  EXPECT_EQ((unsigned int)-1, (unsigned int)Apto::StrAs("-1"));
  EXPECT_EQ((unsigned int)2147483647, (unsigned int)Apto::StrAs("2147483647"));
  EXPECT_EQ((unsigned int)(-2147483647-1), (unsigned int)Apto::StrAs("-2147483648"));
  EXPECT_EQ((unsigned int)0, (unsigned int)Apto::StrAs("0foo"));
  EXPECT_EQ((unsigned int)0, (unsigned int)Apto::StrAs("foo"));  
  
  // Conversion to unsigned long
  EXPECT_EQ((unsigned long)1, (unsigned long)Apto::StrAs("1"));
  EXPECT_EQ((unsigned long)-1, (unsigned long)Apto::StrAs("-1"));
  EXPECT_EQ((unsigned long)2147483647, (unsigned long)Apto::StrAs("2147483647"));
  EXPECT_EQ((unsigned long)(-2147483647-1), (unsigned long)Apto::StrAs("-2147483648"));
  EXPECT_EQ((unsigned long)0, (unsigned long)Apto::StrAs("0foo"));
  EXPECT_EQ((unsigned long)0, (unsigned long)Apto::StrAs("foo"));  
  
  // Conversion to unsigned long long
  EXPECT_EQ((unsigned long long)1, (unsigned long long)Apto::StrAs("1"));
  EXPECT_EQ((unsigned long long)-1, (unsigned long long)Apto::StrAs("-1"));
  EXPECT_EQ((unsigned long long)2147483647, (unsigned long long)Apto::StrAs("2147483647"));
  EXPECT_EQ((unsigned long long)(-2147483647-1), (unsigned long long)Apto::StrAs("-2147483648"));
  EXPECT_EQ((unsigned long long)22147483647ll, (unsigned long long)Apto::StrAs("22147483647"));
  EXPECT_EQ((unsigned long long)-22147483648ll, (unsigned long long)Apto::StrAs("-22147483648"));
  EXPECT_EQ((unsigned long long)0, (unsigned long long)Apto::StrAs("0foo"));
  EXPECT_EQ((unsigned long long)0, (unsigned long long)Apto::StrAs("foo"));  
  
  // Conversion to float
  EXPECT_EQ((float)1.0, (float)Apto::StrAs("1.0"));
  EXPECT_EQ((float)-1.0, (float)Apto::StrAs("-1.0"));
  EXPECT_EQ((float)16.0e-10, (float)Apto::StrAs("16.0e-10"));
  EXPECT_EQ((float)-1.5e12, (float)Apto::StrAs("-1.5e12"));
  
  // Conversion to double
  EXPECT_EQ((double)1.0, (double)Apto::StrAs("1.0"));
  EXPECT_EQ((double)-1.0, (double)Apto::StrAs("-1.0"));
  EXPECT_EQ((double)16.0e-10, (double)Apto::StrAs("16.0e-10"));
  EXPECT_EQ((double)-1.5e12, (double)Apto::StrAs("-1.5e12"));
  
  // Conversion to long double
  EXPECT_EQ((long double)1.0L, (long double)Apto::StrAs("1.0"));
  EXPECT_EQ((long double)-1.0L, (long double)Apto::StrAs("-1.0"));
  EXPECT_EQ((long double)1.6e-10L, (long double)Apto::StrAs("1.6e-10"));
  EXPECT_EQ((long double)-1.5e12L, (long double)Apto::StrAs("-1.5e12"));
}

TEST(CoreStringUtils, AsStr) {
  EXPECT_TRUE(Apto::String("12") == Apto::AsStr(12));
  Apto::String test_str(Apto::AsStr(1.5));
  EXPECT_TRUE(test_str[0] == '1' && test_str[1] == '.' && test_str[2] == '5');
  EXPECT_TRUE(Apto::String("true") == Apto::AsStr(true));
  EXPECT_TRUE(Apto::String("false") == Apto::AsStr(false));
  EXPECT_TRUE(Apto::String("barbaz") == Apto::AsStr("barbaz"));
  EXPECT_TRUE(Apto::String("barbazf") == Apto::AsStr(Apto::String("barbazf")));
}

TEST(CoreStringUtils, FormatStr) {
  Apto::String str = Apto::FormatStr("%d foo", 42);
  EXPECT_TRUE(str == "42 foo");
}
