/*
 *  unittests/core/String.cc
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

#include "gtest/gtest.h"


TEST(CoreBasicString, Construction) {
  // These should all be synonymous
  Apto::BasicString<Apto::SingleThreaded> s1;
  EXPECT_EQ(0, s1.GetSize());
  Apto::BasicString<Apto::SingleThreaded> s2;
  EXPECT_EQ(0, s2.GetSize());
  Apto::String s3;
  EXPECT_EQ(0, s3.GetSize());
  
  // Test Multithreaded
  Apto::BasicString<Apto::ThreadSafe> s4;
  EXPECT_EQ(0, s4.GetSize());


  // Test Construction with strings
  const char* teststr = "Foo Bar Baz";
  Apto::String s5(teststr);
  EXPECT_EQ(11, s5.GetSize());
  Apto::String s6(3, teststr);
  EXPECT_EQ(3, s6.GetSize());
  
  // Test Threading-Type Cross Conversion
  Apto::String s7(s4);
  EXPECT_EQ(0, s7.GetSize());
  Apto::BasicString<Apto::ThreadSafe> s8(s1);
  EXPECT_EQ(0, s8.GetSize());
}


TEST(CoreBasicString, Indexing) {
  Apto::String s1("foo");
  EXPECT_EQ('f', s1[0]);
  EXPECT_EQ('o', s1[1]);
  EXPECT_EQ('o', s1[2]);  
}


TEST(CoreBasicString, Assignment) {
  Apto::String s1("foo");
  EXPECT_EQ('f', s1[0]);
  EXPECT_EQ('o', s1[1]);
  EXPECT_EQ('o', s1[2]);
  Apto::String s2("bar");
  EXPECT_EQ('b', s2[0]);
  EXPECT_EQ('a', s2[1]);
  EXPECT_EQ('r', s2[2]);
  
  Apto::String s3(s1);
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


TEST(CoreBasicString, Iterators) {
  Apto::String s1("abcdef");
  
  Apto::String::Iterator it = s1.Begin();
  for (int i = 0; it.Next(); i++) EXPECT_EQ('a' + i, *it.Get());

  const Apto::String& cs1 = s1;
  Apto::String::ConstIterator cit = cs1.Begin();
  for (int i = 0; cit.Next(); i++) EXPECT_EQ('a' + i, *cit.Get());  
}


TEST(CoreBasicString, Comparison) {
  Apto::String s1("bar");
  Apto::String s2("baz");
  
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


TEST(CoreBasicString, Concatenation) {
  Apto::String s1("foo");
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
  
  s1 += Apto::String("baz");
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


TEST(CoreBasicString, Substring) {
  Apto::String s1("foobarbaz");
  Apto::String s2 = s1.Substring(3, 3);
  EXPECT_EQ(3, s2.GetSize());
  EXPECT_EQ('b', s2[0]);
  EXPECT_EQ('a', s2[1]);
  EXPECT_EQ('r', s2[2]);
}

TEST(CoreBasicString, Pop) {
  Apto::String s1("foo bar,baz");
  Apto::String foo = s1.Pop(' ');
  EXPECT_TRUE(foo == Apto::String("foo"));
  Apto::String bar = s1.Pop(',');
  EXPECT_TRUE(bar == Apto::String("bar"));
  EXPECT_TRUE(s1 == Apto::String("baz"));
}


TEST(CoreBasicString, CharacterInspection) {
  Apto::String s1("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 \f\n\r\t\v.");
  for (int i = 0; i < s1.GetSize(); i++) {
    if (i <  26          ) EXPECT_TRUE(s1.IsUpper(i));  else EXPECT_FALSE(s1.IsUpper(i));
    if (i >= 26 && i < 52) EXPECT_TRUE(s1.IsLower(i));  else EXPECT_FALSE(s1.IsLower(i));
    if (i <  52          ) EXPECT_TRUE(s1.IsLetter(i)); else EXPECT_FALSE(s1.IsLetter(i));
    if (i >= 52 && i < 62) EXPECT_TRUE(s1.IsNumber(i)); else EXPECT_FALSE(s1.IsNumber(i));
    if (i >= 62 && i < 68) EXPECT_TRUE(s1.IsWhitespace(i)); else EXPECT_FALSE(s1.IsWhitespace(i));
  }
}

TEST(CoreBasicString, ToLower) {
  Apto::String s1("ABCDEFG");
  Apto::String s1c(s1);
  
  EXPECT_TRUE(s1 == s1c);
  s1.ToLower();
  EXPECT_FALSE(s1 == s1c);
  EXPECT_TRUE(s1 == "abcdefg");
  EXPECT_TRUE(s1c == "ABCDEFG");
  
  Apto::String s2("ABcdEFG12");
  Apto::String s2c(s2);
  
  EXPECT_TRUE(s2 == s2c);
  s2.ToLower();
  EXPECT_FALSE(s2 == s2c);
  EXPECT_TRUE(s2 == "abcdefg12");
  EXPECT_TRUE(s2c == "ABcdEFG12");
}

TEST(CoreBasicString, AsLower) {
  Apto::String s1("ABCDEFG");
  Apto::String s1c(s1);
  
  EXPECT_TRUE(s1 == s1c);
  Apto::String s1a = s1.AsLower();
  EXPECT_TRUE(s1 == s1c);
  EXPECT_FALSE(s1 == s1a);
  EXPECT_TRUE(s1a == "abcdefg");
  EXPECT_TRUE(s1 == "ABCDEFG");
  EXPECT_TRUE(s1c == "ABCDEFG");
  
  Apto::String s2("ABcdEFG12");
  Apto::String s2c(s2);
  
  EXPECT_TRUE(s2 == s2c);
  Apto::String s2a = s2.AsLower();
  EXPECT_TRUE(s2 == s2c);
  EXPECT_FALSE(s2 == s2a);
  EXPECT_TRUE(s2a == "abcdefg12");
  EXPECT_TRUE(s2 == "ABcdEFG12");
  EXPECT_TRUE(s2c == "ABcdEFG12");
}

TEST(CoreBasicString, ToUpper) {
  Apto::String s1("abcdefg");
  Apto::String s1c(s1);
  
  EXPECT_TRUE(s1 == s1c);
  s1.ToUpper();
  EXPECT_FALSE(s1 == s1c);
  EXPECT_TRUE(s1 == "ABCDEFG");
  EXPECT_TRUE(s1c == "abcdefg");
  
  Apto::String s2("ABcdEFG12");
  Apto::String s2c(s2);
  
  EXPECT_TRUE(s2 == s2c);
  s2.ToUpper();
  EXPECT_FALSE(s2 == s2c);
  EXPECT_TRUE(s2 == "ABCDEFG12");
  EXPECT_TRUE(s2c == "ABcdEFG12");
}

TEST(CoreBasicString, AsUpper) {
  Apto::String s1("abcdefg");
  Apto::String s1c(s1);
  
  EXPECT_TRUE(s1 == s1c);
  Apto::String s1a = s1.AsUpper();
  EXPECT_TRUE(s1 == s1c);
  EXPECT_FALSE(s1 == s1a);
  EXPECT_TRUE(s1a == "ABCDEFG");
  EXPECT_TRUE(s1 == "abcdefg");
  EXPECT_TRUE(s1c == "abcdefg");
  
  Apto::String s2("ABcdEFG12");
  Apto::String s2c(s2);
  
  EXPECT_TRUE(s2 == s2c);
  Apto::String s2a = s2.AsUpper();
  EXPECT_TRUE(s2 == s2c);
  EXPECT_FALSE(s2 == s2a);
  EXPECT_TRUE(s2a == "ABCDEFG12");
  EXPECT_TRUE(s2 == "ABcdEFG12");
  EXPECT_TRUE(s2c == "ABcdEFG12");
}

TEST(CoreBasicString, Trim) {
  Apto::String s1("  foo  ");
  Apto::String s1c(s1);
  
  EXPECT_TRUE(s1 == s1c);
  s1.Trim();
  EXPECT_FALSE(s1 == s1c);
  EXPECT_TRUE(s1 == "foo");
  EXPECT_TRUE(s1c == "  foo  ");
  
  Apto::String s2("bar");
  EXPECT_TRUE(s2 == "bar");
  s2.Trim();
  EXPECT_TRUE(s2 == "bar");
}

TEST(CoreBasicString, Trimmed) {
  Apto::String s1("  foo  ");
  Apto::String s1c(s1);
  
  EXPECT_TRUE(s1 == s1c);
  Apto::String s1t = s1.Trimmed();
  EXPECT_TRUE(s1 == s1c);
  EXPECT_FALSE(s1 == s1t);
  EXPECT_TRUE(s1t == "foo");
  EXPECT_TRUE(s1 == "  foo  ");
  EXPECT_TRUE(s1c == "  foo  ");
  
  Apto::String s2("bar");
  EXPECT_TRUE(s2 == "bar");
  Apto::String s2t = s2.Trimmed();
  EXPECT_TRUE(s2t == "bar");
}

TEST(CoreBasicString, Find) {
  Apto::String s1("the quick brown fox jumped over the lazy dog");
  
  EXPECT_EQ(s1.Find('q'), 4);
  EXPECT_EQ(s1.Find('q', 5), -1);
}

TEST(CoreBasicString, FindStr) {
  Apto::String s1("the quick brown fox jumped over the lazy dog");
  Apto::String s2("quick");
  
  EXPECT_EQ(s1.Find("quick"), 4);
  EXPECT_EQ(s1.Find("quick", 5), -1);
  EXPECT_EQ(s1.Find(s2), 4);
  EXPECT_EQ(s1.Find(s2, 5), -1);
}

