/*
 *  unittests/core/ArrayUtils.cc
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

#include "gtest/gtest.h"

#include <iostream>

TEST(CoreArrayUtils, ISort) {
  Apto::Array<int> a(5);
  a[0] = 3;
  a[1] = 1;
  a[2] = 5;
  a[3] = 4;
  a[4] = 2;
  
  Apto::ISort(a);
  for (int i = 0; i < a.GetSize(); i++) ASSERT_EQ(i + 1, a[i]);
}


TEST(CoreArrayUtils, QSort) {
  Apto::Array<int> a(20);
  a[ 0] = 3;
  a[ 1] = 1;
  a[ 2] = 5;
  a[ 3] = 4;
  a[ 4] = 13;
  a[ 5] = 8;
  a[ 6] = 12;
  a[ 7] = 19;
  a[ 8] = 20;
  a[ 9] = 18;
  a[10] = 10;
  a[11] = 2;
  a[12] = 16;
  a[13] = 6;
  a[14] = 11;
  a[15] = 7;
  a[16] = 17;
  a[17] = 14;
  a[18] = 9;
  a[19] = 15;
  
  
  Apto::QSort(a);
  for (int i = 0; i < a.GetSize(); i++) ASSERT_EQ(i + 1, a[i]);
}
