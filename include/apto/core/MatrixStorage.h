/*
 *  MatrixStorage.h
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

#ifndef AptoCoreMatrixStorage_h
#define AptoCoreMatrixStorage_h

#include "apto/core/Array.h"


namespace Apto {
  
  // Matrix Storage Policies
  // --------------------------------------------------------------------------------------------------------------
  
  
  template <class T> class ArrayMatrix
  {
  protected:
    Array<Array<T, Basic>, ManagedPointer> m_data; // Data Matrix
    
    typedef T StoredType;
    
    explicit ArrayMatrix(SizeType rows = 0, SizeType cols = 0) : m_data(0) { ResizeClear(rows, cols); }
    ArrayMatrix(const ArrayMatrix& rhs) : m_data(0) { this->operator=(rhs); }
    ~ArrayMatrix() { ; }
    
    inline SizeType NumRows() const { return m_data.GetSize(); }
    inline SizeType NumCols() const { return (m_data.GetSize()) ? m_data[0].GetSize() : 0; }
    
    inline void ResizeClear(const SizeType rows, const SizeType cols)
    {
      m_data.Resize(rows);
      for (SizeType i = 0; i < rows; i++) m_data[i].ResizeClear(cols);
    }
    
    void Resize(const SizeType rows, const SizeType cols)
    {
      m_data.Resize(rows);
      for (SizeType i = 0; i < rows; i++) m_data[i].Resize(cols);
    }
    
    ArrayMatrix& operator=(const ArrayMatrix& rhs)
    {
      m_data.Resize(rhs.NumRows());
      for (SizeType i = 0; i < rhs.NumRows(); i++) m_data[i] = rhs.m_data[i];
      return *this;
    }
    
    inline T& ElementAt(SizeType r, SizeType c) { return m_data[r][c]; }
    inline const T& ElementAt(SizeType r, SizeType c) const { return m_data[r][c]; }

    
  public:
    inline const Array<T, Basic>& Row(const SizeType index) { return m_data[index]; }
    inline const Array<T, Basic>& Row(const SizeType index) const { return m_data[index]; }
    inline const Array<T, Basic>& operator[](const SizeType index) { return m_data[index]; }
    inline const Array<T, Basic>& operator[](const SizeType index) const { return m_data[index]; }
  };
  
};

#endif
