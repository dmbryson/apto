/*
 *  utils/fexact/main.cc
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
 *  Utility Front End for Fishers Exact
 */

#include "apto/stat/ContingencyTable.h"
#include "apto/stat/Functions.h"

#include <iostream>


int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;
  
  double pvalue = 0.0;
//  Apto::Stat::ContingencyTable t1(2,2);
//  t1[0][0] = 2; t1[0][1] = 2;
//  t1[1][0] = 4; t1[1][1] = 0;
//  pvalue = Apto::Stat::FishersExact(t1);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t2(2,9);
//  t2[0][0] = 1; t2[0][1] = 1; t2[0][2] = 1; t2[0][3] = 0; t2[0][4] = 0; t2[0][5] = 0; t2[0][6] = 1; t2[0][7] = 3; t2[0][8] = 3;  
//  t2[1][0] = 4; t2[1][1] = 4; t2[1][2] = 4; t2[1][3] = 4; t2[1][4] = 4; t2[1][5] = 4; t2[1][6] = 4; t2[1][7] = 1; t2[1][8] = 1;
//  pvalue = Apto::Stat::FishersExact(t2);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t3(4,5);
//  t3[0][0] = 2; t3[0][1] = 0; t3[0][2] = 1; t3[0][3] = 2; t3[0][4] = 6;
//  t3[1][0] = 1; t3[1][1] = 3; t3[1][2] = 1; t3[1][3] = 1; t3[1][4] = 1;
//  t3[2][0] = 1; t3[2][1] = 0; t3[2][2] = 3; t3[2][3] = 1; t3[2][4] = 0;
//  t3[3][0] = 1; t3[3][1] = 2; t3[3][2] = 1; t3[3][3] = 2; t3[3][4] = 0;
//  pvalue = Apto::Stat::FishersExact(t3);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t4(4,6);
//  t4[0][0] = 2; t4[0][1] = 0; t4[0][2] = 1; t4[0][3] = 2; t4[0][4] = 6; t4[0][5] = 5;
//  t4[1][0] = 1; t4[1][1] = 3; t4[1][2] = 1; t4[1][3] = 1; t4[1][4] = 1; t4[1][5] = 2;
//  t4[2][0] = 1; t4[2][1] = 0; t4[2][2] = 3; t4[2][3] = 1; t4[2][4] = 0; t4[2][5] = 0;
//  t4[3][0] = 1; t4[3][1] = 2; t4[3][2] = 1; t4[3][3] = 2; t4[3][4] = 0; t4[3][5] = 0;
//  pvalue = Apto::Stat::FishersExact(t4);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t5(3,9);
//  t5[0][0] = 1; t5[0][1] = 1; t5[0][2] = 1; t5[0][3] = 0; t5[0][4] = 0; t5[0][5] = 0; t5[0][6] = 1; t5[0][7] = 2; t5[0][8] = 4;  
//  t5[1][0] = 4; t5[1][1] = 4; t5[1][2] = 4; t5[1][3] = 5; t5[1][4] = 5; t5[1][5] = 5; t5[1][6] = 6; t5[1][7] = 5; t5[1][8] = 0;
//  t5[2][0] = 1; t5[2][1] = 1; t5[2][2] = 1; t5[2][3] = 0; t5[2][4] = 0; t5[2][5] = 0; t5[2][6] = 1; t5[2][7] = 2; t5[2][8] = 4;
//  pvalue = Apto::Stat::FishersExact(t5);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t6(5,6);
//  t6[0][0] = 1; t6[0][1] = 2; t6[0][2] = 2; t6[0][3] = 1; t6[0][4] = 1; t6[0][5] = 0;
//  t6[1][0] = 2; t6[1][1] = 0; t6[1][2] = 0; t6[1][3] = 2; t6[1][4] = 3; t6[1][5] = 0;
//  t6[2][0] = 0; t6[2][1] = 1; t6[2][2] = 1; t6[2][3] = 1; t6[2][4] = 2; t6[2][5] = 7;
//  t6[3][0] = 1; t6[3][1] = 1; t6[3][2] = 2; t6[3][3] = 0; t6[3][4] = 0; t6[3][5] = 0;
//  t6[4][0] = 0; t6[4][1] = 1; t6[4][2] = 1; t6[4][3] = 1; t6[4][4] = 1; t6[4][5] = 0;
//  pvalue = Apto::Stat::FishersExact(t6);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t7(5,7);
//  t7[0][0] = 1; t7[0][1] = 2; t7[0][2] = 2; t7[0][3] = 1; t7[0][4] = 1; t7[0][5] = 0; t7[0][6] = 1;
//  t7[1][0] = 2; t7[1][1] = 0; t7[1][2] = 0; t7[1][3] = 2; t7[1][4] = 3; t7[1][5] = 0; t7[1][6] = 0;
//  t7[2][0] = 0; t7[2][1] = 1; t7[2][2] = 1; t7[2][3] = 1; t7[2][4] = 2; t7[2][5] = 7; t7[2][6] = 3;
//  t7[3][0] = 1; t7[3][1] = 1; t7[3][2] = 2; t7[3][3] = 0; t7[3][4] = 0; t7[3][5] = 0; t7[3][6] = 1;
//  t7[4][0] = 0; t7[4][1] = 1; t7[4][2] = 1; t7[4][3] = 1; t7[4][4] = 1; t7[4][5] = 0; t7[4][6] = 0;
//  pvalue = Apto::Stat::FishersExact(t7);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t8(3,3);
//  t8[0][0] = 2; t8[0][1] = 4; t8[0][2] = 6;
//  t8[1][0] = 7; t8[1][1] = 6; t8[1][2] = 1;
//  t8[2][0] = 5; t8[2][1] = 0; t8[2][2] = 0;
//  pvalue = Apto::Stat::FishersExact(t8);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t9(2,8);
//  t9[0][0] = 22; t9[0][1] = 13; t9[0][2] = 5; t9[0][3] = 4; t9[0][4] = 5; t9[0][5] = 3; t9[0][6] = 2; t9[0][7] = 1;
//  t9[1][0] =  7; t9[1][1] =  1; t9[1][2] = 4; t9[1][3] = 3; t9[1][4] = 1; t9[1][5] = 2; t9[1][6] = 3; t9[1][7] = 4;
//  pvalue = Apto::Stat::FishersExact(t9);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t10(2,5);
//  t10[0][0] = 2; t10[0][1] = 3; t10[0][2] =  4; t10[0][3] =  8; t10[0][4] =  9;
//  t10[1][0] = 0; t10[1][1] = 0; t10[1][2] = 11; t10[1][3] = 10; t10[1][4] = 11;
//  pvalue = Apto::Stat::FishersExact(t10);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t11(3,5);
//  t11[0][0] = 24; t11[0][1] = 7; t11[0][2] = 3; t11[0][3] = 8; t11[0][4] = 1;
//  t11[1][0] =  9; t11[1][1] = 5; t11[1][2] = 5; t11[1][3] = 0; t11[1][4] = 3;
//  t11[2][0] =  2; t11[2][1] = 0; t11[2][2] = 2; t11[2][3] = 0; t11[2][4] = 1;
//  pvalue = Apto::Stat::FishersExact(t11);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t12(2,10);
//  t12[0][0] = 20; t12[0][1] = 3; t12[0][2] = 6; t12[0][3] = 4; t12[0][4] = 7; t12[0][5] = 6; t12[0][6] = 6; t12[0][7] = 2; t12[0][8] = 2; t12[0][9] = 2;  
//  t12[1][0] =  8; t12[1][1] = 8; t12[1][2] = 4; t12[1][3] = 5; t12[1][4] = 2; t12[1][5] = 1; t12[1][6] = 0; t12[1][7] = 2; t12[1][8] = 1; t12[1][9] = 1;
//  pvalue = Apto::Stat::FishersExact(t12);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t13(3,7);
//  t13[0][0] =  1; t13[0][1] = 8; t13[0][2] = 5; t13[0][3] = 4; t13[0][4] = 4; t13[0][5] = 2; t13[0][6] = 2;
//  t13[1][0] =  5; t13[1][1] = 3; t13[1][2] = 3; t13[1][3] = 4; t13[1][4] = 3; t13[1][5] = 1; t13[1][6] = 0;
//  t13[2][0] = 10; t13[2][1] = 1; t13[2][2] = 4; t13[2][3] = 0; t13[2][4] = 0; t13[2][5] = 0; t13[2][6] = 0;
//  pvalue = Apto::Stat::FishersExact(t13);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t14(4,5);
//  t14[0][0] = 9; t14[0][1] = 1; t14[0][2] = 3; t14[0][3] = 1; t14[0][4] = 3;
//  t14[1][0] = 3; t14[1][1] = 3; t14[1][2] = 2; t14[1][3] = 1; t14[1][4] = 3;
//  t14[2][0] = 8; t14[2][1] = 2; t14[2][2] = 4; t14[2][3] = 2; t14[2][4] = 0;
//  t14[3][0] = 4; t14[3][1] = 9; t14[3][2] = 2; t14[3][3] = 0; t14[3][4] = 0;
//  pvalue = Apto::Stat::FishersExact(t14);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t15(3,6);
//  t15[0][0] = 12; t15[0][1] =  6; t15[0][2] = 12; t15[0][3] = 1; t15[0][4] = 1; t15[0][5] = 0;
//  t15[1][0] =  5; t15[1][1] = 12; t15[1][2] =  4; t15[1][3] = 4; t15[1][4] = 0; t15[1][5] = 1;
//  t15[2][0] =  5; t15[2][1] = 12; t15[2][2] = 10; t15[2][3] = 1; t15[2][4] = 1; t15[2][5] = 0;
//  pvalue = Apto::Stat::FishersExact(t15);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//  
//  Apto::Stat::ContingencyTable t16(3,8);
//  t16[0][0] = 20; t16[0][1] = 20; t16[0][2] = 2; t16[0][3] = 2; t16[0][4] = 0; t16[0][5] = 0; t16[0][6] = 0; t16[0][7] = 1;
//  t16[1][0] = 10; t16[1][1] = 10; t16[1][2] = 1; t16[1][3] = 1; t16[1][4] = 3; t16[1][5] = 3; t16[1][6] = 1; t16[1][7] = 0;
//  t16[2][0] = 20; t16[2][1] = 20; t16[2][2] = 2; t16[2][3] = 2; t16[2][4] = 0; t16[2][5] = 0; t16[2][6] = 0; t16[2][7] = 1;
//  pvalue = Apto::Stat::FishersExact(t16);
//  std::cout << "pre = " << pvalue << std::endl;
//  
//
//  Apto::Stat::ContingencyTable t17(19,2);
//  t17[ 0][0] =  3; t17[ 0][1] =  7;
//  t17[ 1][0] =  1; t17[ 1][1] =  9;
//  t17[ 2][0] = 10; t17[ 2][1] =  0;
//  t17[ 3][0] =  0; t17[ 3][1] = 10;
//  t17[ 4][0] =  4; t17[ 4][1] =  6;
//  t17[ 5][0] =  3; t17[ 5][1] =  7;
//  t17[ 6][0] =  3; t17[ 6][1] =  7;
//  t17[ 7][0] =  2; t17[ 7][1] =  8;
//  t17[ 8][0] =  0; t17[ 8][1] = 10;
//  t17[ 9][0] =  2; t17[ 9][1] =  8;
//  t17[10][0] =  0; t17[10][1] = 10;
//  t17[11][0] =  2; t17[11][1] =  8;
//  t17[12][0] =  1; t17[12][1] =  9;
//  t17[13][0] =  1; t17[13][1] =  9;
//  t17[14][0] =  6; t17[14][1] =  4;
//  t17[15][0] = 10; t17[15][1] =  0;
//  t17[16][0] =  5; t17[16][1] =  5;
//  t17[17][0] =  7; t17[17][1] =  3;
//  t17[18][0] =  2; t17[18][1] =  8;
//  pvalue = Apto::Stat::FishersExact(t17);
//  std::cout << "pre = " << pvalue << std::endl;


//  Apto::Stat::ContingencyTable t18(42,2);
//  t18[ 0][0] =  5; t18[ 0][1] =  5;
//  t18[ 1][0] =  0; t18[ 1][1] = 10;
//  t18[ 2][0] =  1; t18[ 2][1] =  9;
//  t18[ 3][0] =  1; t18[ 3][1] =  9;
//  t18[ 4][0] =  1; t18[ 4][1] =  9;
//  t18[ 5][0] =  1; t18[ 5][1] =  9;
//  t18[ 6][0] =  5; t18[ 6][1] =  5;
//  t18[ 7][0] =  6; t18[ 7][1] =  4;
//  t18[ 8][0] =  3; t18[ 8][1] =  7;
//  t18[ 9][0] =  3; t18[ 9][1] =  7;
//  t18[10][0] =  0; t18[10][1] = 10;
//  t18[11][0] =  6; t18[11][1] =  4;
//  t18[12][0] =  7; t18[12][1] =  3;
//  t18[13][0] =  4; t18[13][1] =  6;
//  t18[14][0] =  0; t18[14][1] = 10;
//  t18[15][0] =  1; t18[15][1] =  9;
//  t18[16][0] =  0; t18[16][1] = 10;
//  t18[17][0] =  0; t18[17][1] = 10;
//  t18[18][0] =  3; t18[18][1] =  7;
//  t18[19][0] =  9; t18[19][1] =  1;
//  t18[20][0] =  5; t18[20][1] =  5;
//  t18[21][0] =  1; t18[21][1] =  9;
//  t18[22][0] =  0; t18[22][1] = 10;
//  t18[23][0] =  0; t18[23][1] = 10;
//  t18[24][0] =  3; t18[24][1] =  7;
//  t18[25][0] =  9; t18[25][1] =  1;
//  t18[26][0] =  8; t18[26][1] =  2;
//  t18[27][0] =  7; t18[27][1] =  3;
//  t18[28][0] = 10; t18[28][1] =  0;
//  t18[29][0] =  0; t18[29][1] = 10;
//  t18[30][0] =  2; t18[30][1] =  8;
//  t18[31][0] =  2; t18[31][1] =  8;
//  t18[32][0] =  0; t18[32][1] = 10;
//  t18[33][0] =  0; t18[33][1] = 10;
//  t18[34][0] =  1; t18[34][1] =  9;
//  t18[35][0] =  2; t18[35][1] =  8;
//  t18[36][0] =  7; t18[36][1] =  3;
//  t18[37][0] =  3; t18[37][1] =  7;
//  t18[38][0] =  2; t18[38][1] =  8;
//  t18[39][0] =  3; t18[39][1] =  7;
//  t18[40][0] =  0; t18[40][1] = 10;
//  t18[41][0] =  9; t18[41][1] =  1;
//  pvalue = Apto::Stat::FishersExact(t18);
//  std::cout << "pre = " << pvalue << std::endl;


//  Apto::Stat::ContingencyTable t18(100,2);
//  t18[ 0][0] = 10; t18[ 0][1] =  0;
//  t18[ 1][0] =  2; t18[ 1][1] =  8;
//  t18[ 2][0] =  2; t18[ 2][1] =  8;
//  t18[ 3][0] =  2; t18[ 3][1] =  8;
//  t18[ 4][0] =  0; t18[ 4][1] = 10;
//  t18[ 5][0] =  4; t18[ 5][1] =  6;
//  t18[ 6][0] =  8; t18[ 6][1] =  2;
//  t18[ 7][0] =  9; t18[ 7][1] =  1;
//  t18[ 8][0] = 10; t18[ 8][1] =  0;
//  t18[ 9][0] = 10; t18[ 9][1] =  0;
//  t18[10][0] =  2; t18[10][1] =  8;
//  t18[11][0] = 10; t18[11][1] =  0;
//  t18[12][0] =  2; t18[12][1] =  8;
//  t18[13][0] = 10; t18[13][1] =  0;
//  t18[14][0] = 10; t18[14][1] =  0;
//  t18[15][0] =  4; t18[15][1] =  6;
//  t18[16][0] =  8; t18[16][1] =  2;
//  t18[17][0] =  9; t18[17][1] =  1;
//  t18[18][0] =  5; t18[18][1] =  5;
//  t18[19][0] =  7; t18[19][1] =  3;
//  t18[20][0] =  3; t18[20][1] =  7;
//  t18[21][0] =  5; t18[21][1] =  5;
//  t18[22][0] =  7; t18[22][1] =  3;
//  t18[23][0] =  2; t18[23][1] =  8;
//  t18[24][0] =  4; t18[24][1] =  6;
//  t18[25][0] = 10; t18[25][1] =  0;
//  t18[26][0] = 10; t18[26][1] =  0;
//  t18[27][0] =  2; t18[27][1] =  8;
//  t18[28][0] =  4; t18[28][1] =  6;
//  t18[29][0] =  9; t18[29][1] =  1;
//  t18[30][0] =  4; t18[30][1] =  6;
//  t18[31][0] = 10; t18[31][1] =  0;
//  t18[32][0] = 10; t18[32][1] =  0;
//  t18[33][0] = 10; t18[33][1] =  0;
//  t18[34][0] =  2; t18[34][1] =  8;
//  t18[35][0] =  2; t18[35][1] =  8;
//  t18[36][0] =  1; t18[36][1] =  9;
//  t18[37][0] =  8; t18[37][1] =  2;
//  t18[38][0] = 10; t18[38][1] =  0;
//  t18[39][0] = 10; t18[39][1] =  0;
//  t18[40][0] =  2; t18[40][1] =  8;
//  t18[41][0] = 10; t18[41][1] =  0;
//  t18[42][0] =  4; t18[42][1] =  6;
//  t18[43][0] =  8; t18[43][1] =  2;
//  t18[44][0] = 10; t18[44][1] =  0;
//  t18[45][0] =  7; t18[45][1] =  3;
//  t18[46][0] =  0; t18[46][1] = 10;
//  t18[47][0] =  8; t18[47][1] =  2;
//  t18[48][0] = 10; t18[48][1] =  0;
//  t18[49][0] =  1; t18[49][1] =  9;
//  t18[50][0] = 10; t18[50][1] =  0;
//  t18[51][0] = 10; t18[51][1] =  0;
//  t18[52][0] =  8; t18[52][1] =  2;
//  t18[53][0] =  2; t18[53][1] =  8;
//  t18[54][0] =  7; t18[54][1] =  3;
//  t18[55][0] =  9; t18[55][1] =  1;
//  t18[56][0] =  5; t18[56][1] =  5;
//  t18[57][0] = 10; t18[57][1] =  0;
//  t18[58][0] =  1; t18[58][1] =  9;
//  t18[59][0] =  3; t18[59][1] =  7;
//  t18[60][0] =  2; t18[60][1] =  8;
//  t18[61][0] =  6; t18[61][1] =  4;
//  t18[62][0] =  1; t18[62][1] =  9;
//  t18[63][0] =  7; t18[63][1] =  3;
//  t18[64][0] =  1; t18[64][1] =  9;
//  t18[65][0] =  7; t18[65][1] =  3;
//  t18[66][0] =  7; t18[66][1] =  3;
//  t18[67][0] = 10; t18[67][1] =  0;
//  t18[68][0] = 10; t18[68][1] =  0;
//  t18[69][0] = 10; t18[69][1] =  0;
//  t18[70][0] =  8; t18[70][1] =  2;
//  t18[71][0] = 10; t18[71][1] =  0;
//  t18[72][0] =  0; t18[72][1] = 10;
//  t18[73][0] =  0; t18[73][1] = 10;
//  t18[74][0] =  7; t18[74][1] =  3;
//  t18[75][0] =  9; t18[75][1] =  1;
//  t18[76][0] =  0; t18[76][1] = 10;
//  t18[77][0] = 10; t18[77][1] =  0;
//  t18[78][0] =  1; t18[78][1] =  9;
//  t18[79][0] =  2; t18[79][1] =  8;
//  t18[80][0] =  1; t18[80][1] =  9;
//  t18[81][0] =  4; t18[81][1] =  6;
//  t18[82][0] =  7; t18[82][1] =  3;
//  t18[83][0] = 10; t18[83][1] =  0;
//  t18[84][0] = 10; t18[84][1] =  0;
//  t18[85][0] =  6; t18[85][1] =  4;
//  t18[86][0] =  8; t18[86][1] =  2;
//  t18[87][0] =  1; t18[87][1] =  9;
//  t18[88][0] =  1; t18[88][1] =  9;
//  t18[89][0] =  2; t18[89][1] =  8;
//  t18[90][0] =  9; t18[90][1] =  1;
//  t18[91][0] =  0; t18[91][1] = 10;
//  t18[92][0] = 10; t18[92][1] =  0;
//  t18[93][0] =  9; t18[93][1] =  1;
//  t18[94][0] = 10; t18[94][1] =  0;
//  t18[95][0] =  7; t18[95][1] =  3;
//  t18[96][0] = 10; t18[96][1] =  0;
//  t18[97][0] = 10; t18[97][1] =  0;
//  t18[98][0] =  0; t18[98][1] = 10;
//  t18[99][0] =  2; t18[99][1] =  8;
//  pvalue = Apto::Stat::FishersExact(t18);
//  std::cout << "pre = " << pvalue << std::endl;
  
  std::cout << "1-step enter:" << std::endl;
  Apto::Stat::ContingencyTable t19(2,4);
  t19[0][0] = 3642; t19[0][1] = 2676; t19[0][2] = 52714; t19[0][3] = 0;
  t19[1][0] = 7; t19[1][1] = 11; t19[1][2] = 22; t19[1][3] = 0;
  pvalue = Apto::Stat::FishersExact(t19);
  std::cout << "pre = " << pvalue << std::endl;
  
  std::cout << "1-step leave:" << std::endl;
  t19[1][0] = 10; t19[1][1] = 8; t19[1][2] = 14; t19[1][3] = 0;
  pvalue = Apto::Stat::FishersExact(t19);
  std::cout << "pre = " << pvalue << std::endl;

  std::cout << "2-step enter:" << std::endl;
  t19[1][0] = 71; t19[1][1] = 25; t19[1][2] = 125; t19[1][3] = 0;
  pvalue = Apto::Stat::FishersExact(t19);
  std::cout << "pre = " << pvalue << std::endl;

  std::cout << "2-step leave:" << std::endl;
  t19[1][0] = 43; t19[1][1] = 63; t19[1][2] = 107; t19[1][3] = 0;
  pvalue = Apto::Stat::FishersExact(t19);
  std::cout << "pre = " << pvalue << std::endl;


  std::cout << std::endl << "hc:" << std::endl;
  Apto::Stat::ContingencyTable t20(2,11);
  t20[0][0] = 0; t20[0][1] = 0; t20[0][2] =  1; t20[0][3] = 4; t20[0][4] = 11; t20[0][5] = 20; t20[0][6] = 25; t20[0][7] = 21; t20[0][8] = 12; t20[0][9] = 4; t20[0][10] =  1;
  t20[1][0] = 7; t20[1][1] = 9; t20[1][2] = 15; t20[1][3] = 2; t20[1][4] =  7; t20[1][5] =  3; t20[1][6] =  2; t20[1][7] = 10; t20[1][8] =  8; t20[1][9] = 7; t20[1][10] = 30;
  pvalue = Apto::Stat::FishersExact(t20);
  std::cout << "pre = " << pvalue << std::endl;
}
