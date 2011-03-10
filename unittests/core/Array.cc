/*
 *  unittests/core/Array.cc
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

#include "apto/core/Array.h"

#include "gtest/gtest.h"


// Array<int, Basic>
// --------------------------------------------------------------------------------------------------------------  

TEST(CoreBasicArray, Construction) {
  Apto::Array<int, Apto::Basic> default_constructor;
  EXPECT_EQ(0, default_constructor.GetSize());
  Apto::Array<int, Apto::Basic> constructor_sz_3(3);
  EXPECT_EQ(3, constructor_sz_3.GetSize());
}

TEST(CoreBasicArray, Indexing) {
  Apto::Array<int, Apto::Basic> array(5);
  for (int i = 0; i < array.GetSize(); i++) array[i] = i;
  
  EXPECT_EQ(0, array[0]);
  EXPECT_EQ(1, array[1]);
  EXPECT_EQ(2, array[2]);
  EXPECT_EQ(3, array[3]);
  EXPECT_EQ(4, array[4]);
  
  array[3] = 12;
  
  EXPECT_EQ(12, array[3]);
}

TEST(CoreBasicArray, Assignment) {
  Apto::Array<int, Apto::Basic> array1(5);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  
  Apto::Array<int, Apto::Basic> array2(6);
  for (int i = 0; i < array2.GetSize(); i++) array2[i] = 5 + i;
  
  EXPECT_NE(array1.GetSize(), array2.GetSize());
  EXPECT_NE(array1[3], array2[3]);
  
  array1 = array2;
  
  EXPECT_EQ(array1.GetSize(), array2.GetSize());
  EXPECT_EQ(array1[3], array2[3]);
  
  Apto::Array<int, Apto::Basic> array_copy_constructor(array2);
  EXPECT_EQ(array2.GetSize(), array_copy_constructor.GetSize());
  EXPECT_EQ(array2[3], array_copy_constructor[3]);
}

TEST(CoreBasicArray, Resize) {
  Apto::Array<int, Apto::Basic> array1(5);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  EXPECT_EQ(5, array1.GetSize());
  EXPECT_EQ(4, array1[4]);
  
  array1.Resize(20);
  EXPECT_EQ(20, array1.GetSize());
  EXPECT_EQ(4, array1[4]);
  
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = 10 + i;
  EXPECT_EQ(14, array1[4]);
  EXPECT_EQ(19, array1[9]);
  
  array1.Resize(5);
  EXPECT_EQ(14, array1[4]);
  
  array1.ResizeClear(3);
  EXPECT_EQ(3, array1.GetSize());
}

TEST(CoreBasicArray, PushPop) {
  Apto::Array<int, Apto::Basic> array;
  EXPECT_EQ(0, array.GetSize());
  
  for (int i = 0; i < 3; i++) array.Push(i);
  EXPECT_EQ(3, array.GetSize());
  EXPECT_EQ(2, array[2]);
  
  EXPECT_EQ(2, array.Pop());
  EXPECT_EQ(2, array.GetSize());
  EXPECT_EQ(1, array.Pop());
  array.Push(8);
  EXPECT_EQ(8, array.Pop());
  EXPECT_EQ(1, array.GetSize());
}

TEST(CoreBasicArray, Swap) {
  Apto::Array<int, Apto::Basic> array(3);  
  for (int i = 0; i < 3; i++) array[i] = i;
  
  EXPECT_EQ(0, array[0]);
  EXPECT_EQ(1, array[1]);
  EXPECT_EQ(2, array[2]);
  
  array.Swap(0, 2);
  
  EXPECT_EQ(2, array[0]);
  EXPECT_EQ(1, array[1]);
  EXPECT_EQ(0, array[2]);
}

TEST(CoreBasicArray, SetAll) {
  Apto::Array<int, Apto::Basic> array(3);
  
  array.SetAll(2);
  for (int i = 0; i < array.GetSize(); i++) EXPECT_EQ(2, array[i]);

  array.SetAll(26);
  for (int i = 0; i < array.GetSize(); i++) EXPECT_EQ(26, array[i]);
}

TEST(CoreBasicArray, Iterators) {
  Apto::Array<int, Apto::Basic> array(5);  
  for (int i = 0; i < 5; i++) array[i] = i;
  
  Apto::Array<int, Apto::Basic>::Iterator it = array.Begin();
  int i = 0;
  while (it.Next()) {
    EXPECT_EQ(i, *it.Get());
    i++;
  }
  
  const Apto::Array<int, Apto::Basic>& const_array = array;
  Apto::Array<int, Apto::Basic>::ConstIterator cit = const_array.Begin();
  i = 0;
  while (cit.Next()) {
    EXPECT_EQ(i, *cit.Get());
    i++;
  }
}


TEST(CoreBasicArray, Comparison) {
  Apto::Array<int, Apto::Basic> array1(3);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  
  Apto::Array<int, Apto::Basic> array2(array1);
  array2[1] = 5;
  
  EXPECT_TRUE(array1 == array1);
  EXPECT_TRUE(array2 == array2);
  EXPECT_TRUE(array1 != array2);
  EXPECT_FALSE(array1 == array2);
  EXPECT_FALSE(array1 != array1);
  EXPECT_FALSE(array2 != array2);
}


TEST(CoreBasicArray, Concatenation) {
  Apto::Array<int, Apto::Basic> array1(3);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  
  Apto::Array<int, Apto::Basic> array2(1);
  array2[0] = 5;
  
  array2 += array2;
  EXPECT_EQ(2, array2.GetSize());
  EXPECT_EQ(5, array2[0]);
  EXPECT_EQ(5, array2[1]);
  
  array1 += array2;
  
  EXPECT_EQ(0, array1[0]);
  EXPECT_EQ(1, array1[1]);
  EXPECT_EQ(2, array1[2]);
  EXPECT_EQ(5, array1[3]);
  EXPECT_EQ(5, array1[4]);
  
  Apto::Array<int, Apto::Basic> array3 = array2 + array2;
  EXPECT_EQ(4, array3.GetSize());
  EXPECT_EQ(5, array3[0]);
  EXPECT_EQ(5, array3[1]);
  EXPECT_EQ(5, array3[2]);
  EXPECT_EQ(5, array3[3]);  
  
  array2 = array2 + array2;
  EXPECT_TRUE(array2 == array3);
}



// Array<int, Smart>
// --------------------------------------------------------------------------------------------------------------  

TEST(CoreSmartArray, Construction) {
  Apto::Array<int, Apto::Smart> default_constructor;
  EXPECT_EQ(0, default_constructor.GetSize());
  Apto::Array<int, Apto::Smart> constructor_sz_3(3);
  EXPECT_EQ(3, constructor_sz_3.GetSize());
}

TEST(CoreSmartArray, Indexing) {
  Apto::Array<int, Apto::Smart> array(5);
  for (int i = 0; i < array.GetSize(); i++) array[i] = i;
  
  EXPECT_EQ(0, array[0]);
  EXPECT_EQ(1, array[1]);
  EXPECT_EQ(2, array[2]);
  EXPECT_EQ(3, array[3]);
  EXPECT_EQ(4, array[4]);
  
  array[3] = 12;
  
  EXPECT_EQ(12, array[3]);
}

TEST(CoreSmartArray, Assignment) {
  Apto::Array<int, Apto::Smart> array1(5);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  
  Apto::Array<int, Apto::Smart> array2(6);
  for (int i = 0; i < array2.GetSize(); i++) array2[i] = 5 + i;
  
  EXPECT_NE(array1.GetSize(), array2.GetSize());
  EXPECT_NE(array1[3], array2[3]);
  
  array1 = array2;
  
  EXPECT_EQ(array1.GetSize(), array2.GetSize());
  EXPECT_EQ(array1[3], array2[3]);
  
  Apto::Array<int, Apto::Smart> array_copy_constructor(array2);
  EXPECT_EQ(array2.GetSize(), array_copy_constructor.GetSize());
  EXPECT_EQ(array2[3], array_copy_constructor[3]);
}

TEST(CoreSmartArray, Resize) {
  Apto::Array<int, Apto::Smart> array1(5);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  EXPECT_EQ(5, array1.GetSize());
  EXPECT_EQ(4, array1[4]);
  
  EXPECT_EQ(0, array1.GetReserve());
  array1.SetReserve(100);
  EXPECT_EQ(100, array1.GetReserve());
  array1.Resize(20);
  EXPECT_EQ(20, array1.GetSize());
  EXPECT_EQ(4, array1[4]);
  EXPECT_EQ(100, array1.GetCapacity());
  
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = 10 + i;
  EXPECT_EQ(14, array1[4]);
  EXPECT_EQ(19, array1[9]);
  
  array1.Resize(5);
  EXPECT_EQ(14, array1[4]);
  
  array1.SetReserve(20);
  array1.ResizeClear(3);
  EXPECT_EQ(3, array1.GetSize());
  EXPECT_EQ(20, array1.GetCapacity());
}

TEST(CoreSmartArray, PushPop) {
  Apto::Array<int, Apto::Smart> array;
  EXPECT_EQ(0, array.GetSize());
  
  for (int i = 0; i < 3; i++) array.Push(i);
  EXPECT_EQ(3, array.GetSize());
  EXPECT_EQ(2, array[2]);
  
  EXPECT_EQ(2, array.Pop());
  EXPECT_EQ(2, array.GetSize());
  EXPECT_EQ(1, array.Pop());
  array.Push(8);
  EXPECT_EQ(8, array.Pop());
  EXPECT_EQ(1, array.GetSize());
}

TEST(CoreSmartArray, Swap) {
  Apto::Array<int, Apto::Smart> array(3);  
  for (int i = 0; i < 3; i++) array[i] = i;
  
  EXPECT_EQ(0, array[0]);
  EXPECT_EQ(1, array[1]);
  EXPECT_EQ(2, array[2]);
  
  array.Swap(0, 2);
  
  EXPECT_EQ(2, array[0]);
  EXPECT_EQ(1, array[1]);
  EXPECT_EQ(0, array[2]);
}

TEST(CoreSmartArray, SetAll) {
  Apto::Array<int, Apto::Smart> array(3);
  
  array.SetAll(2);
  for (int i = 0; i < array.GetSize(); i++) EXPECT_EQ(2, array[i]);
  
  array.SetAll(26);
  for (int i = 0; i < array.GetSize(); i++) EXPECT_EQ(26, array[i]);
}

TEST(CoreSmartArray, Iterators) {
  Apto::Array<int, Apto::Smart> array(5);  
  for (int i = 0; i < 5; i++) array[i] = i;
  
  Apto::Array<int, Apto::Smart>::Iterator it = array.Begin();
  int i = 0;
  while (it.Next()) {
    EXPECT_EQ(i, *it.Get());
    i++;
  }
  
  const Apto::Array<int, Apto::Smart>& const_array = array;
  Apto::Array<int, Apto::Smart>::ConstIterator cit = const_array.Begin();
  i = 0;
  while (cit.Next()) {
    EXPECT_EQ(i, *cit.Get());
    i++;
  }
}


TEST(CoreSmartArray, Comparison) {
  Apto::Array<int, Apto::Smart> array1(3);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  
  Apto::Array<int, Apto::Smart> array2(array1);
  array2[1] = 5;
  
  EXPECT_TRUE(array1 == array1);
  EXPECT_TRUE(array2 == array2);
  EXPECT_TRUE(array1 != array2);
  EXPECT_FALSE(array1 == array2);
  EXPECT_FALSE(array1 != array1);
  EXPECT_FALSE(array2 != array2);
}


TEST(CoreSmartArray, Concatenation) {
  Apto::Array<int, Apto::Smart> array1(3);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  
  Apto::Array<int, Apto::Smart> array2(1);
  array2[0] = 5;
  
  array2 += array2;
  EXPECT_EQ(2, array2.GetSize());
  EXPECT_EQ(5, array2[0]);
  EXPECT_EQ(5, array2[1]);
  
  array1 += array2;
  
  EXPECT_EQ(0, array1[0]);
  EXPECT_EQ(1, array1[1]);
  EXPECT_EQ(2, array1[2]);
  EXPECT_EQ(5, array1[3]);
  EXPECT_EQ(5, array1[4]);
  
  Apto::Array<int, Apto::Smart> array3 = array2 + array2;
  EXPECT_EQ(4, array3.GetSize());
  EXPECT_EQ(5, array3[0]);
  EXPECT_EQ(5, array3[1]);
  EXPECT_EQ(5, array3[2]);
  EXPECT_EQ(5, array3[3]);  
  
  array2 = array2 + array2;
  EXPECT_TRUE(array2 == array3);
}


// Array<int, ManagedPointer>
// --------------------------------------------------------------------------------------------------------------  

TEST(CoreManagedPointerArray, Construction) {
  Apto::Array<int, Apto::ManagedPointer> default_constructor;
  EXPECT_EQ(0, default_constructor.GetSize());
  Apto::Array<int, Apto::ManagedPointer> constructor_sz_3(3);
  EXPECT_EQ(3, constructor_sz_3.GetSize());
}

TEST(CoreManagedPointerArray, Indexing) {
  Apto::Array<int, Apto::ManagedPointer> array(5);
  for (int i = 0; i < array.GetSize(); i++) array[i] = i;
  
  EXPECT_EQ(0, array[0]);
  EXPECT_EQ(1, array[1]);
  EXPECT_EQ(2, array[2]);
  EXPECT_EQ(3, array[3]);
  EXPECT_EQ(4, array[4]);
  
  array[3] = 12;
  
  EXPECT_EQ(12, array[3]);
}

TEST(CoreManagedPointerArray, Assignment) {
  Apto::Array<int, Apto::ManagedPointer> array1(5);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  
  Apto::Array<int, Apto::ManagedPointer> array2(6);
  for (int i = 0; i < array2.GetSize(); i++) array2[i] = 5 + i;
  
  EXPECT_NE(array1.GetSize(), array2.GetSize());
  EXPECT_NE(array1[3], array2[3]);
  
  array1 = array2;
  
  EXPECT_EQ(array1.GetSize(), array2.GetSize());
  EXPECT_EQ(array1[3], array2[3]);
  
  Apto::Array<int, Apto::ManagedPointer> array_copy_constructor(array2);
  EXPECT_EQ(array2.GetSize(), array_copy_constructor.GetSize());
  EXPECT_EQ(array2[3], array_copy_constructor[3]);
}

TEST(CoreManagedPointerArray, Resize) {
  Apto::Array<int, Apto::ManagedPointer> array1(5);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  EXPECT_EQ(5, array1.GetSize());
  EXPECT_EQ(4, array1[4]);
  
  array1.Resize(20);
  EXPECT_EQ(20, array1.GetSize());
  EXPECT_EQ(4, array1[4]);
  
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = 10 + i;
  EXPECT_EQ(14, array1[4]);
  EXPECT_EQ(19, array1[9]);
  
  array1.Resize(5);
  EXPECT_EQ(14, array1[4]);
  
  array1.ResizeClear(3);
  EXPECT_EQ(3, array1.GetSize());
}

TEST(CoreManagedPointerArray, PushPop) {
  Apto::Array<int, Apto::ManagedPointer> array;
  EXPECT_EQ(0, array.GetSize());
  
  for (int i = 0; i < 3; i++) array.Push(i);
  EXPECT_EQ(3, array.GetSize());
  EXPECT_EQ(2, array[2]);
  
  EXPECT_EQ(2, array.Pop());
  EXPECT_EQ(2, array.GetSize());
  EXPECT_EQ(1, array.Pop());
  array.Push(8);
  EXPECT_EQ(8, array.Pop());
  EXPECT_EQ(1, array.GetSize());
}

TEST(CoreManagedPointerArray, Swap) {
  Apto::Array<int, Apto::ManagedPointer> array(3);  
  for (int i = 0; i < 3; i++) array[i] = i;
  
  EXPECT_EQ(0, array[0]);
  EXPECT_EQ(1, array[1]);
  EXPECT_EQ(2, array[2]);
  
  array.Swap(0, 2);
  
  EXPECT_EQ(2, array[0]);
  EXPECT_EQ(1, array[1]);
  EXPECT_EQ(0, array[2]);
}

TEST(CoreManagedPointerArray, SetAll) {
  Apto::Array<int, Apto::ManagedPointer> array(3);
  
  array.SetAll(2);
  for (int i = 0; i < array.GetSize(); i++) EXPECT_EQ(2, array[i]);
  
  array.SetAll(26);
  for (int i = 0; i < array.GetSize(); i++) EXPECT_EQ(26, array[i]);
}

TEST(CoreManagedPointerArray, Iterators) {
  Apto::Array<int, Apto::ManagedPointer> array(5);  
  for (int i = 0; i < 5; i++) array[i] = i;
  
  Apto::Array<int, Apto::ManagedPointer>::Iterator it = array.Begin();
  int i = 0;
  while (it.Next()) {
    EXPECT_EQ(i, *it.Get());
    i++;
  }
  
  const Apto::Array<int, Apto::ManagedPointer>& const_array = array;
  Apto::Array<int, Apto::ManagedPointer>::ConstIterator cit = const_array.Begin();
  i = 0;
  while (cit.Next()) {
    EXPECT_EQ(i, *cit.Get());
    i++;
  }
}


TEST(CoreManagedPointerArray, Comparison) {
  Apto::Array<int, Apto::ManagedPointer> array1(3);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  
  Apto::Array<int, Apto::ManagedPointer> array2(array1);
  array2[1] = 5;
  
  EXPECT_TRUE(array1 == array1);
  EXPECT_TRUE(array2 == array2);
  EXPECT_TRUE(array1 != array2);
  EXPECT_FALSE(array1 == array2);
  EXPECT_FALSE(array1 != array1);
  EXPECT_FALSE(array2 != array2);
}


TEST(CoreManagedPointerArray, Concatenation) {
  Apto::Array<int, Apto::ManagedPointer> array1(3);
  for (int i = 0; i < array1.GetSize(); i++) array1[i] = i;
  
  Apto::Array<int, Apto::ManagedPointer> array2(1);
  array2[0] = 5;
  
  array2 += array2;
  EXPECT_EQ(2, array2.GetSize());
  EXPECT_EQ(5, array2[0]);
  EXPECT_EQ(5, array2[1]);
  
  array1 += array2;
  
  EXPECT_EQ(0, array1[0]);
  EXPECT_EQ(1, array1[1]);
  EXPECT_EQ(2, array1[2]);
  EXPECT_EQ(5, array1[3]);
  EXPECT_EQ(5, array1[4]);
  
  Apto::Array<int, Apto::ManagedPointer> array3 = array2 + array2;
  EXPECT_EQ(4, array3.GetSize());
  EXPECT_EQ(5, array3[0]);
  EXPECT_EQ(5, array3[1]);
  EXPECT_EQ(5, array3[2]);
  EXPECT_EQ(5, array3[3]);  
  
  array2 = array2 + array2;
  EXPECT_TRUE(array2 == array3);
}


