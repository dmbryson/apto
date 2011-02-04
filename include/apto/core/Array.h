/*
 *  Array.h
 *  Apto
 *
 *  Created by David on 2/4/11.
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

#ifndef AptoCoreArray_h
#define AptoCoreArray_h

#include <cassert>

namespace Apto {
  
  template <class T> class Array
  {
  private:
    
    T* m_data;    // Data Array
    int m_size;   // Array Size
    
  public:
    typedef T ValueType;
    
    explicit Array(int size = 0) : m_data(NULL), m_size(0) { ResizeClear(size); }
    Array(const Array& rhs) : m_data(NULL), m_size(0) { this->operator=(rhs); }
    
    ~Array() { delete [] m_data; }
    
    Array& operator=(const Array& rhs)
    {
      if (m_size != rhs.GetSize()) Resize(rhs.GetSize());
      for (int i = 0; i < m_size; i++) m_data[i] = rhs[i];
      return *this;
    }
    
    int GetSize() const { return m_size; }
    
    void ResizeClear(const int in_size)
    {
      assert(in_size >= 0);
      
      if (m_data != NULL) delete [] m_data;
      
      if (in_size > 0) {
        m_data = new T[in_size];  // Allocate block for data
        assert(m_data != NULL);   // Memory allocation error: Out of Memory?
      }
      else m_data = NULL;
      
      m_size = in_size;
    }
    
    void Resize(int new_size)
    {
      assert(new_size >= 0);
      
      // If we're already at the size we want, don't bother doing anything.
      if (new_size == m_size) return;
      
      // If new size is 0, clean up and go!
      if (new_size == 0) {
        if (m_data != NULL) delete [] m_data;
        m_data = NULL;
        m_size = 0;
        return;
      }
      
      T* new_data = new T[new_size];
      assert(new_data != NULL); // Memory Allocation Error: Out of Memory?
      
      // Copy over old data...
      for (int i = 0; i < m_size && i < new_size; i++) new_data[i] = m_data[i];
      if (m_data != NULL) delete [] m_data;  // remove old data if exists

      m_data = new_data;
      m_size = new_size;
    }
    
    void Resize(int new_size, const T& empty_value)
    {
      int old_size = m_size;
      Resize(new_size);
      for (int i = old_size; i < new_size; i++) m_data[i] = empty_value;
    }
    
    T& operator[](const int index)
    {
      assert(index >= 0);       // Lower Bounds Error
      assert(index < m_size); // Upper Bounds Error
      return m_data[index];
    }
    const T& operator[](const int index) const
    {
      assert(index >= 0);       // Lower Bounds Error
      assert(index < m_size); // Upper Bounds Error
      return m_data[index];
    }
    
    
    // Stack-like Methods...
    void Push(const T& value)
    {
      Resize(m_size + 1);
      m_data[m_size - 1] = value;
    }
    
    T Pop()
    {
      T value = m_data[m_size - 1];
      Resize(m_size - 1);
      return value;
    }
    
    
    void Swap(int idx1, int idx2)
    {
      assert(idx1 >= 0);      // Lower Bounds Error
      assert(idx1 < m_size);  // Upper Bounds Error
      assert(idx2 >= 0);      // Lower Bounds Error
      assert(idx2 < m_size);  // Upper Bounds Error
      
      T v = m_data[idx1];
      m_data[idx1] = m_data[idx2];
      m_data[idx2] = v;
    }  
    
    
    void SetAll(const T& value)
    {
      for (int i = 0; i < m_size; i++) m_data[i] = value;
    }
  };
  
};

#endif
