/*
 *  unittests/stat/ContingencyTable.cc
 *  Apto
 *
 *  Created by David on 2/15/11.
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

#include "apto/stat/ContingencyTable.h"

#include "gtest/gtest.h"


TEST(StatContingencyTable, Construction) {
  Apto::Stat::ContingencyTable t1(3, 3);
  EXPECT_EQ(3, t1.NumRows());
  EXPECT_EQ(3, t1.NumCols());
  
  Apto::Stat::ContingencyTable t2(2, 4);
  EXPECT_EQ(2, t2.NumRows());
  EXPECT_EQ(4, t2.NumCols());
}

TEST(StatContingencyTable, Indexing) {
  Apto::Stat::ContingencyTable t1(3, 3);
  const Apto::Stat::ContingencyTable& t2 = t1;

  for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) {
    t1[i][j] = i + (10 - j);
    EXPECT_EQ(i + (10 - j), t1[i][j]);
    EXPECT_EQ(i + (10 - j), t2[i][j]);

    t1.SetElement(i, j, i + (20 - j));
    EXPECT_EQ(i + (20 - j), t1[i][j]);
    EXPECT_EQ(i + (20 - j), t2[i][j]);
    EXPECT_EQ(i + (20 - j), t1.ElementAt(i, j));
    EXPECT_EQ(i + (20 - j), t2.ElementAt(i, j));
    
    t1.ElementAt(i, j) = i + (10 - j);
    EXPECT_EQ(i + (10 - j), t1[i][j]);
    EXPECT_EQ(i + (10 - j), t2[i][j]);
    EXPECT_EQ(i + (10 - j), t1.ElementAt(i, j));
    EXPECT_EQ(i + (10 - j), t2.ElementAt(i, j)); 
  }  
  
  int rowt = 0;
  for (int j = 0; j < 3; j++) rowt += t1[2][j];
  EXPECT_EQ(rowt, t1.RowMarginals()[2]);

  int colt = 0;
  for (int i = 0; i < 3; i++) colt += t1[i][1];
  EXPECT_EQ(colt, t1.ColMarginals()[1]);
  
  int totr = 0;
  int totc = 0;
  for (int i = 0; i < 3; i++) {
    totr += t1.RowMarginals()[i];
    totc += t1.ColMarginals()[i];
  }
  EXPECT_EQ(totr, t1.MarginalTotal());
  EXPECT_EQ(totc, t1.MarginalTotal());
  
  t1[1][1] = 115;
  EXPECT_EQ(115, t1[1][1]);

  t1[1][1] = 5;
  t1[1][1] += 115;
  EXPECT_EQ(120, t1[1][1]);

  t1[1][1] = 5;
  t1[1][1] -= 115;
  EXPECT_EQ(110, t1[1][1]);

  t1[1][1] = 5;
  EXPECT_EQ(5, t1[1][1]++);
  EXPECT_EQ(6, t1[1][1]);
  
  t1[1][1] = 5;
  EXPECT_EQ(5, t1[1][1]--);
  EXPECT_EQ(4, t1[1][1]);
  
  t1[1][1] = 5;
  EXPECT_EQ(6, ++t1[1][1]);
  
  t1[1][1] = 5;
  EXPECT_EQ(4, --t1[1][1]);
}

TEST(StatContingencyTable, Assignment) {
  Apto::Stat::ContingencyTable t1(3, 3);
  for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) t1[i][j] = i + (10 - j);

  Apto::Stat::ContingencyTable t2(2, 4);
  for (int i = 0; i < 2; i++) for (int j = 0; j < 4; j++) t2[i][j] = i + (10 - j);
  
  t2 = t1;
  EXPECT_EQ(3, t2.NumRows());
  EXPECT_EQ(3, t2.NumCols());
  for (int i = 0; i < t2.NumRows(); i++) for (int j = 0; j < t2.NumCols(); j++) EXPECT_EQ(i + (10 - j), t2[i][j]);

  Apto::Stat::ContingencyTable t3(t1);
  EXPECT_EQ(3, t3.NumRows());
  EXPECT_EQ(3, t3.NumCols());
  for (int i = 0; i < t3.NumRows(); i++) for (int j = 0; j < t3.NumCols(); j++) EXPECT_EQ(i + (10 - j), t3[i][j]);  
}

