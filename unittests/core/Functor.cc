/*
 *  unittests/core/Functor.cc
 *  Apto
 *
 *  Created by David on 5/12/11.
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

#include "apto/core/Functor.h"

#include "gtest/gtest.h"

static void VoidFunction() { ; }

static int Add1(int p1)
{
  return p1;
}

static int Add2(int p1, int p2)
{
  return p1 + p2;
}

static int Add3(int p1, int p2, int p3)
{
  return p1 + p2 + p3;
}

static int Add4(int p1, int p2, int p3, int p4)
{
  return p1 + p2 + p3 + p4;
}

static int Add5(int p1, int p2, int p3, int p4, int p5)
{
  return p1 + p2 + p3 + p4 + p5;
}

static int Add6(int p1, int p2, int p3, int p4, int p5, int p6)
{
  return p1 + p2 + p3 + p4 + p5 + p6;
}

static int Add7(int p1, int p2, int p3, int p4, int p5, int p6, int p7)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7;
}

static int Add8(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;
}

static int Add9(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
}

static int Add10(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
}

static int Add11(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11;
}

static int Add12(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12;
}

static int Add13(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13;
}

static int Add14(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13,
                 int p14)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13 + p14;
}

static int Add15(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13,
                 int p14, int p15)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13 + p14 + p15;
}

static int Add16(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13,
                 int p14, int p15, int p16)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13 + p14 + p15 + p16;
}

class FunctorTest {
public:
  void VoidFunction() { ; }
  
  int Add1(int p1)
  {
    return p1;
  }
  
  int Add2(int p1, int p2)
  {
    return p1 + p2;
  }
  
  int Add3(int p1, int p2, int p3)
  {
    return p1 + p2 + p3;
  }
  
  int Add4(int p1, int p2, int p3, int p4)
  {
    return p1 + p2 + p3 + p4;
  }
  
  int Add5(int p1, int p2, int p3, int p4, int p5)
  {
    return p1 + p2 + p3 + p4 + p5;
  }
  
  int Add6(int p1, int p2, int p3, int p4, int p5, int p6)
  {
    return p1 + p2 + p3 + p4 + p5 + p6;
  }
  
  int Add7(int p1, int p2, int p3, int p4, int p5, int p6, int p7)
  {
    return p1 + p2 + p3 + p4 + p5 + p6 + p7;
  }
  
  int Add8(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8)
  {
    return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;
  }
  
  int Add9(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9)
  {
    return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
  }
  
  int Add10(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10)
  {
    return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
  }
  
  int Add11(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11)
  {
    return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11;
  }
  
  int Add12(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12)
  {
    return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12;
  }
  
  int Add13(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13)
  {
    return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13;
  }
  
  int Add14(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13,
            int p14)
  {
    return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13 + p14;
  }
  
  int Add15(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13,
            int p14, int p15)
  {
    return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13 + p14 + p15;
  }
  
  int Add16(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13,
            int p14, int p15, int p16)
  {
    return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13 + p14 + p15 + p16;
  }
};

class IntW
{
private:
  int m_value;
public:
  IntW(int value) : m_value(value) { ; }
  operator int() { return m_value; }
};


static int Add1W(IntW p1)
{
  return p1;
}

static int Add2W(IntW p1, IntW p2)
{
  return p1 + p2;
}

static int Add3W(IntW p1, IntW p2, IntW p3)
{
  return p1 + p2 + p3;
}

static int Add4W(IntW p1, IntW p2, IntW p3, IntW p4)
{
  return p1 + p2 + p3 + p4;
}

static int Add5W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5)
{
  return p1 + p2 + p3 + p4 + p5;
}

static int Add6W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5, IntW p6)
{
  return p1 + p2 + p3 + p4 + p5 + p6;
}

static int Add7W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5, IntW p6, IntW p7)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7;
}

static int Add8W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5, IntW p6, IntW p7, IntW p8)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;
}

static int Add9W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5, IntW p6, IntW p7, IntW p8, IntW p9)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
}

static int Add10W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5, IntW p6, IntW p7, IntW p8, IntW p9, IntW p10)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
}

static int Add11W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5, IntW p6, IntW p7, IntW p8, IntW p9, IntW p10, IntW p11)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11;
}

static int Add12W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5, IntW p6, IntW p7, IntW p8, IntW p9, IntW p10, IntW p11,
                 IntW p12)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12;
}

static int Add13W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5, IntW p6, IntW p7, IntW p8, IntW p9, IntW p10, IntW p11,
                 IntW p12, IntW p13)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13;
}

static int Add14W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5, IntW p6, IntW p7, IntW p8, IntW p9, IntW p10, IntW p11,
                 IntW p12, IntW p13, IntW p14)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13 + p14;
}

static int Add15W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5, IntW p6, IntW p7, IntW p8, IntW p9, IntW p10, IntW p11,
                 IntW p12, IntW p13, IntW p14, IntW p15)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13 + p14 + p15;
}

static int Add16W(IntW p1, IntW p2, IntW p3, IntW p4, IntW p5, IntW p6, IntW p7, IntW p8, IntW p9, IntW p10, IntW p11,
                 IntW p12, IntW p13, IntW p14, IntW p15, IntW p16)
{
  return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12 + p13 + p14 + p15 + p16;
}


TEST(CoreFunctor, BasicFunctor) {
  Apto::Functor<> t1(VoidFunction);
  t1();
  Apto::Functor<void, Apto::NullType> t2(VoidFunction);
  t2();
  t1 = t2;
  t1();
  
  Apto::Functor<int, Apto::TL::Create<int> > a1(Add1);
  EXPECT_EQ(1, a1(1));
  
  Apto::Functor<int, Apto::TL::Create<int, int> > a2(Add2);
  EXPECT_EQ(3, a2(1, 2));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int> > a3(Add3);
  EXPECT_EQ(6, a3(1, 2, 3));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int> > a4(Add4);
  EXPECT_EQ(10, a4(1, 2, 3, 4));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int> > a5(Add5);
  EXPECT_EQ(15, a5(1, 2, 3, 4, 5));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int> > a6(Add6);
  EXPECT_EQ(21, a6(1, 2, 3, 4, 5, 6));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int> > a7(Add7);
  EXPECT_EQ(28, a7(1, 2, 3, 4, 5, 6, 7));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int> > a8(Add8);
  EXPECT_EQ(36, a8(1, 2, 3, 4, 5, 6, 7, 8));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int> > a9(Add9);
  EXPECT_EQ(45, a9(1, 2, 3, 4, 5, 6, 7, 8, 9));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int> > a10(Add10);
  EXPECT_EQ(55, a10(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int> > a11(Add11);
  EXPECT_EQ(66, a11(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int, int> > a12(Add12);
  EXPECT_EQ(78, a12(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int, int, int> > a13(Add13);
  EXPECT_EQ(91, a13(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int, int, int, int> > a14(Add14);
  EXPECT_EQ(105, a14(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int> > a15(Add15);
  EXPECT_EQ(120, a15(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int> > a16(Add16);
  EXPECT_EQ(136, a16(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
}

TEST(CoreFunctor, MemberFunctionFunctor) {
  Apto::SmartPtr<FunctorTest> obj(new FunctorTest);
  
  Apto::Functor<> t1(obj, &FunctorTest::VoidFunction);
  t1();
  Apto::Functor<void, Apto::NullType> t2(obj, &FunctorTest::VoidFunction);
  t2();
  t1 = t2;
  t1();
  
  Apto::Functor<int, Apto::TL::Create<int> > a1(obj, &FunctorTest::Add1);
  EXPECT_EQ(1, a1(1));
  
  Apto::Functor<int, Apto::TL::Create<int, int> > a2(obj, &FunctorTest::Add2);
  EXPECT_EQ(3, a2(1, 2));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int> > a3(obj, &FunctorTest::Add3);
  EXPECT_EQ(6, a3(1, 2, 3));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int> > a4(obj, &FunctorTest::Add4);
  EXPECT_EQ(10, a4(1, 2, 3, 4));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int> > a5(obj, &FunctorTest::Add5);
  EXPECT_EQ(15, a5(1, 2, 3, 4, 5));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int> > a6(obj, &FunctorTest::Add6);
  EXPECT_EQ(21, a6(1, 2, 3, 4, 5, 6));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int> > a7(obj, &FunctorTest::Add7);
  EXPECT_EQ(28, a7(1, 2, 3, 4, 5, 6, 7));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int> > a8(obj, &FunctorTest::Add8);
  EXPECT_EQ(36, a8(1, 2, 3, 4, 5, 6, 7, 8));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int> > a9(obj, &FunctorTest::Add9);
  EXPECT_EQ(45, a9(1, 2, 3, 4, 5, 6, 7, 8, 9));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int> > a10(obj, &FunctorTest::Add10);
  EXPECT_EQ(55, a10(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int> > a11(obj, &FunctorTest::Add11);
  EXPECT_EQ(66, a11(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int, int> > a12(obj, &FunctorTest::Add12);
  EXPECT_EQ(78, a12(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int, int, int> > a13(obj, &FunctorTest::Add13);
  EXPECT_EQ(91, a13(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int, int, int, int> > a14(obj, &FunctorTest::Add14);
  EXPECT_EQ(105, a14(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int> > a15(obj, &FunctorTest::Add15);
  EXPECT_EQ(120, a15(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int> > a16(obj, &FunctorTest::Add16);
  EXPECT_EQ(136, a16(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
}

TEST(CoreFunctor, NonTrivialArguments) {
  Apto::Functor<int, Apto::TL::Create<IntW> > a1(Add1W);
  EXPECT_EQ(1, a1(1));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW> > a2(Add2W);
  EXPECT_EQ(3, a2(1, 2));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW> > a3(Add3W);
  EXPECT_EQ(6, a3(1, 2, 3));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW> > a4(Add4W);
  EXPECT_EQ(10, a4(1, 2, 3, 4));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW> > a5(Add5W);
  EXPECT_EQ(15, a5(1, 2, 3, 4, 5));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW, IntW> > a6(Add6W);
  EXPECT_EQ(21, a6(1, 2, 3, 4, 5, 6));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW, IntW, IntW> > a7(Add7W);
  EXPECT_EQ(28, a7(1, 2, 3, 4, 5, 6, 7));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW> > a8(Add8W);
  EXPECT_EQ(36, a8(1, 2, 3, 4, 5, 6, 7, 8));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW> > a9(Add9W);
  EXPECT_EQ(45, a9(1, 2, 3, 4, 5, 6, 7, 8, 9));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW> > a10(Add10W);
  EXPECT_EQ(55, a10(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW> > a11(Add11W);
  EXPECT_EQ(66, a11(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW> > a12(Add12W);
  EXPECT_EQ(78, a12(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW> > a13(Add13W);
  EXPECT_EQ(91, a13(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW> > a14(Add14W);
  EXPECT_EQ(105, a14(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW> > a15(Add15W);
  EXPECT_EQ(120, a15(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
  
  Apto::Functor<int, Apto::TL::Create<IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW, IntW> > a16(Add16W);
  EXPECT_EQ(136, a16(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
}


TEST(CoreFunctor, BindFirst) {
  Apto::Functor<int, Apto::TL::Create<int> > a1(Add1);
  Apto::Functor<int, Apto::NullType> a1b = Apto::BindFirst(a1, 1);
  EXPECT_EQ(1, a1(1));
  EXPECT_EQ(1, a1b());
  
  Apto::Functor<int, Apto::TL::Create<int, int> > a2(Add2);
  Apto::Functor<int, Apto::TL::Create<int> > a2b = Apto::BindFirst(a2, 1);
  EXPECT_EQ(3, a2(1, 2));
  EXPECT_EQ(3, a2b(2));
  
  Apto::Functor<int, Apto::TL::Create<int, int, int> > a3(Add3);
  Apto::Functor<int, Apto::TL::Create<int, int> > a3b = Apto::BindFirst(a3, 1);
  EXPECT_EQ(6, a3(1, 2, 3));
  EXPECT_EQ(6, a3b(2, 3));
  
  a3b = a2;
  EXPECT_EQ(3, a3b(1, 2));
  
  a2 = Apto::BindFirst(a3, 1);
  EXPECT_EQ(6, a2(2, 3));
  
  a1 = Apto::BindFirst(Apto::BindFirst(a3, 1), 2);
  EXPECT_EQ(6, a1(3));
  
  a1b = Apto::BindFirst(a1, 3);
  EXPECT_EQ(6, a1b());
}
