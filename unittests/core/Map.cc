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
#include "apto/core/String.h"

#include "gtest/gtest.h"


// Map<int, int, HashBTree>
// --------------------------------------------------------------------------------------------------------------  

TEST(CoreHashBTreeMap, Construction) {
  Apto::Map<int, int, Apto::DefaultHashBTree> map;
  EXPECT_EQ(0, map.GetSize());
}


TEST(CoreHashBTreeMap, Indexing) {
  Apto::Map<int, int, Apto::DefaultHashBTree> map;
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


TEST(CoreHashBTreeMap, AddRemove100) {
  Apto::Map<int, int> map;
  for (int i = 0; i < 100; i++) { map.Set(i, i); }
  for (int i = 0; i < 100; i++) { map.Remove(i); }
}

TEST(CoreHashBTreeMap, Remove1000) {
  Apto::Map<int, int> map;
  for (int i = 0; i < 1000; i++) { map.Set(i, i); }
  for (int i = 0; i < 1000; i++) { map.Remove(i); }
}



TEST(CoreHashBTreeMap, Assignment) {
  Apto::Map<int, int, Apto::DefaultHashBTree> map1;
  for (int i = 0; i < 4; i++) map1[i] = i;
  for (int i = 0; i < 4; i++) EXPECT_EQ(i, map1[i]);
  
  Apto::Map<int, int, Apto::DefaultHashBTree> map2;
  for (int i = 0; i < 4; i++) map2[i] = i + 2;
  for (int i = 0; i < 4; i++) EXPECT_EQ(i + 2, map2[i]);
  
  map2 = map1;
  for (int i = 0; i < 4; i++) EXPECT_EQ(i, map2[i]);
  
  Apto::Map<int, int, Apto::DefaultHashBTree> map3(map1);
  for (int i = 0; i < 4; i++) EXPECT_EQ(i, map3[i]);
}

template <class K, class V> class HashBTreeSize1 : public Apto::HashBTree<K, V, 1> { ; };

TEST(CoreHashBTreeMap, Removal) {
  // Set HashFactor to 1 to really test the tree removal
  Apto::Map<int, int, HashBTreeSize1> map;
  EXPECT_FALSE(map.Remove(8));
  EXPECT_EQ(0, map.GetSize());
  
  // Build fully balanced tree
  map[4] = 4;
  map[2] = 2;
  map[6] = 6;
  map[1] = 1;
  map[3] = 3;
  map[5] = 5;
  map[7] = 7;
  
  // Check full tree
  EXPECT_EQ(7, map.GetSize());
  for (int i = 1; i <= 7; i++) EXPECT_EQ(i, map[i]);
  
  // Remove left leaf node
  EXPECT_TRUE(map.Remove(1));
  EXPECT_EQ(6, map.GetSize());
  for (int i = 2; i <= 7; i++) EXPECT_EQ(i, map[i]);
  
  // Remove right leaf node
  EXPECT_TRUE(map.Remove(7));
  EXPECT_EQ(5, map.GetSize());
  for (int i = 2; i <= 6; i++) EXPECT_EQ(i, map[i]);
  
  // Remove internal node with right only subtree
  EXPECT_TRUE(map.Remove(2));
  EXPECT_EQ(4, map.GetSize());
  for (int i = 3; i <= 6; i++) EXPECT_EQ(i, map[i]);
  
  // Remove internal node with left only subtree
  EXPECT_TRUE(map.Remove(6));
  EXPECT_EQ(3, map.GetSize());
  for (int i = 3; i <= 5; i++) EXPECT_EQ(i, map[i]);
  
  // Remove internal node with two subtrees (also the root node)
  EXPECT_TRUE(map.Remove(4));
  EXPECT_EQ(2, map.GetSize());
  EXPECT_EQ(3, map[3]);
  EXPECT_EQ(5, map[5]);
  
  // Make sure that remove works properly when node does not exist
  EXPECT_FALSE(map.Remove(8));
  EXPECT_EQ(2, map.GetSize());
  
  // Make sure we can still add into the map
  map[4] = 4;
  EXPECT_EQ(3, map.GetSize());
  
  
  Apto::Map<Apto::String, int, HashBTreeSize1> map2;
  map2["DIVIDE_DEL_PROB"] = 1;
  map2["RETURN_STORED_ON_DEATH"] = 2;
  map2["REQUIRED_RESOURCE_LEVEL"] = 3;

  EXPECT_TRUE(map2.Has("DIVIDE_DEL_PROB"));
  EXPECT_TRUE(map2.Has("RETURN_STORED_ON_DEATH"));
  EXPECT_TRUE(map2.Has("REQUIRED_RESOURCE_LEVEL"));
  map2.Remove("DIVIDE_DEL_PROB");
  EXPECT_FALSE(map2.Has("DIVIDE_DEL_PROB"));
  EXPECT_TRUE(map2.Has("RETURN_STORED_ON_DEATH"));
  EXPECT_TRUE(map2.Has("REQUIRED_RESOURCE_LEVEL"));
  
  
  
  Apto::Map<Apto::String, int> map3;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org0-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org1-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org2-ud-1-grp-1_ft3-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org3-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org4-ud-1-grp-1_ft5-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org5-ud-1-grp-1_ft3-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org6-ud-1-grp-1_ft1-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org7-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org8-ud-1-grp-1_ft0-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org9-ud-1-grp-1_ft5-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org10-ud-1-grp-1_ft4-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org11-ud-1-grp-1_ft5-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org12-ud-1-grp-1_ft5-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org13-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org14-ud-1-grp-1_ft4-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org15-ud-1-grp-1_ft5-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org16-ud-1-grp-1_ft4-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org17-ud-1-grp-1_ft1-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org18-ud-1-grp-1_ft1-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org19-ud-1-grp-1_ft1-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org20-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org21-ud-1-grp-1_ft3-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org22-ud-1-grp-1_ft3-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org23-ud-1-grp-1_ft5-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org24-ud-1-grp-1_ft3-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org25-ud-1-grp-1_ft3-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org26-ud-1-grp-1_ft2-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org27-ud-1-grp-1_ft1-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org28-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org29-ud-1-grp-1_ft4-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org30-ud-1-grp-1_ft5-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org31-ud-1-grp-1_ft5-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org32-ud-1-grp-1_ft1-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org33-ud-1-grp-1_ft1-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org34-ud-1-grp-1_ft5-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org35-ud-1-grp-1_ft4-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org36-ud-1-grp-1_ft1-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org37-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org38-ud-1-grp-1_ft4-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org39-ud-1-grp-1_ft3-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org40-ud-1-grp-1_ft3-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org41-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org42-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org43-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org44-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org45-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org46-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org47-ud-1-grp-1_ft1-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org48-ud-1-grp-1_ft1-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org49-ud-1-grp-1_ft3-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org50-ud-1-grp-1_ft0-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org51-ud-1-grp-1_ft1-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org52-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org53-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org54-ud-1-grp-1_ft6-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org55-ud-1-grp-1_ft4-gt1.trc"] = 1;
  map3["/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org56-ud-1-grp-1_ft5-gt1.trc"] = 1;
  
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org26-ud-1-grp-1_ft2-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org37-ud-1-grp-1_ft6-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org50-ud-1-grp-1_ft0-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org0-ud-1-grp-1_ft6-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org1-ud-1-grp-1_ft6-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org2-ud-1-grp-1_ft3-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org3-ud-1-grp-1_ft6-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org4-ud-1-grp-1_ft5-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org5-ud-1-grp-1_ft3-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org6-ud-1-grp-1_ft1-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org7-ud-1-grp-1_ft6-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org8-ud-1-grp-1_ft0-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org9-ud-1-grp-1_ft5-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org10-ud-1-grp-1_ft4-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org11-ud-1-grp-1_ft5-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org12-ud-1-grp-1_ft5-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org13-ud-1-grp-1_ft6-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org14-ud-1-grp-1_ft4-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org15-ud-1-grp-1_ft5-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org16-ud-1-grp-1_ft4-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org17-ud-1-grp-1_ft1-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org18-ud-1-grp-1_ft1-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org19-ud-1-grp-1_ft1-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org20-ud-1-grp-1_ft6-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org21-ud-1-grp-1_ft3-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org22-ud-1-grp-1_ft3-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org23-ud-1-grp-1_ft5-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org24-ud-1-grp-1_ft3-gt1.trc");
  map3.Remove("/Users/apwagner/current/experiment_files/niche/GP8/traces/crash_test/data//minitraces/org25-ud-1-grp-1_ft3-gt1.trc");
  
}


TEST(CoreHashBTreeMap, Comparison) {
  Apto::Map<int, int, Apto::DefaultHashBTree> map1;
  for (int i = 0; i < 4; i++) map1[i] = i;
  EXPECT_TRUE(map1 == map1);
  EXPECT_FALSE(map1 != map1);
  
  Apto::Map<int, int, Apto::DefaultHashBTree> map2;
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
  
  Apto::Map<int, int, Apto::DefaultHashBTree> map3(map1);
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
  Apto::Map<int, int, Apto::DefaultHashBTree> map1;
  for (int i = 0; i < 4; i++) map1[i] = i;

  Apto::Array<int> key_array(map1.GetSize());
  Apto::Array<int> value_array(map1.GetSize());
  Apto::Map<int, int, Apto::DefaultHashBTree>::Iterator it = map1.Begin();
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
  
  int new_entry = map1.GetSize();
  map1[new_entry] = new_entry;
  EXPECT_EQ(map1.GetSize() - 1, map1[map1.GetSize() - 1]);
  
  key_array.Resize(map1.GetSize());
  Apto::Map<int, int, Apto::DefaultHashBTree>::KeyIterator kit = map1.Keys();
  for (int i = 0; kit.Next(); i++) key_array[i] = *kit.Get();
  QSort(key_array);
  for (int i = 0; i < map1.GetSize(); i++) EXPECT_EQ(i, key_array[i]);
  
  value_array.Resize(map1.GetSize());
  Apto::Map<int, int, Apto::DefaultHashBTree>::ValueIterator vit = map1.Values();
  for (int i = 0; vit.Next(); i++) value_array[i] = *vit.Get();
  QSort(value_array);
  for (int i = 0; i < map1.GetSize(); i++) EXPECT_EQ(i, value_array[i]);
}
