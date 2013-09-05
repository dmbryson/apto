/*
 *  unittests/core/Matrix.cc
 *  Apto
 *
 *  Created by David on 9/5/13.
 *  Copyright 2013 David Michael Bryson. All rights reserved.
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

#include "apto/core/Matrix.h"

#include "gtest/gtest.h"


// Matrix<int, ArrayMatrix>
// --------------------------------------------------------------------------------------------------------------

TEST(CoreArrayMatrixMatrix, Construction) {
  Apto::Matrix<int, Apto::ArrayMatrix> default_constructor;
  EXPECT_EQ(0, default_constructor.NumRows());
  EXPECT_EQ(0, default_constructor.NumCols());
  Apto::Matrix<int, Apto::ArrayMatrix> constructor_sz_3_4(3, 4);
  EXPECT_EQ(3, constructor_sz_3_4.NumRows());
  EXPECT_EQ(4, constructor_sz_3_4.NumCols());
}

TEST(CoreArrayMatrixMatrix, Indexing) {
  Apto::Matrix<int, Apto::ArrayMatrix> matrix(5, 5);
  for (int r = 0; r < matrix.NumRows(); r++) {
    for (int c = 0; c < matrix.NumCols(); c++) {
      matrix.ElementAt(r, c) = r * c;
    }
  }
  
  EXPECT_EQ(0, matrix.ElementAt(0, 0));
  EXPECT_EQ(1, matrix.ElementAt(1, 1));
  EXPECT_EQ(4, matrix.ElementAt(2, 2));
  EXPECT_EQ(9, matrix.ElementAt(3, 3));
  EXPECT_EQ(16, matrix.ElementAt(4, 4));
  EXPECT_EQ(0, matrix.Row(0)[0]);
  EXPECT_EQ(1, matrix.Row(1)[1]);
  EXPECT_EQ(4, matrix[2][2]);
  EXPECT_EQ(9, matrix[3][3]);
  EXPECT_EQ(16, matrix[4][4]);
  
  matrix.ElementAt(3, 3) = 12;
  
  EXPECT_EQ(12, matrix.ElementAt(3, 3));
  EXPECT_EQ(12, matrix[3][3]);
}

TEST(CoreArrayMatrixMatrix, Assignment) {
  Apto::Matrix<int, Apto::ArrayMatrix> matrix1(5, 5);
  for (int r = 0; r < matrix1.NumRows(); r++) {
    for (int c = 0; c < matrix1.NumCols(); c++) {
      matrix1.ElementAt(r, c) = r * c;
    }
  }
  
  Apto::Matrix<int, Apto::ArrayMatrix> matrix2(4, 5);
  for (int r = 0; r < matrix2.NumRows(); r++) {
    for (int c = 0; c < matrix2.NumCols(); c++) {
      matrix2.ElementAt(r, c) = r * c + 5;
    }
  }
  
  EXPECT_NE(matrix1.NumRows(), matrix2.NumRows());
  EXPECT_NE(matrix1.ElementAt(3,3), matrix2.ElementAt(3,3));
  
  matrix1 = matrix2;
  
  EXPECT_EQ(matrix1.NumRows(), matrix2.NumRows());
  EXPECT_EQ(matrix1.ElementAt(3,3), matrix2.ElementAt(3,3));
  
  Apto::Matrix<int, Apto::ArrayMatrix> matrix_copy_constructor(matrix2);
  EXPECT_EQ(matrix2.NumRows(), matrix_copy_constructor.NumRows());
  EXPECT_EQ(matrix1.ElementAt(3,3), matrix2.ElementAt(3,3));
}
