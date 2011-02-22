/*
 *  unittests/stat/Functions.cc
 *  Apto
 *
 *  Created by David on 2/22/11.
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
#include "apto/stat/Functions.h"

#include "gtest/gtest.h"

#include <iostream>


TEST(Functions, FishersExact) {
  Apto::Stat::ContingencyTable t22(2,2);
  t22[0][0] = 2;
  t22[0][1] = 2;
  t22[1][0] = 4;
  t22[1][1] = 0;
  
  double pvalue = Apto::Stat::FishersExact(t22);
  std::cout << "pre = " << pvalue << std::endl;

  Apto::Stat::ContingencyTable t29(2,9);
  t29[0][0] = 1;
  t29[0][1] = 1;
  t29[0][2] = 1;
  t29[0][3] = 0;
  t29[0][4] = 0;
  t29[0][5] = 0;
  t29[0][6] = 1;
  t29[0][7] = 3;
  t29[0][8] = 3;
  
  t29[1][0] = 4;
  t29[1][1] = 4;
  t29[1][2] = 4;
  t29[1][3] = 4;
  t29[1][4] = 4;
  t29[1][5] = 4;
  t29[1][6] = 4;
  t29[1][7] = 1;
  t29[1][8] = 1;
  
  pvalue = Apto::Stat::FishersExact(t29);
  std::cout << "pre = " << pvalue << std::endl;
}