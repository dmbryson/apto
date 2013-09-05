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

#include "apto/core/Definitions.h"
#include "apto/core/ArrayStorage.h"

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
    class Iterator;
    class ConstIterator;
    class Slice;
    class ConstSlice;
    
  public:
    inline explicit Array(SizeType size = 0) : SP(size) { ; }
    
    template <typename T1, template <class> class SP1>
    inline explicit Array(const Array<T1, SP1>& rhs) : SP(rhs.GetSize()) { this->operator=(rhs); }
    
    ~Array() { ; }
    
    template <typename T1, template <class> class SP1>
    Array& operator=(const Array<T1, SP1>& rhs)
    {
      if (SP::GetSize() != rhs.GetSize()) SP::Resize(rhs.GetSize());
      for (SizeType i = 0; i < GetSize(); i++) SP::operator[](i) = rhs[i];
      return *this;
    }
    
    Array& operator=(const Slice& rhs)
    {
      if (SP::GetSize() != rhs.GetSize()) SP::Resize(rhs.GetSize());
      for (SizeType i = 0; i < GetSize(); i++) SP::operator[](i) = rhs[i];
      return *this;
    }
    
    Array& operator=(const ConstSlice& rhs)
    {
      if (SP::GetSize() != rhs.GetSize()) SP::Resize(rhs.GetSize());
      for (SizeType i = 0; i < GetSize(); i++) SP::operator[](i) = rhs[i];
      return *this;
    }
    
    template <typename T1, template <class> class SP1>
    Array& operator+=(const Array<T1, SP1>& rhs)
    {
      SizeType old_size = SP::GetSize();
      SizeType rhs_size = rhs.GetSize();
      SP::Resize(old_size + rhs_size);
      for (SizeType i = 0; i < rhs_size; i++) SP::operator[](i + old_size) = rhs[i];
      return *this;
    }

    Array& operator+=(const Slice& rhs)
    {
      SizeType old_size = SP::GetSize();
      SizeType rhs_size = rhs.GetSize();
      SP::Resize(old_size + rhs_size);
      for (SizeType i = 0; i < rhs_size; i++) SP::operator[](i + old_size) = rhs[i];
      return *this;
    }

    Array& operator+=(const ConstSlice& rhs)
    {
      SizeType old_size = SP::GetSize();
      SizeType rhs_size = rhs.GetSize();
      SP::Resize(old_size + rhs_size);
      for (SizeType i = 0; i < rhs_size; i++) SP::operator[](i + old_size) = rhs[i];
      return *this;
    }

    template <typename T1, template <class> class SP1>
    Array operator+(const Array<T1, SP1>& rhs) const
    {
      Array new_arr(SP::GetSize() + rhs.GetSize());
      for (SizeType i = 0; i < SP::GetSize(); i++) new_arr[i] = SP::operator[](i);
      for (SizeType i = 0; i < rhs.GetSize(); i++) new_arr[SP::GetSize() + i] = rhs[i];
      return new_arr;
    }

    Array operator+(const Slice& rhs) const
    {
      Array new_arr(SP::GetSize() + rhs.GetSize());
      for (SizeType i = 0; i < SP::GetSize(); i++) new_arr[i] = SP::operator[](i);
      for (SizeType i = 0; i < rhs.GetSize(); i++) new_arr[SP::GetSize() + i] = rhs[i];
      return new_arr;
    }

    Array operator+(const ConstSlice& rhs) const
    {
      Array new_arr(SP::GetSize() + rhs.GetSize());
      for (SizeType i = 0; i < SP::GetSize(); i++) new_arr[i] = SP::operator[](i);
      for (SizeType i = 0; i < rhs.GetSize(); i++) new_arr[SP::GetSize() + i] = rhs[i];
      return new_arr;
    }

    template <typename T1, template <class> class SP1>
    bool operator==(const Array<T1, SP1>& rhs)
    {
      if (SP::GetSize() != rhs.GetSize()) return false;
      for (SizeType i = 0; i < SP::GetSize(); i++) if (SP::operator[](i) != rhs[i]) return false;
      return true;
    }

    bool operator==(const Slice& rhs)
    {
      if (SP::GetSize() != rhs.GetSize()) return false;
      for (SizeType i = 0; i < SP::GetSize(); i++) if (SP::operator[](i) != rhs[i]) return false;
      return true;
    }

    bool operator==(const ConstSlice& rhs)
    {
      if (SP::GetSize() != rhs.GetSize()) return false;
      for (SizeType i = 0; i < SP::GetSize(); i++) if (SP::operator[](i) != rhs[i]) return false;
      return true;
    }
    
    template <typename T1, template <class> class SP1>
    bool operator!=(const Array<T1, SP1>& rhs) { return !operator==(rhs); }
    
    bool operator!=(const Slice& rhs) { return !operator==(rhs); }
    bool operator!=(const ConstSlice& rhs) { return !operator==(rhs); }

    inline SizeType GetSize() const { return SP::GetSize(); }
    
    inline void ResizeClear(const SizeType in_size)
    {
      assert(in_size >= 0);
      SP::ResizeClear(in_size);
    }
    
    inline void Resize(SizeType new_size)
    {
      assert(new_size >= 0);
      SP::Resize(new_size);
    }
    
    void Resize(SizeType new_size, const T& empty_value)
    {
      SizeType old_size = SP::GetSize();
      SP::Resize(new_size);
      for (SizeType i = old_size; i < new_size; i++) SP::operator[](i) = empty_value;
    }
    
    T& operator[](const SizeType index)
    {
      assert(index >= 0);       // Lower Bounds Error
      assert(index < SP::GetSize()); // Upper Bounds Error
      return SP::operator[](index);
    }
    const T& operator[](const SizeType index) const
    {
      assert(index >= 0);       // Lower Bounds Error
      assert(index < SP::GetSize()); // Upper Bounds Error
      return SP::operator[](index);
    }
    
    inline T& Get(const SizeType index) { return operator[](index); }
    inline const T& Get(const SizeType index) const { return operator[](index); }
    
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
    
    
    void Swap(SizeType idx1, SizeType idx2)
    {
      assert(idx1 >= 0);      // Lower Bounds Error
      assert(idx1 < SP::GetSize());  // Upper Bounds Error
      assert(idx2 >= 0);      // Lower Bounds Error
      assert(idx2 < SP::GetSize());  // Upper Bounds Error
      
      SP::Swap(idx1, idx2);
    }  
    
    
    void RemoveAt(SizeType idx)
    {
      assert(idx >= 0);
      assert(idx < SP::GetSize());
      
      for (SizeType i = idx; i < SP::GetSize() - 1; i++) SP::operator[](i) = SP::operator[](i + 1);
      SP::Resize(SP::GetSize() - 1);
    }
    
    
    void SetAll(const T& value)
    {
      for (SizeType i = 0; i < SP::GetSize(); i++) SP::operator[](i) = value;
    }
    
    Iterator Begin() { return Iterator(*this); }
    ConstIterator Begin() const { return ConstIterator(*this); }
    
    Slice Range(SizeType start_idx, SizeType end_idx) { return Slice(*this, start_idx, end_idx); }
    ConstSlice Range(SizeType start_idx, SizeType end_idx) const { return ConstSlice(*this, start_idx, end_idx); }
    
    
  public:
    class Iterator
    {
      friend class Array;
      
    private:
      Array& m_arr;
      SizeType m_index;
      
      Iterator(); // @not_implemented
      Iterator(Array& arr) : m_arr(arr), m_index(-1) { ; }
      
    public:      
      T* Get() { return (m_index < m_arr.GetSize()) ? &(m_arr.SP::operator[](m_index)) : NULL; }
      T* Next() { return (++m_index < m_arr.GetSize()) ? &(m_arr.SP::operator[](m_index)) : NULL; }
    };
    
    class ConstIterator
    {
      friend class Array;
    private:
      const Array& m_arr;
      SizeType m_index;
      
      ConstIterator(); // @not_implemented
      ConstIterator(const Array& arr) : m_arr(arr), m_index(-1) { ; }
            
    public:
      const T* Get() { return (m_index < m_arr.GetSize()) ? &(m_arr.SP::operator[](m_index)) : NULL; }
      const T* Next() { return (++m_index < m_arr.GetSize()) ? &(m_arr.SP::operator[](m_index)) : NULL; }
    };
    
    class Slice
    {
      friend class Array;
      
    private:
      Array& m_arr;
      SizeType m_start_idx;
      SizeType m_end_idx;
      
      Slice(); // @not_implemented
      inline Slice(Array& arr, SizeType start_idx, SizeType end_idx) : m_arr(arr), m_start_idx(start_idx), m_end_idx(end_idx)
      {
        assert(end_idx >= start_idx);
        assert(end_idx < arr.GetSize());
        assert(start_idx >= 0);
      }
      
    public:
      inline SizeType GetSize() const { return m_end_idx - m_start_idx + 1; }

      inline T& operator[](const SizeType index)
      {
        assert(index >= 0);        // Lower Bounds Error
        assert(index < GetSize()); // Upper Bounds Error
        return m_arr[m_start_idx + index];
      }
      inline const T& operator[](const SizeType index) const
      {
        assert(index >= 0);       // Lower Bounds Error
        assert(index < GetSize()); // Upper Bounds Error
        return m_arr[m_start_idx + index];
      }
      
      inline T& Get(const SizeType index) { return operator[](index); }
      inline const T& Get(const SizeType index) const { return operator[](index); }
      
      Slice Range(SizeType start_idx, SizeType end_idx) { return Slice(m_arr, m_start_idx + start_idx, m_start_idx + end_idx); }
      ConstSlice Range(SizeType start_idx, SizeType end_idx) const { return ConstSlice(m_arr, m_start_idx + start_idx, m_start_idx + end_idx); }
    };

    class ConstSlice
    {
      friend class Array;
      
    private:
      const Array& m_arr;
      SizeType m_start_idx;
      SizeType m_end_idx;
      
      ConstSlice(); // @not_implemented
      inline ConstSlice(Array& arr, SizeType start_idx, SizeType end_idx) : m_arr(arr), m_start_idx(start_idx), m_end_idx(end_idx)
      {
        assert(end_idx >= start_idx);
        assert(end_idx < arr.GetSize());
        assert(start_idx >= 0);
      }
      
    public:
      inline SizeType GetSize() const { return m_end_idx - m_start_idx + 1; }
      
      inline const T& operator[](const SizeType index) const
      {
        assert(index >= 0);       // Lower Bounds Error
        assert(index < GetSize()); // Upper Bounds Error
        return m_arr[m_start_idx + index];
      }
      
      inline const T& Get(const SizeType index) const { return operator[](index); }

      ConstSlice Range(SizeType start_idx, SizeType end_idx) const { return ConstSlice(m_arr, m_start_idx + start_idx, m_start_idx + end_idx); }
    };
  };
};

#endif
