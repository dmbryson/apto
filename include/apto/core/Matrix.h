/*
 *  Matrix.h
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

#ifndef AptoCoreMatrix_h
#define AptoCoreMatrix_h

#include "apto/core/Definitions.h"
#include "apto/core/MatrixStorage.h"


namespace Apto {
  
  // Matrix
  // --------------------------------------------------------------------------------------------------------------
  
  template <class T, template <class> class StoragePolicy = ArrayMatrix>
  class Matrix : public StoragePolicy<T>
  {
    typedef StoragePolicy<T> SP;
    
  public:
    typedef T ValueType;


  public:
    inline explicit Matrix(SizeType rows = 0, SizeType cols = 0) : SP(rows, cols) { ; }
    
    template <typename T1, template <class> class SP1>
    inline explicit Matrix(const Matrix<T1, SP1>& rhs) : SP(rhs.NumRows(), rhs.NumCols()) { this->operator=(rhs); }
    
    ~Matrix() { ; }
    
    Matrix& operator=(const Matrix& rhs) { SP::operator=(rhs); return *this; }

    template <typename T1, template <class> class SP1>
    Matrix& operator=(const Matrix<T1, SP1>& rhs)
    {
      if (SP::NumRows() != rhs.NumRows() || SP::NumCols() != rhs.NumCols()) SP::Resize(rhs.NumRows(), rhs.NumCols());
      for (SizeType r = 0; r < SP::NumRows(); r++) {
        for (SizeType c = 0; c < SP::NumCols(); c++) SP::ElementAt(r, c) = rhs.ElementAt(r, c);
      }
      return *this;
    }

    
    inline SizeType NumRows() const { return SP::NumRows(); }
    inline SizeType NumCols() const { return SP::NumCols(); }
    
    inline void ResizeClear(const SizeType in_size)
    {
      assert(in_size >= 0);
      SP::ResizeClear(in_size);
    }
    
    inline T& ElementAt(SizeType r, SizeType c)
    {
      assert(r >= 0);
      assert(c >= 0);
      assert(r < SP::NumRows());
      assert(c < SP::NumCols());
      return SP::ElementAt(r, c);
    }
    inline const T& ElementAt(SizeType r, SizeType c) const
    {
      assert(r >= 0);
      assert(c >= 0);
      assert(r < SP::NumRows());
      assert(c < SP::NumCols());
      return SP::ElementAt(r, c);
    }
    
    inline void SetAll(const T& value)
    {
      for (SizeType r = 0; r < SP::NumRows(); r++) {
        for (SizeType c = 0; c < SP::NumCols(); c++) SP::ElementAt(r, c) = value;
      }
    }
  };
  
};

#endif
