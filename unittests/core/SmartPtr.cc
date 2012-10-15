/*
 *  unittests/core/SmartPtr.cc
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
 *  SmartPtr unit test cases derived from Loki SmartPtr tests:
 *
 *    Copyright (c) 2006 Richard Sposato
 *    Permission to use, copy, modify, distribute and sell this software for any 
 *        purpose is hereby granted without fee, provided that the above copyright 
 *        notice appear in all copies and that both that copyright notice and this 
 *        permission notice appear in supporting documentation.
 *
 */

#include "apto/core/RefCount.h"
#include "apto/core/SmartPtr.h"

#include "gtest/gtest.h"


// Test Classes
// --------------------------------------------------------------------------------------------------------------

class BaseClass
{
private:
  static int s_constructions;
  static int s_destructions;
  
public:
  BaseClass() { s_constructions++; }
  virtual ~BaseClass() { s_destructions++; }
  
  // These 2 functions let BaseClass impersonate a RefCountObject
  void AddReference() { ; }
  void RemoveReference() { ; }
  
  // This function is used only for the DeepCopy policy.
  virtual BaseClass* Clone() const { return new BaseClass(); }
  
  void DoThat() const { ; }
  
  static inline bool AllDestroyed() { return (s_constructions == s_destructions); }
  static inline int GetConstructorCount() { return s_constructions; }  
  static inline int GetDestructorCount() { return s_destructions; }
  
private:
  BaseClass(const BaseClass&); // @not_implemented
  BaseClass& operator=(const BaseClass&); // @not_implemented
};


class PublicSubClass : public BaseClass
{
public:
  // This function is used only for the DeepCopy policy.
  virtual BaseClass* Clone() const { return new PublicSubClass; }
};


class PrivateSubClass : private BaseClass
{
public:
  // This function is used only for the DeepCopy policy.
  virtual BaseClass* Clone() const { return new PrivateSubClass; }
};


class RCBaseClass : public Apto::RefCountObject<Apto::ThreadSafe>
{
private:
  static int s_constructions;
  static int s_destructions;
  
public:
  RCBaseClass() { s_constructions++; }
  virtual ~RCBaseClass() { s_destructions++; }
  
  // This function is used only for the DeepCopy policy.
  virtual RCBaseClass* Clone() const { return new RCBaseClass(); }
  
  void DoThat() const { ; }
  
  static inline bool AllDestroyed() { return (s_constructions == s_destructions); }  
  static inline int GetConstructorCount() { return s_constructions; }  
  static inline int GetDestructorCount() { return s_destructions; }
  
private:
  RCBaseClass(const RCBaseClass&); // @not_implemented
  RCBaseClass& operator=(const RCBaseClass&); // @not_implemented
};


class Feline
{
public:
  virtual ~Feline() {; }
};

class Lion : public Feline
{
public:
  virtual ~Lion() { ; }
};

class Tiger : public Feline
{
public:
  virtual ~Tiger() { ; }
};

class Dog
{
public:
  virtual ~Dog() { ; }
};



// Define Test Class Statics
// --------------------------------------------------------------------------------------------------------------

int BaseClass::s_constructions = 0;
int BaseClass::s_destructions = 0;

int RCBaseClass::s_constructions = 0;
int RCBaseClass::s_destructions = 0;


// Check that SmartPtr can be used with a forward-reference
// --------------------------------------------------------------------------------------------------------------

class Thingy;

typedef Apto::SmartPtr<Thingy, Apto::RefCount, Apto::HeapStorage> Thingy_RefCount_ptr;


// Declare Types for Testing
// --------------------------------------------------------------------------------------------------------------

// Used for testing most policies
typedef Apto::SmartPtr<BaseClass, Apto::RefCount, Apto::HeapStorage> Base_RefCount_ptr;
typedef Apto::SmartPtr<const BaseClass, Apto::RefCount, Apto::HeapStorage> ConstBase_RefCount_ptr;

// Used for testing ownership policies
typedef Apto::SmartPtr<BaseClass, Apto::ThreadSafeRefCount, Apto::HeapStorage> Base_ThreadSafeRefCount_ptr;
typedef Apto::SmartPtr<BaseClass, Apto::InternalRCObject, Apto::HeapStorage> Base_InternalRCObject_ptr;
typedef Apto::SmartPtr<BaseClass, Apto::DeepCopy, Apto::HeapStorage> Base_DeepCopy_ptr;
typedef Apto::SmartPtr<BaseClass, Apto::DestructiveCopy, Apto::HeapStorage> Base_DestructiveCopy_ptr;
typedef Apto::SmartPtr<const BaseClass, Apto::DestructiveCopy, Apto::HeapStorage> ConstBase_DestructiveCopy_ptr;
typedef Apto::SmartPtr<BaseClass, Apto::NoCopy, Apto::HeapStorage> Base_NoCopy_ptr;

// Used for testing inheritance
typedef Apto::SmartPtr<PublicSubClass, Apto::RefCount, Apto::HeapStorage> PublicSub_RefCount_ptr;
typedef Apto::SmartPtr<PrivateSubClass, Apto::RefCount, Apto::HeapStorage> PrivateSub_RefCount_ptr;

// Used for testing internal ref count ptrs
typedef Apto::SmartPtr<RCBaseClass, Apto::InternalRCObject, Apto::HeapStorage> RCBase_InternalRCObject_ptr;


// Test Cases
// --------------------------------------------------------------------------------------------------------------

TEST(CoreSmartPtr, ConstConversion) {
  Base_RefCount_ptr p1;
  ConstBase_RefCount_ptr p2(p1);
  
//  p1 = p2;  // illegal! converts const to non-const.
  p2 = p1;  // legal.  natural const conversion.

  EXPECT_TRUE(BaseClass::AllDestroyed());
}


TEST(CoreSmartPtr, OwnershipConversion) {
  Base_RefCount_ptr p1;
  Base_ThreadSafeRefCount_ptr p2;
  Base_InternalRCObject_ptr p3;
  Base_DeepCopy_ptr p4(new BaseClass);
  Base_DestructiveCopy_ptr p5;
  Base_NoCopy_ptr p6;

  // Legal Constructions
  Base_RefCount_ptr p1_1(p1);
  Base_ThreadSafeRefCount_ptr p2_2(p2);
  Base_InternalRCObject_ptr p3_3(p3);
  Base_DeepCopy_ptr p4_4(p4);
  Base_DestructiveCopy_ptr p5_5(p5);
  
  // Illegal Constructions -- Can't copy anything with the NoCopy policy
//  Base_NoCopy_ptr p6_6(p6);

  // Illegal Constructions -- Can't convert from one policy to another
//  Base_RefCount_ptr p2_1(p2);
//  Base_RefCount_ptr p3_1(p3);
//  Base_RefCount_ptr p4_1(p4);
//  Base_RefCount_ptr p5_1(p5);
//  Base_RefCount_ptr p6_1(p6);
//
//  Base_ThreadSafeRefCount_ptr p1_2(p1);
//  Base_ThreadSafeRefCount_ptr p3_2(p3);
//  Base_ThreadSafeRefCount_ptr p4_2(p4);
//  Base_ThreadSafeRefCount_ptr p5_2(p5);
//  Base_ThreadSafeRefCount_ptr p6_2(p6);
//  
//  Base_InternalRCObject_ptr p1_3(p1);
//  Base_InternalRCObject_ptr p2_3(p2);
//  Base_InternalRCObject_ptr p4_3(p4);
//  Base_InternalRCObject_ptr p5_3(p5);
//  Base_InternalRCObject_ptr p6_3(p6);
//  
//  Base_DeepCopy_ptr p1_4(p1);
//  Base_DeepCopy_ptr p2_4(p2);
//  Base_DeepCopy_ptr p3_4(p3);
//  Base_DeepCopy_ptr p5_4(p5);
//  Base_DeepCopy_ptr p6_4(p6);
//  
//  Base_DestructiveCopy_ptr p1_5(p1);
//  Base_DestructiveCopy_ptr p2_5(p2);
//  Base_DestructiveCopy_ptr p3_5(p3);
//  Base_DestructiveCopy_ptr p4_5(p4);
//  Base_DestructiveCopy_ptr p6_5(p6);
//  
//  Base_NoCopy_ptr p1_6(p1);
//  Base_NoCopy_ptr p2_6(p2);
//  Base_NoCopy_ptr p3_6(p3);
//  Base_NoCopy_ptr p4_6(p4);
//  Base_NoCopy_ptr p5_6(p5);
  
  // Legal Constructions -- Preserves Constness
  ConstBase_DestructiveCopy_ptr p5_c(p5);
  ConstBase_DestructiveCopy_ptr p5_c1(p5_c);
  
  // Illegal Constructions -- Don't preserve constness
  const BaseClass* raw_p = new BaseClass;
//  Base_DestructiveCopy_ptr p5_nc(raw_p);
//  Base_DestructiveCopy_ptr p5_nc1(p5_c);
  ConstBase_DestructiveCopy_ptr p5_c_r(raw_p);

  Base_DestructiveCopy_ptr p5_a;
  // Legal Assignment - preserves constness
  p5_a = p5;
  // Illegal Assignment - doesn't preserve constness
//  p5_a = p5_c;
  
  // Illegal Assignments - can't convert from one ownership policy to another
//  p1 = p2;
//  p1 = p3;
//  p1 = p4;
//  p1 = p5;
//  p1 = p6;
//  
//  p2 = p1;
//  p2 = p3;
//  p2 = p4;
//  p2 = p5;
//  p2 = p6;
//  
//  p3 = p1;
//  p3 = p2;
//  p3 = p4;
//  p3 = p5;
//  p3 = p6;
//  
//  p4 = p1;
//  p4 = p2;
//  p4 = p3;
//  p4 = p5;
//  p4 = p6;
//  
//  p5 = p1;
//  p5 = p2;
//  p5 = p3;
//  p5 = p4;
//  p5 = p6;
//  
//  p6 = p1;
//  p6 = p2;
//  p6 = p3;
//  p6 = p4;
//  p6 = p5;
}


TEST(CoreSmartPtr, Inheritance) {
  Base_RefCount_ptr p1;
  PublicSub_RefCount_ptr p2(new PublicSubClass());
  PrivateSub_RefCount_ptr p3(new PrivateSubClass());
  
  p1 = p2;
  EXPECT_TRUE(p1 == p2);
//  p1 = p3;  // Illegal -- Can't assign pointer since base class is private.
//  p2 = p1;  // Illegal -- Can't do cast to derived class in pointer assignment.
//  p2 = p3;  // Illegal -- Can't assign when types are unrelated.
//  p3 = p1;  // Illegal -- Can't do cast to derived class in pointer assignment.
//  p3 = p2;  // Illegal -- Can't assign when types are unrelated.
  
  Base_RefCount_ptr p4(p2);
  EXPECT_TRUE(p4 == p1);
  EXPECT_TRUE(p4 == p2);
  
  // Illegal Copy-Constructions -- see reasons above
//  Base_RefCount_ptr p5(p3);
//  PublicSub_RefCount_ptr p6(p1);
//  PublicSub_RefCount_ptr p7(p3);
//  PrivateSub_RefCount_ptr p8(p1);
//  PrivateSub_RefCount_ptr p9(p2);
}

TEST(CoreSmartPtr, RefCountSwap) {
  Base_RefCount_ptr p1(new BaseClass);
  Base_RefCount_ptr p2(new BaseClass);
  
  Base_RefCount_ptr p3(p1);
  Base_RefCount_ptr p4(p2);
  
  // p1 == p3  and  p2 == p4
  EXPECT_TRUE(p1 == p3);
  EXPECT_TRUE(p1 != p2);
  EXPECT_TRUE(p1 != p4);
  EXPECT_TRUE(p2 == p4);
  EXPECT_TRUE(p2 != p3);
  EXPECT_TRUE(p2 != p1);
  
  // p1 == p4  and  p2 == p3
  p3.Swap(p4);
  EXPECT_TRUE(p1 == p4);
  EXPECT_TRUE(p1 != p2);
  EXPECT_TRUE(p1 != p3);
  EXPECT_TRUE(p2 == p3);
  EXPECT_TRUE(p2 != p4);
  EXPECT_TRUE(p2 != p1);

  // p1 == p3  and  p2 == p4
  p3.Swap(p4);
  EXPECT_TRUE(p1 == p3);
  EXPECT_TRUE(p1 != p2);
  EXPECT_TRUE(p1 != p4);
  EXPECT_TRUE(p2 == p4);
  EXPECT_TRUE(p2 != p3);
  EXPECT_TRUE(p2 != p1);

  // p2 == p3  and  p1 == p4
  p3.Swap(p4);
  EXPECT_TRUE(p1 == p4);
  EXPECT_TRUE(p1 != p2);
  EXPECT_TRUE(p1 != p3);
  EXPECT_TRUE(p2 == p3);
  EXPECT_TRUE(p2 != p4);
  EXPECT_TRUE(p2 != p1);

  // p2 == p3  and  p1 == p4
  p1.Swap(p1);
  EXPECT_TRUE(p1 == p4);
  EXPECT_TRUE(p1 != p2);
  EXPECT_TRUE(p1 != p3);
  EXPECT_TRUE(p2 == p3);
  EXPECT_TRUE(p2 != p4);
  EXPECT_TRUE(p2 != p1);
  
  // p2 == p3  and  p4 == p1
  p1.Swap(p4);
  EXPECT_TRUE(p1 == p4);
  EXPECT_TRUE(p1 != p2);
  EXPECT_TRUE(p1 != p3);
  EXPECT_TRUE(p2 == p3);
  EXPECT_TRUE(p2 != p4);
  EXPECT_TRUE(p2 != p1);
}


TEST(CoreSmartPtr, RefCountNullPointer) {
  BaseClass* pNull = NULL;
  const int constructor_count = BaseClass::GetConstructorCount();
  const int destructor_count = BaseClass::GetDestructorCount();
  EXPECT_EQ(constructor_count, destructor_count);
  
  {
    Base_RefCount_ptr p0;
    Base_RefCount_ptr p1;
    Base_RefCount_ptr p2(p0);
    
    EXPECT_TRUE(!p0);
    EXPECT_TRUE(!p1);
    EXPECT_TRUE(!p2);
    
    EXPECT_TRUE(p0 == pNull);
    EXPECT_TRUE(p1 == pNull);
    EXPECT_TRUE(p2 == pNull);
    
    EXPECT_TRUE(pNull == p0);
    EXPECT_TRUE(pNull == p1);
    EXPECT_TRUE(pNull == p2);
    
    EXPECT_TRUE(p0 == p0);
    EXPECT_TRUE(p1 == p1);
    EXPECT_TRUE(p2 == p2);
    
    EXPECT_TRUE(p0 == p1);
    EXPECT_TRUE(p0 == p2);
    EXPECT_TRUE(p1 == p0);
    EXPECT_TRUE(p2 == p0);

    EXPECT_TRUE(p0 <= p1);
    EXPECT_TRUE(p0 <= p2);
    EXPECT_TRUE(p1 <= p0);
    EXPECT_TRUE(p2 <= p0);
    
    EXPECT_TRUE(p0 >= p1);
    EXPECT_TRUE(p0 >= p2);
    EXPECT_TRUE(p1 >= p0);
    EXPECT_TRUE(p2 >= p0);
    
    EXPECT_FALSE(p0 < p1);
    EXPECT_FALSE(p0 < p2);
    EXPECT_FALSE(p1 < p0);
    EXPECT_FALSE(p2 < p0);

    EXPECT_FALSE(p0 > p1);
    EXPECT_FALSE(p0 > p2);
    EXPECT_FALSE(p1 > p0);
    EXPECT_FALSE(p2 > p0);
    
    EXPECT_FALSE(p0 < pNull);
    EXPECT_FALSE(p0 > pNull);
    EXPECT_FALSE(pNull < p0);
    EXPECT_FALSE(pNull > p0);
  }
  
  EXPECT_EQ(constructor_count, BaseClass::GetConstructorCount());
  EXPECT_EQ(destructor_count, BaseClass::GetDestructorCount());

  EXPECT_TRUE(BaseClass::AllDestroyed());
}


TEST(CoreSmartPtr, InternalRCObject) {
  const int constructor_count = RCBaseClass::GetConstructorCount();
  const int destructor_count = RCBaseClass::GetDestructorCount();
  EXPECT_TRUE(RCBaseClass::AllDestroyed());

  {
    RCBase_InternalRCObject_ptr p1;
  }
  EXPECT_TRUE(RCBaseClass::AllDestroyed());
  EXPECT_EQ(constructor_count, RCBaseClass::GetConstructorCount());
  EXPECT_EQ(destructor_count, RCBaseClass::GetDestructorCount());

  {
    RCBase_InternalRCObject_ptr p1(new RCBaseClass);
  }
  EXPECT_EQ(constructor_count + 1, RCBaseClass::GetConstructorCount());
  EXPECT_EQ(destructor_count + 1, RCBaseClass::GetDestructorCount());
  
  {
    RCBase_InternalRCObject_ptr p1(new RCBaseClass);
    RCBase_InternalRCObject_ptr P2(p1);
    RCBase_InternalRCObject_ptr p3;
    p3 = p1;
  }
  EXPECT_EQ(constructor_count + 2, RCBaseClass::GetConstructorCount());
  EXPECT_EQ(destructor_count + 2, RCBaseClass::GetDestructorCount());
  
  EXPECT_TRUE(RCBaseClass::AllDestroyed());
}


TEST(CoreSmartPtr, ForwardReference) {
  // These should generate an error message about attempting to call the destructor for an undefined type
//  Thingy_RefCount_ptr p1;
//  Thingy_RefCount_ptr p2(p1);
//  Thingy_RefCount_ptr p3;
//  p3 = p2;
}

TEST(CoreSmartPtr, DynamicCast) {
  typedef Apto::SmartPtr<Feline> FelinePtr;
  typedef Apto::SmartPtr<Lion> LionPtr;
  typedef Apto::SmartPtr<Tiger> TigerPtr;
  typedef Apto::SmartPtr<Dog> DogPtr;
  
  Feline* feline = new Lion;
  Lion* lion = new Lion;
  Tiger* tiger = new Tiger;
  Dog* dog = new Dog;
  
  FelinePtr pFeline(feline);
  LionPtr pLion(lion);
  TigerPtr pTiger(tiger);
  DogPtr pDog(dog);
  
  // Legal downcast
  pFeline = pLion;
  FelinePtr pFeline2(pLion);
  
  // Illegal upcast
  //pLion = pFeline;
    
  // Can dynamic up-cast from feline to lion
  EXPECT_TRUE(pFeline);
  pLion.DynamicCastFrom(pFeline);
  EXPECT_TRUE(pLion);
  EXPECT_TRUE(pLion == pFeline);
  
  // Can't cast from lion to tiger
  pTiger.DynamicCastFrom(pLion);
  EXPECT_TRUE(!pTiger);
  
  // Can't cast from dog to lion
  pLion.DynamicCastFrom(pDog);
  EXPECT_TRUE(!pLion);
  
  pLion.DynamicCastFrom(pFeline);
  EXPECT_TRUE(pLion);
  EXPECT_TRUE(pLion == pFeline);
  
  // Can't cast from lion to dog
  pDog.DynamicCastFrom(pLion);
  EXPECT_TRUE(!pDog);
}

