/*
 *  unittests/core/Map.cc
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
#include "apto/core/Map.h"

#include "gtest/gtest.h"


TEST(CoreHashBTreeMap, Construction) {
  Apto::Map<int, int, Apto::HashBTree> map;
  EXPECT_EQ(0, map.GetSize());
}


TEST(CoreHashBTreeMap, Indexing) {
  Apto::Map<int, int, Apto::HashBTree> map;
  map[1] = 1;
  map.Set(2, 2);
  map.Get(3) = 3;
  
  int val;
  
  EXPECT_EQ(3, map.GetSize());
  EXPECT_EQ(1, map[1]);
  EXPECT_EQ(1, map.Get(1));
  EXPECT_EQ(1, map.GetWithDefault(1, -1));
  val = -1;
  EXPECT_TRUE(map.Get(1, val));
  EXPECT_EQ(1, val);
  
  EXPECT_EQ(2, map[2]);
  EXPECT_EQ(2, map.Get(2));
  EXPECT_EQ(2, map.GetWithDefault(2, -1));
  val = -1;
  EXPECT_TRUE(map.Get(2, val));
  EXPECT_EQ(2, val);
  
  EXPECT_EQ(3, map[3]);
  EXPECT_EQ(3, map.Get(3));
  EXPECT_EQ(3, map.GetWithDefault(3, -1));
  val = -1;
  EXPECT_TRUE(map.Get(3, val));
  EXPECT_EQ(3, val);
  
  EXPECT_EQ(4, map.GetWithDefault(4, 4));
  EXPECT_EQ(4, map.GetSize());
}


TEST(CoreHashBTreeMap, Assignment) {
  Apto::Map<int, int, Apto::HashBTree> map1;
  for (int i = 0; i < 4; i++) map1[i] = i;
  for (int i = 0; i < 4; i++) EXPECT_EQ(i, map1[i]);
  
  Apto::Map<int, int, Apto::HashBTree> map2;
  for (int i = 0; i < 4; i++) map2[i] = i + 2;
  for (int i = 0; i < 4; i++) EXPECT_EQ(i + 2, map2[i]);
  
  map2 = map1;
  for (int i = 0; i < 4; i++) EXPECT_EQ(i, map2[i]);
  
  Apto::Map<int, int, Apto::HashBTree> map3(map1);
  for (int i = 0; i < 4; i++) EXPECT_EQ(i, map3[i]);
}


TEST(CoreHashBTreeMap, Removal) {
  
}


TEST(CoreHashBTreeMap, Comparison) {
  Apto::Map<int, int, Apto::HashBTree> map1;
  for (int i = 0; i < 4; i++) map1[i] = i;
  EXPECT_TRUE(map1 == map1);
  EXPECT_FALSE(map1 != map1);
  
  Apto::Map<int, int, Apto::HashBTree> map2;
  for (int i = 0; i < 4; i++) map2[i] = i + 2;
  EXPECT_TRUE(map2 == map2);
  EXPECT_FALSE(map2 != map2);

  EXPECT_FALSE(map1 == map2);
  EXPECT_TRUE(map1 != map2);
  EXPECT_FALSE(map2 == map1);
  EXPECT_TRUE(map2 != map1);
  
  map2 = map1;  
  EXPECT_FALSE(map1 != map2);
  EXPECT_TRUE(map1 == map2);
  EXPECT_FALSE(map2 != map1);
  EXPECT_TRUE(map2 == map1);
  
  Apto::Map<int, int, Apto::HashBTree> map3(map1);
  EXPECT_FALSE(map1 != map3);
  EXPECT_TRUE(map1 == map3);
  EXPECT_FALSE(map3 != map1);
  EXPECT_TRUE(map3 == map1);
  
  map3[5] = 5;
  EXPECT_FALSE(map1 == map3);
  EXPECT_TRUE(map1 != map3);
  EXPECT_FALSE(map3 == map1);
  EXPECT_TRUE(map3 != map1);
}


TEST(CoreHashBTreeMap, Iteration) {
  Apto::Map<int, int, Apto::HashBTree> map1;
  for (int i = 0; i < 4; i++) map1[i] = i;

  Apto::Array<int> key_array(map1.GetSize());
  Apto::Array<int> value_array(map1.GetSize());
  Apto::Map<int, int, Apto::HashBTree>::Iterator it = map1.Begin();
  for (int i = 0; it.Next(); i++) {
    key_array[i] = it.Get()->Value1();
    value_array[i] = *it.Get()->Value2();
  }
  QSort(key_array);
  QSort(value_array);
  for (int i = 0; i < map1.GetSize(); i++) {
    EXPECT_EQ(i, key_array[i]);
    EXPECT_EQ(i, value_array[i]);
  }
  
  map1[map1.GetSize()] = map1.GetSize();
  EXPECT_EQ(map1.GetSize() - 1, map1[map1.GetSize() - 1]);
  
  key_array.Resize(map1.GetSize());
  Apto::Map<int, int, Apto::HashBTree>::KeyIterator kit = map1.Keys();
  for (int i = 0; kit.Next(); i++) key_array[i] = *kit.Get();
  QSort(key_array);
  for (int i = 0; i < map1.GetSize(); i++) EXPECT_EQ(i, key_array[i]);
  
  value_array.Resize(map1.GetSize());
  Apto::Map<int, int, Apto::HashBTree>::ValueIterator vit = map1.Values();
  for (int i = 0; vit.Next(); i++) value_array[i] = *vit.Get();
  QSort(value_array);
  for (int i = 0; i < map1.GetSize(); i++) EXPECT_EQ(i, value_array[i]);
}
