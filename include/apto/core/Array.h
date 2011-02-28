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

#include "apto/core/ArrayStorage.h"
#include "apto/core/Iterator.h"

#include <cassert>


namespace Apto {
  
  // Array
  // --------------------------------------------------------------------------------------------------------------  
  template <class T, template <class> class StoragePolicy = Basic>
  class Array : public StoragePolicy<T>
  {
    typedef StoragePolicy<T> SP;
    
  public:
    typedef T ValueType;
    
    inline explicit Array(int size = 0) : SP(size) { ; }
    
    template <typename T1, template <class> class SP1>
    inline explicit Array(const Array<T1, SP1>& rhs) : SP(rhs.GetSize()) { this->operator=(rhs); }
    
    ~Array() { ; }
    
    template <typename T1, template <class> class SP1>
    Array& operator=(const Array<T1, SP1>& rhs)
    {
      if (SP::GetSize() != rhs.GetSize()) SP::Resize(rhs.GetSize());
      for (int i = 0; i < GetSize(); i++) SP::operator[](i) = rhs[i];
      return *this;
    }
    
    inline int GetSize() const { return SP::GetSize(); }
    
    inline void ResizeClear(const int in_size)
    {
      assert(in_size >= 0);
      SP::ResizeClear(in_size);
    }
    
    inline void Resize(int new_size)
    {
      assert(new_size >= 0);
      SP::Resize(new_size);
    }
    
    void Resize(int new_size, const T& empty_value)
    {
      int old_size = SP::GetSize();
      SP::Resize(new_size);
      for (int i = old_size; i < new_size; i++) SP::operator[](i) = empty_value;
    }
    
    T& operator[](const int index)
    {
      assert(index >= 0);       // Lower Bounds Error
      assert(index < SP::GetSize()); // Upper Bounds Error
      return SP::operator[](index);
    }
    const T& operator[](const int index) const
    {
      assert(index >= 0);       // Lower Bounds Error
      assert(index < SP::GetSize()); // Upper Bounds Error
      return SP::operator[](index);
    }
    
    
    // Stack-like Methods...
    void Push(const T& value)
    {
      SP::Resize(SP::GetSize() + 1);
      SP::operator[](SP::GetSize() - 1) = value;
    }
    
    T Pop()
    {
      T value = SP::operator[](SP::GetSize() - 1);
      SP::Resize(SP::GetSize() - 1);
      return value;
    }
    
    
    void Swap(int idx1, int idx2)
    {
      assert(idx1 >= 0);      // Lower Bounds Error
      assert(idx1 < SP::GetSize());  // Upper Bounds Error
      assert(idx2 >= 0);      // Lower Bounds Error
      assert(idx2 < SP::GetSize());  // Upper Bounds Error
      
      SP::Swap(idx1, idx2);
    }  
    
    
    void SetAll(const T& value)
    {
      for (int i = 0; i < SP::GetSize(); i++) SP::operator[](i) = value;
    }
    
    Iterator<T>* Iterator() { return new ValueIterator(*this); }
    ConstIterator<T>* Iterator() const { return new ConstValueIterator(*this); }
    
    
  protected:
    class ValueIterator : public Apto::Iterator<T>
    {
    private:
      Array& m_arr;
      int m_index;
      
      ValueIterator(); // @not_implemented
      
    public:
      ValueIterator(Array& arr) : m_arr(arr), m_index(-1) { ; }
      
      T* Get() { return (m_index < m_arr.GetSize()) ? &(m_arr.SP::operator[](m_index)) : NULL; }
      T* Next() { return (++m_index < m_arr.GetSize()) ? &(m_arr.SP::operator[](m_index)) : NULL; }
    };
    
    class ConstValueIterator : public Apto::ConstIterator<T>
    {
    private:
      const Array& m_arr;
      int m_index;
      
      ConstValueIterator(); // @not_implemented
      
    public:
      ConstValueIterator(const Array& arr) : m_arr(arr), m_index(-1) { ; }
      
      const T* Get() { return (m_index < m_arr.GetSize()) ? &(m_arr.SP::operator[](m_index)) : NULL; }
      const T* Next() { return (++m_index < m_arr.GetSize()) ? &(m_arr.SP::operator[](m_index)) : NULL; }
    };
  };
};

#endif
