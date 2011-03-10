/*
 *  unittests/core/List.cc
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

#include "apto/core/List.h"

#include "gtest/gtest.h"


// List<int, DL>
// --------------------------------------------------------------------------------------------------------------  

TEST(CoreDLList, Construction) {
  Apto::List<int, Apto::DL> default_constructor;
  EXPECT_EQ(0, default_constructor.GetSize());
}


TEST(CoreDLList, PushPop) {
  Apto::List<int, Apto::DL> list;
  EXPECT_EQ(0, list.GetSize());
  
  for (int i = 0; i < 3; i++) list.Push(i);
  EXPECT_EQ(3, list.GetSize());
  EXPECT_EQ(2, list.GetFirst());
  
  EXPECT_EQ(2, list.Pop());
  EXPECT_EQ(2, list.GetSize());
  EXPECT_EQ(1, list.Pop());
  list.Push(8);
  EXPECT_EQ(8, list.Pop());
  EXPECT_EQ(1, list.GetSize());
  
  list.PushRear(12);
  EXPECT_EQ(0, list.Pop());
  EXPECT_EQ(12, list.Pop());
  EXPECT_EQ(0, list.GetSize());
  
  for (int i = 0; i < 3; i++) list.PushRear(i);
  EXPECT_EQ(3, list.GetSize());
  EXPECT_EQ(2, list.GetLast());
  EXPECT_EQ(2, list.PopRear());
  
  list.Clear();
  EXPECT_EQ(0, list.GetSize());
}


TEST(CoreDLList, Assignment) {
  Apto::List<int, Apto::DL> list1;
  for (int i = 0; i < 5; i++) list1.PushRear(i);
  
  Apto::List<int, Apto::DL> list2;
  for (int i = 0; i < 6; i++) list2.PushRear(5 + i);
  
  EXPECT_NE(list1.GetSize(), list2.GetSize());
  EXPECT_NE(list1.GetFirst(), list2.GetFirst());
  
  list1 = list2;
  EXPECT_EQ(list1.GetSize(), list2.GetSize());
  EXPECT_EQ(list1.GetFirst(), list2.GetFirst());

  list1 = list1;
  EXPECT_EQ(list1.GetSize(), list2.GetSize());
  EXPECT_EQ(list1.GetFirst(), list2.GetFirst());

  Apto::List<int, Apto::DL> list_copy_constructor(list2);
  EXPECT_EQ(list2.GetSize(), list_copy_constructor.GetSize());
  EXPECT_EQ(list2.GetFirst(), list_copy_constructor.GetFirst());
}


TEST(CoreDLList, Remove) {
  Apto::List<int, Apto::DL> list;
  Apto::List<int, Apto::DL>::EntryHandle* handle = NULL;
  
  list.Push(5);
  list.Push(10, &handle);
  list.Push(15);
  EXPECT_TRUE(handle != NULL);
  if (handle) EXPECT_TRUE(handle->IsValid());
  list.Remove(10);
  if (handle) EXPECT_FALSE(handle->IsValid());
  EXPECT_EQ(15, list.Pop());
  EXPECT_EQ(5, list.Pop());
  EXPECT_EQ(0, list.GetSize());
  
  list.Push(5);
  list.Push(10, &handle);
  list.Push(15);
  EXPECT_TRUE(handle != NULL);
  if (handle) EXPECT_TRUE(handle->IsValid());
  handle->Remove();
  if (handle) EXPECT_FALSE(handle->IsValid());
  EXPECT_EQ(15, list.Pop());
  EXPECT_EQ(5, list.Pop());
  EXPECT_EQ(0, list.GetSize());
  
  delete handle;
}


TEST(CoreDLList, Contains) {
  Apto::List<int, Apto::DL> list;
  list.Push(5);
  list.Push(10);
  list.Push(15);
  EXPECT_TRUE(list.Contains(10));
  EXPECT_FALSE(list.Contains(20));
}


TEST(CoreDLList, Iterators) {
  Apto::List<int, Apto::DL> list;
  for (int i = 0; i < 5; i++) list.PushRear(i);
  
  Apto::List<int, Apto::DL>::Iterator it = list.Begin();
  int i = 0;
  while (it.Next()) {
    EXPECT_EQ(i, *it.Get());
    i++;
  }
  
  const Apto::List<int, Apto::DL>& const_list = list;
  Apto::List<int, Apto::DL>::ConstIterator cit = const_list.Begin();
  i = 0;
  while (cit.Next()) {
    EXPECT_EQ(i, *cit.Get());
    i++;
  }
}


TEST(CoreDLList, Comparison) {
  Apto::List<int, Apto::DL> list1;
  for (int i = 0; i < 3; i++) list1.PushRear(i);
  
  Apto::List<int, Apto::DL> list2(list1);
  list2.GetLast() = 5;
  
  EXPECT_TRUE(list1 == list1);
  EXPECT_TRUE(list2 == list2);
  EXPECT_TRUE(list1 != list2);
  EXPECT_FALSE(list1 == list2);
  EXPECT_FALSE(list1 != list1);
  EXPECT_FALSE(list2 != list2);
}


TEST(CoreDLList, Concatenation) {
  Apto::List<int, Apto::DL> list1;
  for (int i = 0; i < 3; i++) list1.PushRear(i);
  
  
  Apto::List<int, Apto::DL> list2;
  list2.PushRear(5);
  
  list2 += list2;
  EXPECT_EQ(2, list2.GetSize());
  EXPECT_EQ(5, list2.GetFirst());
  EXPECT_EQ(5, list2.GetLast());
  
  list1 += list2;
  
  EXPECT_EQ(0, list1.Pop());
  EXPECT_EQ(1, list1.Pop());
  EXPECT_EQ(2, list1.Pop());
  EXPECT_EQ(5, list1.Pop());
  EXPECT_EQ(5, list1.Pop());
  
  list2.PushRear(6);
  Apto::List<int, Apto::DL> list3 = list2 + list2;
  EXPECT_EQ(6, list3.GetSize());
  EXPECT_EQ(5, list3.GetFirst());
  EXPECT_EQ(6, list3.GetLast());
  
  list2 = list2 + list2;
  EXPECT_TRUE(list2 == list3);
}



// List<int, BufferedDL>
// --------------------------------------------------------------------------------------------------------------  

TEST(CoreBufferedDLList, Construction) {
  Apto::List<int, Apto::BufferedDL> default_constructor;
  EXPECT_EQ(0, default_constructor.GetSize());
}


TEST(CoreBufferedDLList, PushPop) {
  Apto::List<int, Apto::BufferedDL> list;
  EXPECT_EQ(0, list.GetSize());
  
  for (int i = 0; i < 3; i++) list.Push(i);
  EXPECT_EQ(3, list.GetSize());
  EXPECT_EQ(2, list.GetFirst());
  
  EXPECT_EQ(2, list.Pop());
  EXPECT_EQ(2, list.GetSize());
  EXPECT_EQ(1, list.Pop());
  list.Push(8);
  EXPECT_EQ(8, list.Pop());
  EXPECT_EQ(1, list.GetSize());
  
  list.PushRear(12);
  EXPECT_EQ(0, list.Pop());
  EXPECT_EQ(12, list.Pop());
  EXPECT_EQ(0, list.GetSize());
  
  for (int i = 0; i < 3; i++) list.PushRear(i);
  EXPECT_EQ(3, list.GetSize());
  EXPECT_EQ(2, list.GetLast());
  EXPECT_EQ(2, list.PopRear());
  
  list.Clear();
  EXPECT_EQ(0, list.GetSize());
}


TEST(CoreBufferedDLList, Assignment) {
  Apto::List<int, Apto::BufferedDL> list1;
  for (int i = 0; i < 5; i++) list1.PushRear(i);
  
  Apto::List<int, Apto::BufferedDL> list2;
  for (int i = 0; i < 6; i++) list2.PushRear(5 + i);
  
  EXPECT_NE(list1.GetSize(), list2.GetSize());
  EXPECT_NE(list1.GetFirst(), list2.GetFirst());
  
  list1 = list2;
  EXPECT_EQ(list1.GetSize(), list2.GetSize());
  EXPECT_EQ(list1.GetFirst(), list2.GetFirst());
  
  list1 = list1;
  EXPECT_EQ(list1.GetSize(), list2.GetSize());
  EXPECT_EQ(list1.GetFirst(), list2.GetFirst());

  Apto::List<int, Apto::BufferedDL> list_copy_constructor(list2);
  EXPECT_EQ(list2.GetSize(), list_copy_constructor.GetSize());
  EXPECT_EQ(list2.GetFirst(), list_copy_constructor.GetFirst());
}


TEST(CoreBufferedDLList, Remove) {
  Apto::List<int, Apto::BufferedDL> list;
  Apto::List<int, Apto::BufferedDL>::EntryHandle* handle = NULL;
  
  list.Push(5);
  list.Push(10, &handle);
  list.Push(15);
  EXPECT_TRUE(handle != NULL);
  if (handle) EXPECT_TRUE(handle->IsValid());
  list.Remove(10);
  if (handle) EXPECT_FALSE(handle->IsValid());
  EXPECT_EQ(15, list.Pop());
  EXPECT_EQ(5, list.Pop());
  EXPECT_EQ(0, list.GetSize());
  
  list.Push(5);
  list.Push(10, &handle);
  list.Push(15);
  EXPECT_TRUE(handle != NULL);
  if (handle) EXPECT_TRUE(handle->IsValid());
  handle->Remove();
  if (handle) EXPECT_FALSE(handle->IsValid());
  EXPECT_EQ(15, list.Pop());
  EXPECT_EQ(5, list.Pop());
  EXPECT_EQ(0, list.GetSize());
  
  delete handle;
}


TEST(CoreBufferedDLList, Contains) {
  Apto::List<int, Apto::BufferedDL> list;
  list.Push(5);
  list.Push(10);
  list.Push(15);
  EXPECT_TRUE(list.Contains(10));
  EXPECT_FALSE(list.Contains(20));
}


TEST(CoreBufferedDLList, Iterators) {
  Apto::List<int, Apto::BufferedDL> list;
  for (int i = 0; i < 5; i++) list.PushRear(i);
  
  Apto::List<int, Apto::BufferedDL>::Iterator it = list.Begin();
  int i = 0;
  while (it.Next()) {
    EXPECT_EQ(i, *it.Get());
    i++;
  }
  
  const Apto::List<int, Apto::BufferedDL>& const_list = list;
  Apto::List<int, Apto::BufferedDL>::ConstIterator cit = const_list.Begin();
  i = 0;
  while (cit.Next()) {
    EXPECT_EQ(i, *cit.Get());
    i++;
  }
}


TEST(CoreBufferedDLList, Comparison) {
  Apto::List<int, Apto::BufferedDL> list1;
  for (int i = 0; i < 3; i++) list1.PushRear(i);
  
  Apto::List<int, Apto::BufferedDL> list2(list1);
  list2.GetLast() = 5;
  
  EXPECT_TRUE(list1 == list1);
  EXPECT_TRUE(list2 == list2);
  EXPECT_TRUE(list1 != list2);
  EXPECT_FALSE(list1 == list2);
  EXPECT_FALSE(list1 != list1);
  EXPECT_FALSE(list2 != list2);
}


TEST(CoreBufferedDLList, Concatenation) {
  Apto::List<int, Apto::BufferedDL> list1;
  for (int i = 0; i < 3; i++) list1.PushRear(i);
  
  
  Apto::List<int, Apto::BufferedDL> list2;
  list2.PushRear(5);
  
  list2 += list2;
  EXPECT_EQ(2, list2.GetSize());
  EXPECT_EQ(5, list2.GetFirst());
  EXPECT_EQ(5, list2.GetLast());
  
  list1 += list2;
  
  EXPECT_EQ(0, list1.Pop());
  EXPECT_EQ(1, list1.Pop());
  EXPECT_EQ(2, list1.Pop());
  EXPECT_EQ(5, list1.Pop());
  EXPECT_EQ(5, list1.Pop());
  
  list2.PushRear(6);
  Apto::List<int, Apto::BufferedDL> list3 = list2 + list2;
  EXPECT_EQ(6, list3.GetSize());
  EXPECT_EQ(5, list3.GetFirst());
  EXPECT_EQ(6, list3.GetLast());
  
  list2 = list2 + list2;
  EXPECT_TRUE(list2 == list3);
}



// List<int, SparseVector>
// --------------------------------------------------------------------------------------------------------------  

TEST(CoreSparseVectorList, Construction) {
  Apto::List<int, Apto::SparseVector> default_constructor;
  EXPECT_EQ(0, default_constructor.GetSize());
}


TEST(CoreSparseVectorList, PushPop) {
  Apto::List<int, Apto::SparseVector> list;
  EXPECT_EQ(0, list.GetSize());
  
  for (int i = 0; i < 3; i++) list.Push(i);
  EXPECT_EQ(3, list.GetSize());
  EXPECT_EQ(2, list.GetFirst());
  
  EXPECT_EQ(2, list.Pop());
  EXPECT_EQ(2, list.GetSize());
  EXPECT_EQ(1, list.Pop());
  list.Push(8);
  EXPECT_EQ(8, list.Pop());
  EXPECT_EQ(1, list.GetSize());
  
  list.PushRear(12);
  EXPECT_EQ(0, list.Pop());
  EXPECT_EQ(12, list.Pop());
  EXPECT_EQ(0, list.GetSize());
  
  for (int i = 0; i < 3; i++) list.PushRear(i);
  EXPECT_EQ(3, list.GetSize());
  EXPECT_EQ(2, list.GetLast());
  EXPECT_EQ(2, list.PopRear());
  
  list.Clear();
  EXPECT_EQ(0, list.GetSize());
}


TEST(CoreSparseVectorList, Assignment) {
  Apto::List<int, Apto::SparseVector> list1;
  for (int i = 0; i < 5; i++) list1.PushRear(i);
  
  Apto::List<int, Apto::SparseVector> list2;
  for (int i = 0; i < 6; i++) list2.PushRear(5 + i);
  
  EXPECT_NE(list1.GetSize(), list2.GetSize());
  EXPECT_NE(list1.GetFirst(), list2.GetFirst());
  
  list1 = list2;
  EXPECT_EQ(list1.GetSize(), list2.GetSize());
  EXPECT_EQ(list1.GetFirst(), list2.GetFirst());
  
  list1 = list1;
  EXPECT_EQ(list1.GetSize(), list2.GetSize());
  EXPECT_EQ(list1.GetFirst(), list2.GetFirst());
  
  Apto::List<int, Apto::SparseVector> list_copy_constructor(list2);
  EXPECT_EQ(list2.GetSize(), list_copy_constructor.GetSize());
  EXPECT_EQ(list2.GetFirst(), list_copy_constructor.GetFirst());
}


TEST(CoreSparseVectorList, Remove) {
  Apto::List<int, Apto::SparseVector> list;
  Apto::List<int, Apto::SparseVector>::EntryHandle* handle = NULL;
  
  list.Push(5);
  list.Push(10, &handle);
  list.Push(15);
  EXPECT_TRUE(handle != NULL);
  if (handle) EXPECT_TRUE(handle->IsValid());
  list.Remove(10);
  if (handle) EXPECT_FALSE(handle->IsValid());
  EXPECT_EQ(15, list.Pop());
  EXPECT_EQ(5, list.Pop());
  EXPECT_EQ(0, list.GetSize());
  
  list.Push(5);
  list.Push(10, &handle);
  list.Push(15);
  EXPECT_TRUE(handle != NULL);
  if (handle) EXPECT_TRUE(handle->IsValid());
  handle->Remove();
  if (handle) EXPECT_FALSE(handle->IsValid());
  EXPECT_EQ(15, list.Pop());
  EXPECT_EQ(5, list.Pop());
  EXPECT_EQ(0, list.GetSize());
  
  delete handle;
}


TEST(CoreSparseVectorList, Contains) {
  Apto::List<int, Apto::SparseVector> list;
  list.Push(5);
  list.Push(10);
  list.Push(15);
  EXPECT_TRUE(list.Contains(10));
  EXPECT_FALSE(list.Contains(20));
}


TEST(CoreSparseVectorList, Iterators) {
  Apto::List<int, Apto::SparseVector> list;
  for (int i = 0; i < 5; i++) list.PushRear(i);
  
  Apto::List<int, Apto::SparseVector>::Iterator it = list.Begin();
  int i = 0;
  while (it.Next()) {
    EXPECT_EQ(i, *it.Get());
    i++;
  }
  
  const Apto::List<int, Apto::SparseVector>& const_list = list;
  Apto::List<int, Apto::SparseVector>::ConstIterator cit = const_list.Begin();
  i = 0;
  while (cit.Next()) {
    EXPECT_EQ(i, *cit.Get());
    i++;
  }
}


TEST(CoreSparseVectorList, Comparison) {
  Apto::List<int, Apto::SparseVector> list1;
  for (int i = 0; i < 3; i++) list1.PushRear(i);
  
  Apto::List<int, Apto::SparseVector> list2(list1);
  list2.GetLast() = 5;
  
  EXPECT_TRUE(list1 == list1);
  EXPECT_TRUE(list2 == list2);
  EXPECT_TRUE(list1 != list2);
  EXPECT_FALSE(list1 == list2);
  EXPECT_FALSE(list1 != list1);
  EXPECT_FALSE(list2 != list2);
}


TEST(CoreSparseVectorList, Concatenation) {
  Apto::List<int, Apto::SparseVector> list1;
  for (int i = 0; i < 3; i++) list1.PushRear(i);
  
  
  Apto::List<int, Apto::SparseVector> list2;
  list2.PushRear(5);
  
  list2 += list2;
  EXPECT_EQ(2, list2.GetSize());
  EXPECT_EQ(5, list2.GetFirst());
  EXPECT_EQ(5, list2.GetLast());
  
  list1 += list2;
  
  EXPECT_EQ(0, list1.Pop());
  EXPECT_EQ(1, list1.Pop());
  EXPECT_EQ(2, list1.Pop());
  EXPECT_EQ(5, list1.Pop());
  EXPECT_EQ(5, list1.Pop());
  
  list2.PushRear(6);
  Apto::List<int, Apto::SparseVector> list3 = list2 + list2;
  EXPECT_EQ(6, list3.GetSize());
  EXPECT_EQ(5, list3.GetFirst());
  EXPECT_EQ(6, list3.GetLast());
  
  list2 = list2 + list2;
  EXPECT_TRUE(list2 == list3);
}
