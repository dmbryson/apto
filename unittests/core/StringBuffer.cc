/*
 *  unittests/core/StringBuffer.cc
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

#include "apto/core/String.h"
#include "apto/core/StringBuffer.h"

#include "gtest/gtest.h"



TEST(CoreStringBuffer, Construction) {
  Apto::StringBuffer s1;
  EXPECT_EQ(0, s1.GetSize());
  
  // Test Construction with strings
  const char* teststr = "Foo Bar Baz";
  Apto::StringBuffer s2(teststr);
  EXPECT_EQ(11, s2.GetSize());
  Apto::StringBuffer s3(3, teststr);
  EXPECT_EQ(3, s3.GetSize());
  
  Apto::String s4(s3);
  Apto::StringBuffer s5(s4);
}


TEST(CoreStringBuffer, Indexing) {
  Apto::StringBuffer s1("foo");
  EXPECT_EQ('f', s1[0]);
  EXPECT_EQ('o', s1[1]);
  EXPECT_EQ('o', s1[2]);
  
  s1[0] = 'b';
  s1[1] = 'a';
  s1[2] = 'r';
  
  EXPECT_EQ('b', s1[0]);
  EXPECT_EQ('a', s1[1]);
  EXPECT_EQ('r', s1[2]);
  
  // Make a copy (shared backing buffer)
  Apto::StringBuffer s2(s1);
  s2[2] = 'z';
  
  EXPECT_EQ('b', s1[0]);
  EXPECT_EQ('a', s1[1]);
  EXPECT_EQ('r', s1[2]);
  
  EXPECT_EQ('b', s2[0]);
  EXPECT_EQ('a', s2[1]);
  EXPECT_EQ('z', s2[2]);
}


TEST(CoreStringBuffer, Assignment) {
  Apto::StringBuffer s1("foo");
  EXPECT_EQ('f', s1[0]);
  EXPECT_EQ('o', s1[1]);
  EXPECT_EQ('o', s1[2]);
  Apto::StringBuffer s2("bar");
  EXPECT_EQ('b', s2[0]);
  EXPECT_EQ('a', s2[1]);
  EXPECT_EQ('r', s2[2]);
  
  Apto::StringBuffer s3(s1);
  EXPECT_EQ('f', s3[0]);
  EXPECT_EQ('o', s3[1]);
  EXPECT_EQ('o', s3[2]);
  
  s2 = s1;
  EXPECT_EQ('f', s2[0]);
  EXPECT_EQ('o', s2[1]);
  EXPECT_EQ('o', s2[2]);
  
  s3 = "baz";
  EXPECT_EQ('b', s3[0]);
  EXPECT_EQ('a', s3[1]);
  EXPECT_EQ('z', s3[2]);
}


TEST(CoreStringBuffer, Iterators) {
  Apto::StringBuffer s1("abcdef");
  
  Apto::StringBuffer::Iterator it = s1.Begin();
  for (int i = 0; it.Next(); i++) EXPECT_EQ('a' + i, *it.Get());
  
  const Apto::StringBuffer& cs1 = s1;
  Apto::StringBuffer::ConstIterator cit = cs1.Begin();
  for (int i = 0; cit.Next(); i++) EXPECT_EQ('a' + i, *cit.Get());  
}


TEST(CoreStringBuffer, Comparison) {
  Apto::StringBuffer s1("bar");
  Apto::StringBuffer s2("baz");
  
  EXPECT_TRUE(s1 == s1);
  EXPECT_TRUE(s2 == s2);
  EXPECT_FALSE(s1 == s2);
  EXPECT_FALSE(s2 == s1);
  EXPECT_TRUE(s1 == "bar");
  EXPECT_TRUE("bar" == s1);
  EXPECT_FALSE(s1 == "baz");
  EXPECT_FALSE("baz" == s1);
  EXPECT_TRUE(s2 == "baz");
  EXPECT_TRUE("baz" == s2);
  EXPECT_FALSE(s2 == "bar");
  EXPECT_FALSE("bar" == s2);
  
  EXPECT_FALSE(s1 != s1);
  EXPECT_FALSE(s2 != s2);
  EXPECT_TRUE(s1 != s2);
  EXPECT_TRUE(s2 != s1);
  EXPECT_FALSE(s1 != "bar");
  EXPECT_FALSE("bar" != s1);
  EXPECT_TRUE(s1 != "baz");
  EXPECT_TRUE("baz" != s1);
  EXPECT_FALSE(s2 != "baz");
  EXPECT_FALSE("baz" != s2);
  EXPECT_TRUE(s2 != "bar");
  EXPECT_TRUE("bar" != s2);
  
  EXPECT_FALSE(s1 < s1);
  EXPECT_FALSE(s2 < s2);
  EXPECT_TRUE(s1 < s2);
  EXPECT_FALSE(s2 < s1);
  EXPECT_FALSE(s1 < "bar");
  EXPECT_FALSE("bar" < s1);
  EXPECT_TRUE(s1 < "baz");
  EXPECT_FALSE("baz" < s1);
  EXPECT_FALSE(s2 < "bar");
  EXPECT_TRUE("bar" < s2);
  EXPECT_FALSE(s2 < "baz");
  EXPECT_FALSE("baz" < s2);
  
  EXPECT_FALSE(s1 > s1);
  EXPECT_FALSE(s2 > s2);
  EXPECT_FALSE(s1 > s2);
  EXPECT_TRUE(s2 > s1);
  EXPECT_FALSE(s1 > "bar");
  EXPECT_FALSE("bar" > s1);
  EXPECT_FALSE(s1 > "baz");
  EXPECT_TRUE("baz" > s1);
  EXPECT_TRUE(s2 > "bar");
  EXPECT_FALSE("bar" > s2);
  EXPECT_FALSE(s2 > "baz");
  EXPECT_FALSE("baz" > s2);
  
  EXPECT_TRUE(s1 <= s1);
  EXPECT_TRUE(s2 <= s2);
  EXPECT_TRUE(s1 <= s2);
  EXPECT_FALSE(s2 <= s1);
  EXPECT_TRUE(s1 <= "bar");
  EXPECT_TRUE("bar" <= s1);
  EXPECT_TRUE(s1 <= "baz");
  EXPECT_FALSE("baz" <= s1);
  EXPECT_FALSE(s2 <= "bar");
  EXPECT_TRUE("bar" <= s2);
  EXPECT_TRUE(s2 <= "baz");
  EXPECT_TRUE("baz" <= s2);
  
  EXPECT_TRUE(s1 >= s1);
  EXPECT_TRUE(s2 >= s2);
  EXPECT_FALSE(s1 >= s2);
  EXPECT_TRUE(s2 >= s1);
  EXPECT_TRUE(s1 >= "bar");
  EXPECT_TRUE("bar" >= s1);
  EXPECT_FALSE(s1 >= "baz");
  EXPECT_TRUE("baz" >= s1);
  EXPECT_TRUE(s2 >= "bar");
  EXPECT_FALSE("bar" >= s2);
  EXPECT_TRUE(s2 >= "baz");
  EXPECT_TRUE("baz" >= s2);
}


TEST(CoreStringBuffer, Concatenation) {
  Apto::StringBuffer s1("foo");
  EXPECT_EQ(3, s1.GetSize());
  EXPECT_EQ('f', s1[0]);
  EXPECT_EQ('o', s1[1]);
  EXPECT_EQ('o', s1[2]);
  
  s1 += 'b';
  EXPECT_EQ(4, s1.GetSize());
  EXPECT_EQ('f', s1[0]);
  EXPECT_EQ('o', s1[1]);
  EXPECT_EQ('o', s1[2]);
  EXPECT_EQ('b', s1[3]);
  
  s1 += "ar";
  EXPECT_EQ(6, s1.GetSize());
  EXPECT_EQ('f', s1[0]);
  EXPECT_EQ('o', s1[1]);
  EXPECT_EQ('o', s1[2]);
  EXPECT_EQ('b', s1[3]);
  EXPECT_EQ('a', s1[4]);
  EXPECT_EQ('r', s1[5]);
  
  s1 += Apto::StringBuffer("baz");
  EXPECT_EQ(9, s1.GetSize());
  EXPECT_EQ('f', s1[0]);
  EXPECT_EQ('o', s1[1]);
  EXPECT_EQ('o', s1[2]);
  EXPECT_EQ('b', s1[3]);
  EXPECT_EQ('a', s1[4]);
  EXPECT_EQ('r', s1[5]);
  EXPECT_EQ('b', s1[6]);
  EXPECT_EQ('a', s1[7]);
  EXPECT_EQ('z', s1[8]);
  
  s1 = s1 + ' ';
  EXPECT_EQ(10, s1.GetSize());
  EXPECT_EQ('f', s1[0]);
  EXPECT_EQ('o', s1[1]);
  EXPECT_EQ('o', s1[2]);
  EXPECT_EQ('b', s1[3]);
  EXPECT_EQ('a', s1[4]);
  EXPECT_EQ('r', s1[5]);
  EXPECT_EQ('b', s1[6]);
  EXPECT_EQ('a', s1[7]);
  EXPECT_EQ('z', s1[8]);
  EXPECT_EQ(' ', s1[9]);
  
  
  s1 = s1 + "- ";
  EXPECT_EQ(12, s1.GetSize());
  EXPECT_EQ('f', s1[0]);
  EXPECT_EQ('o', s1[1]);
  EXPECT_EQ('o', s1[2]);
  EXPECT_EQ('b', s1[3]);
  EXPECT_EQ('a', s1[4]);
  EXPECT_EQ('r', s1[5]);
  EXPECT_EQ('b', s1[6]);
  EXPECT_EQ('a', s1[7]);
  EXPECT_EQ('z', s1[8]);
  EXPECT_EQ(' ', s1[9]);
  EXPECT_EQ('-', s1[10]);
  EXPECT_EQ(' ', s1[11]);
  
  
  
  s1 = s1 + s1;
  EXPECT_EQ(24, s1.GetSize());
  EXPECT_EQ('f', s1[0]);
  EXPECT_EQ('o', s1[1]);
  EXPECT_EQ('o', s1[2]);
  EXPECT_EQ('b', s1[3]);
  EXPECT_EQ('a', s1[4]);
  EXPECT_EQ('r', s1[5]);
  EXPECT_EQ('b', s1[6]);
  EXPECT_EQ('a', s1[7]);
  EXPECT_EQ('z', s1[8]);
  EXPECT_EQ(' ', s1[9]);
  EXPECT_EQ('-', s1[10]);
  EXPECT_EQ(' ', s1[11]);
  EXPECT_EQ('f', s1[12]);
  EXPECT_EQ('o', s1[13]);
  EXPECT_EQ('o', s1[14]);
  EXPECT_EQ('b', s1[15]);
  EXPECT_EQ('a', s1[16]);
  EXPECT_EQ('r', s1[17]);
  EXPECT_EQ('b', s1[18]);
  EXPECT_EQ('a', s1[19]);
  EXPECT_EQ('z', s1[20]);
  EXPECT_EQ(' ', s1[21]);
  EXPECT_EQ('-', s1[22]);
  EXPECT_EQ(' ', s1[23]);
}


TEST(CoreStringBuffer, Substring) {
  Apto::StringBuffer s1("foobarbaz");
  Apto::StringBuffer s2 = s1.Substring(3, 3);
  EXPECT_EQ(3, s2.GetSize());
  EXPECT_EQ('b', s2[0]);
  EXPECT_EQ('a', s2[1]);
  EXPECT_EQ('r', s2[2]);
}


TEST(CoreStringBuffer, CharacterInstpection) {
  Apto::StringBuffer s1("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 \f\n\r\t\v.");
  for (int i = 0; i < s1.GetSize(); i++) {
    if (i <  26          ) EXPECT_TRUE(s1.IsUpper(i));  else EXPECT_FALSE(s1.IsUpper(i));
    if (i >= 26 && i < 52) EXPECT_TRUE(s1.IsLower(i));  else EXPECT_FALSE(s1.IsLower(i));
    if (i <  52          ) EXPECT_TRUE(s1.IsLetter(i)); else EXPECT_FALSE(s1.IsLetter(i));
    if (i >= 52 && i < 62) EXPECT_TRUE(s1.IsNumber(i)); else EXPECT_FALSE(s1.IsNumber(i));
    if (i >= 62 && i < 68) EXPECT_TRUE(s1.IsWhitespace(i)); else EXPECT_FALSE(s1.IsWhitespace(i));
  }
}
