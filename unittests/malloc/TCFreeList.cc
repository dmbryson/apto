/*
 *  unittests/malloc/TCFreeList.cc
 *  Apto
 *
 *  Created by David on 10/11/12.
 *  Copyright 2012 David Michael Bryson. All rights reserved.
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

#include "apto/core/Malloc.h"
#include "apto/malloc/FixedSegment.h"
#include "apto/malloc/TCFreeList.h"

#include "gtest/gtest.h"

TEST(MallocTCFreeList, FixedSegmentBasicMalloc) {
  using namespace Apto;
  
  struct SizeOneIntP { int* i; };
  
  typedef Malloc::FixedSegment<sizeof(SizeOneIntP), Malloc::TCFreeList<BasicMalloc>, BasicMalloc> DualBasicOneInt;

  class OneInt : public ClassAllocator<DualBasicOneInt>
  {
  public:
    int i;
  };
  
  class OneIntP : public ClassAllocator<DualBasicOneInt>
  {
  public:
    int* i;
  };

  class TwoInt : public ClassAllocator<DualBasicOneInt>
  {
  public:
    int i;
    int j;
  };

  class TwoIntP : public ClassAllocator<DualBasicOneInt>
  {
  public:
    int* i;
    int* j;
  };
  
  
  for (int i = 0; i < 10; i++) {
    OneInt* oi = new OneInt();
    EXPECT_TRUE(oi != NULL);
    oi->i = 2;
    EXPECT_EQ(oi->i, 2);
    delete oi;
  }
  
  for (int i = 0; i < 12; i++) {
    OneIntP* oip = new OneIntP();
    EXPECT_TRUE(oip != NULL);
    delete oip;
  }
  
  for (int i = 0; i < 14; i++) {
    TwoInt* ti = new TwoInt();
    EXPECT_TRUE(ti != NULL);
    delete ti;
  }
  
  for (int i = 0; i < 10; i++) {
    TwoIntP* tip = new TwoIntP();
    EXPECT_TRUE(tip != NULL);
    delete tip;
  }
}
