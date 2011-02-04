/*
 *  ArraySet.h
 *  Apto
 *
 *  Created by David on 11/18/09.
 *  Copyright 2009-2011 David Michael Bryson. All rights reserved.
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

#ifndef AptoCoreArraySet_h
#define AptoCoreArraySet_h

#include <cassert>


namespace Apto {  
  template <class T> class ArraySet
  {
  private:
    T* m_data;  // Data Elements
    int m_size; // Number of Elements
    
  public:
    ArraySet() : m_data(NULL), m_size(0) { ; }
    ArraySet(const ArraySet& rhs) : m_data(NULL), m_size(0) { this->operator=(rhs); }
    
    ~ArraySet() { delete [] m_data; }
        
    inline ArraySet& operator=(const ArraySet& rhs) {
      if (m_size != rhs.GetSize())  ResizeClear(rhs.GetSize());
      for(int i = 0; i < m_size; i++) m_data[i] = rhs[i];
      return *this;
    }
    
    inline ArraySet operator+(const ArraySet& in_array) const {
      ArraySet tmp;
      
      if (m_size >= in_array.m_size) {
        for(int i = 0; i < m_size; i++) tmp.m_data[i] = m_data[i];
        for(int i = 0; i < in_array.m_size; i++) tmp.Add(in_array[i]);
      } else {
        for(int i = 0; i < in_array.m_size; i++) tmp[i] = in_array[i];
        for(int i = 0; i < m_size; i++) tmp.Add(m_data[i]);      
      }
      
      return tmp;
    }
    
    inline int GetSize() const { return m_size; }
    
    
    inline const T& operator[](const int index) const
    {
      assert(index >= 0);     // Lower Bounds Error
      assert(index < m_size); // Upper Bounds Error
      return m_data[index];
    }    
    
    inline void Add(const T& value)
    {
      for (int i = 0; i < m_size; i++) if (m_data[i] == value) return;
      int osize = m_size++;
      T* new_data = new T[m_size];
      for (int i = 0; i < osize; i++) new_data[i] = m_data[i];
      new_data[osize] = value;
      delete [] m_data;
      m_data = new_data;
    }
    
    inline bool Has(const T& value) const
    {
      for (int i = 0; i < m_size; i++) if (m_data[i] == value) return true;    
      return false;
    }
    
    inline void Remove(const T& value)
    {
      int found = -1;
      for (int i = 0; i < m_size; i++) {
        if (m_data[i] == value) {
          found = i;
          break;
        }
      }
      
      if (found > -1) {
        int osize = m_size--;
        T* new_data = new T[m_size];
        for (int i = 0; i < found; i++) new_data[i] = m_data[i];
        for (int i = found + 1; i < osize; i++) new_data[i - 1] = m_data[i];
        delete [] m_data;
        m_data = new_data;
      }
    }
  };    
};

#endif
