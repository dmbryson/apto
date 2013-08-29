/*
 *  unittests/core/Set.cc
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
#include "apto/core/ArrayUtils.h"
#include "apto/core/Set.h"

#include "gtest/gtest.h"


// Set<int, HashBTree>
// --------------------------------------------------------------------------------------------------------------  

TEST(CoreHashBTreeSet, Construction) {
  Apto::Set<int, Apto::DefaultHashBTree> set;
  EXPECT_EQ(0, set.GetSize());
}


TEST(CoreHashBTreeSet, Insertion) {
  Apto::Set<int, Apto::DefaultHashBTree> set;
  for (int i = 0; i < 5; i++) set.Insert(i);
  EXPECT_EQ(5, set.GetSize());
  
  set.Insert(4);
  EXPECT_EQ(5, set.GetSize());
  
  for (int i = 0; i < 5; i++) EXPECT_TRUE(set.Has(i));
  
  set.Insert(5);
  EXPECT_EQ(6, set.GetSize());
}

TEST(CoreHashBTreeSet, AddRemove100) {
  Apto::Set<int> set;
  for (int i = 0; i < 100; i++) { set.Insert(i); }
  for (int i = 0; i < 100; i++) { set.Remove(i); }
  
  for (int i = 0; i < 100; i++) { set.Insert(i); }
  for (int i = 99; i >= 0; i--) { set.Remove(i); }
}

TEST(CoreHashBTreeSet, AddRemove1000) {
  Apto::Set<int> set;
  for (int i = 0; i < 1000; i++) { set.Insert(i); }
  for (int i = 0; i < 1000; i++) { set.Remove(i); }
  
  for (int i = 0; i < 1000; i++) { set.Insert(i); }
  for (int i = 999; i >= 0; i--) { set.Remove(i); }
}


TEST(CoreHashBTreeSet, Assignment) {
  Apto::Set<int, Apto::DefaultHashBTree> set1;
  for (int i = 0; i < 4; i++) set1.Insert(i);
  
  Apto::Set<int, Apto::DefaultHashBTree> set2;
  for (int i = 0; i < 4; i++) set2.Insert(i + 2);
  EXPECT_TRUE(set2.Has(5));
  
  set2 = set1;
  for (int i = 0; i < 4; i++) EXPECT_TRUE(set2.Has(i));
  EXPECT_FALSE(set2.Has(5));
  
  Apto::Set<int, Apto::DefaultHashBTree> set3(set1);
  for (int i = 0; i < 4; i++) EXPECT_TRUE(set3.Has(i));  
}


template <class K, class V> class Hash23BTree : public Apto::HashBTree<K, V, 23> { ; };


TEST(CoreHashBTreeSet, Removal) {
  Apto::Set<int, Apto::DefaultHashBTree> set;
  for (int i = 0; i < 5; i++) set.Insert(i);
  EXPECT_EQ(5, set.GetSize());
  
  set.Remove(4);
  EXPECT_EQ(4, set.GetSize());
  
  
  // Test to check for proper removal of last entry in the tree array
  // - was crashing when cur_idx == last_idx, but size > 0
  Apto::Set<int, Hash23BTree> set2;
  set2.Insert(23);
  set2.Insert(46);
  
  set2.Remove(46);
  set2.Insert(46);
}


TEST(CoreHashBTreeSet, Comparison) {
  Apto::Set<int, Apto::DefaultHashBTree> set1;
  for (int i = 0; i < 4; i++) set1.Insert(i);
  EXPECT_TRUE(set1 == set1);
  EXPECT_FALSE(set1 != set1);
  
  Apto::Set<int, Apto::DefaultHashBTree> set2;
  for (int i = 0; i < 4; i++) set2.Insert(i + 2);
  EXPECT_TRUE(set2 == set2);
  EXPECT_FALSE(set2 != set2);
  
  EXPECT_FALSE(set1 == set2);
  EXPECT_TRUE(set1 != set2);
  EXPECT_FALSE(set2 == set1);
  EXPECT_TRUE(set2 != set1);
  
  set2 = set1;  
  EXPECT_FALSE(set1 != set2);
  EXPECT_TRUE(set1 == set2);
  EXPECT_FALSE(set2 != set1);
  EXPECT_TRUE(set2 == set1);
  
  Apto::Set<int, Apto::DefaultHashBTree> set3(set1);
  EXPECT_FALSE(set1 != set3);
  EXPECT_TRUE(set1 == set3);
  EXPECT_FALSE(set3 != set1);
  EXPECT_TRUE(set3 == set1);
  
  set3.Insert(5);
  EXPECT_FALSE(set1 == set3);
  EXPECT_TRUE(set1 != set3);
  EXPECT_FALSE(set3 == set1);
  EXPECT_TRUE(set3 != set1);  
}


TEST(CoreHashBTreeSet, Iteration) {
  Apto::Set<int, Apto::DefaultHashBTree> set1;
  for (int i = 0; i < 4; i++) set1.Insert(i);
  
  Apto::Array<int> array(set1.GetSize());
  Apto::Set<int, Apto::DefaultHashBTree>::Iterator it = set1.Begin();
  for (int i = 0; it.Next(); i++) array[i] = *it.Get();
  QSort(array);
  for (int i = 0; i < array.GetSize(); i++) EXPECT_EQ(i, array[i]);  

  const Apto::Set<int, Apto::DefaultHashBTree>& cset1 = set1;
  Apto::Set<int, Apto::DefaultHashBTree>::ConstIterator cit = cset1.Begin();
  for (int i = 0; cit.Next(); i++) array[i] = *cit.Get();
  QSort(array);
  for (int i = 0; i < array.GetSize(); i++) EXPECT_EQ(i, array[i]);  
}


// Set<int, HashBTree, Multi>
// --------------------------------------------------------------------------------------------------------------  

TEST(CoreHashBTreeMulitSet, Construction) {
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi> set;
  EXPECT_EQ(0, set.GetSize());  
}


TEST(CoreHashBTreeMulitSet, Insertion) {
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi> set;
  for (int i = 0; i < 5; i++) set.Insert(i);
  EXPECT_EQ(5, set.GetSize());
  
  set.Insert(4);
  EXPECT_EQ(6, set.GetSize());
  
  for (int i = 0; i < 5; i++) EXPECT_TRUE(set.Has(i));
  
  set.Insert(5);
  EXPECT_EQ(7, set.GetSize());
}


TEST(CoreHashBTreeMulitSet, Assignment) {
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi> set1;
  for (int i = 0; i < 4; i++) {
    set1.Insert(i);
    set1.Insert(i);
  }
  EXPECT_EQ(8, set1.GetSize());
  
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi> set2;
  for (int i = 0; i < 4; i++) {
    set2.Insert(i + 2);
    set2.Insert(i + 2);
    set2.Insert(i + 2);
  }
  EXPECT_TRUE(set2.Has(5));
  EXPECT_EQ(3, set2.Count(5));
  EXPECT_EQ(12, set2.GetSize());
  
  set2 = set1;
  EXPECT_EQ(8, set2.GetSize());
  for (int i = 0; i < 4; i++) EXPECT_TRUE(set2.Has(i));
  EXPECT_FALSE(set2.Has(5));
  
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi> set3(set1);
  EXPECT_EQ(8, set3.GetSize());
  for (int i = 0; i < 4; i++) {
    EXPECT_TRUE(set3.Has(i));  
    EXPECT_EQ(2, set3.Count(i));
  }
}


TEST(CoreHashBTreeMulitSet, Removal) {
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi> set;
  for (int i = 0; i < 5; i++) set.Insert(i);
  EXPECT_EQ(5, set.GetSize());
  
  set.Insert(4);
  EXPECT_EQ(6, set.GetSize());
  
  set.Remove(4);
  EXPECT_EQ(5, set.GetSize());
  EXPECT_TRUE(set.Has(4));
  
  set.Remove(4);
  EXPECT_EQ(4, set.GetSize());  
  EXPECT_FALSE(set.Has(4));
}


TEST(CoreHashBTreeMulitSet, Comparison) {
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi> set1;
  for (int i = 0; i < 4; i++) set1.Insert(i);
  set1.Insert(3);
  EXPECT_TRUE(set1 == set1);
  EXPECT_FALSE(set1 != set1);
  
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi> set2;
  for (int i = 0; i < 4; i++) set2.Insert(i + 2);
  EXPECT_TRUE(set2 == set2);
  EXPECT_FALSE(set2 != set2);
  
  EXPECT_FALSE(set1 == set2);
  EXPECT_TRUE(set1 != set2);
  EXPECT_FALSE(set2 == set1);
  EXPECT_TRUE(set2 != set1);
  
  set2 = set1;  
  EXPECT_FALSE(set1 != set2);
  EXPECT_TRUE(set1 == set2);
  EXPECT_FALSE(set2 != set1);
  EXPECT_TRUE(set2 == set1);
  
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi> set3(set1);
  EXPECT_FALSE(set1 != set3);
  EXPECT_TRUE(set1 == set3);
  EXPECT_FALSE(set3 != set1);
  EXPECT_TRUE(set3 == set1);
  
  set3.Insert(5);
  EXPECT_FALSE(set1 == set3);
  EXPECT_TRUE(set1 != set3);
  EXPECT_FALSE(set3 == set1);
  EXPECT_TRUE(set3 != set1);  
}


TEST(CoreHashBTreeMulitSet, Iteration) {
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi> set1;
  for (int i = 0; i < 8; i += 2) {
    set1.Insert(i);
    set1.Insert(i);
  }
  
  Apto::Array<int> array(set1.GetSize());
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi>::Iterator it = set1.Begin();
  for (int i = 0; it.Next(); i++) array[i] = *it.Get();
  QSort(array);
  for (int i = 0; i < array.GetSize(); i += 2) {
    EXPECT_EQ(i, array[i]);
    EXPECT_EQ(i, array[i + 1]);
  }

  const Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi>& cset1 = set1;
  Apto::Set<int, Apto::DefaultHashBTree, Apto::Multi>::ConstIterator cit = cset1.Begin();
  for (int i = 0; cit.Next(); i++) array[i] = *cit.Get();
  QSort(array);
  for (int i = 0; i < array.GetSize(); i += 2) {
    EXPECT_EQ(i, array[i]);
    EXPECT_EQ(i, array[i + 1]);
  }
}

