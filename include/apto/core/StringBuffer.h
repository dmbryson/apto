/*
 *  StringBuffer.h
 *  Apto
 *
 *  Created by David on 2/10/11.
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

#ifndef AptoCoreStringBuffer_h
#define AptoCoreStringBuffer_h

#include "apto/core/Definitions.h"
#include "apto/core/RefCount.h"

#include <cassert>
#include <string.h>


namespace Apto {
  
  // Declarations used to allow string buffer type conversion
  // --------------------------------------------------------------------------------------------------------------
  
  template <template <class> class ThreadingModel> class BasicString;
  
  
  // String Buffer
  // --------------------------------------------------------------------------------------------------------------
  
  class StringBuffer
  {
  public:
    typedef char ValueType;
    class Iterator;
    class ConstIterator;
    
  protected:
    class StringBufferRep;
    class CharProxy;

  protected:
    static const int BUFFER_INCREASE_MINIMUM = 4;
    static const int BUFFER_INCREASE_FACTOR = 2;
    static const int BUFFER_SHRINK_TEST_FACTOR = 4;
    
    inline void copyOnWrite();

  protected:    
    StringBufferRep* m_value;
    
  public:
    inline StringBuffer(const char* str = "")
      : m_value((str) ? new StringBufferRep(static_cast<int>(strlen(str)), str) : new StringBufferRep(0, "")) { assert(m_value); }
    inline StringBuffer(int size, const char* str) : m_value(new StringBufferRep(size, str)) { assert(m_value); }
    inline explicit StringBuffer(int size) : m_value(new StringBufferRep(size)) { assert(m_value); }
    inline StringBuffer(const StringBuffer& rhs) : m_value(NULL) { this->operator=(rhs); }
    template <template <class> class R>
    inline StringBuffer(const BasicString<R>& rhs) : m_value(new StringBufferRep(rhs.GetSize(), rhs.GetData())) { ; }
    
    inline ~StringBuffer() { m_value->RemoveReference(); }
    
    
    // Property Access
    inline int GetSize() const { return m_value->GetSize(); }
    inline const char* GetData() const { return m_value->GetRep(); }
    inline const char* GetCString() const { return m_value->GetRep(); }
    inline operator const char*() const { return m_value->GetRep(); }
    

    // Assignment
    inline StringBuffer& operator=(const StringBuffer& rhs)
    {
      if (&rhs == this) return *this;
      if (m_value) m_value->RemoveReference();
      
      m_value = rhs.m_value;
      m_value->AddReference();
      
      return *this;
    }
    
    inline StringBuffer& operator=(const char* rhs)
    {
      assert(rhs);
      m_value->RemoveReference();
      m_value = new StringBufferRep(static_cast<int>(strlen(rhs)), rhs);
      assert(m_value);
      return *this;
    }
    
    
    // Comparison Operators
    int Compare(const char* str) const
    {
      assert(str);
      int i;
      for (i = 0; i < GetSize() && str[i] != '\0' && (*this)[i] == str[i]; i++) ;
      
      if (i == GetSize() && str[i] == '\0') return 0;
      if (i < GetSize() && str[i] < (*this)[i]) return 1;
      return -1;
    }
    
    bool operator==(const StringBuffer& rhs) const
    {
      if (rhs.GetSize() != GetSize()) return false;
      for (int i = 0; i < GetSize(); i++) if ((*this)[i] != rhs[i]) return false;
      return true;
    }
    inline bool operator==(const char* rhs) const { return Compare(rhs) == 0; }
    
    inline bool operator!=(const StringBuffer& rhs) const { return !operator==(rhs); }
    inline bool operator!=(const char* rhs) const { return Compare(rhs) != 0; }
    
    inline bool operator<(const StringBuffer& rhs) const { return Compare(rhs) < 0; }
    inline bool operator<(const char* rhs) const { return Compare(rhs) < 0; }
    
    inline bool operator>(const StringBuffer& rhs) const { return Compare(rhs) > 0; }
    inline bool operator>(const char* rhs) const { return Compare(rhs) > 0; }
    
    inline bool operator<=(const StringBuffer& rhs) const { return Compare(rhs) <= 0; }
    inline bool operator<=(const char* rhs) const { return Compare(rhs) <= 0; }
    
    inline bool operator>=(const StringBuffer& rhs) const { return Compare(rhs) >= 0; }
    inline bool operator>=(const char* rhs) const { return Compare(rhs) >= 0; }

    
    // Character access
    inline char operator[](int index) const { return m_value->operator[](index); }
    CharProxy operator[](int index) { return CharProxy(*this, index); }
    
    
    // Concatenation
    inline StringBuffer& operator+=(const char c) { return append(1, &c); }
    inline StringBuffer& operator+=(const char* str) { return append(static_cast<int>(strlen(str)), str); }
    inline StringBuffer& operator+=(const StringBuffer& str) { return append(str.GetSize(), str.GetData()); }
    template <template <class> class R>
    inline StringBuffer& operator+=(const BasicString<R>& str) { return append(str.GetSize(), str.GetData()); }
    inline StringBuffer operator+(const char c) { return concat(1, &c); }
    inline StringBuffer operator+(const char* str) { return concat(static_cast<int>(strlen(str)), str); }
    inline StringBuffer operator+(const StringBuffer& str) { return concat(str.GetSize(), str.GetData()); }
    template <template <class> class R>
    inline StringBuffer operator+(const BasicString<R>& str) { return concat(str.GetSize(), str.GetData()); }
    
    
    // Substring extraction
    inline StringBuffer Substring(int idx = 0, int length = -1)
    {
      assert(idx >= 0);
      assert(idx < GetSize());
      assert(length <= (GetSize() - idx));
      return StringBuffer((length >= 0) ? length : (GetSize() - idx), m_value->GetRep() + idx);
    }
    
    
    // Various Character Inspection Utility Methods
    inline bool IsLetter(int idx) const { return IsUpper(idx) || IsLower(idx); }
    inline bool IsLower(int idx) const { return ((*this)[idx] >= 'a' && (*this)[idx] <= 'z'); }
    inline bool IsUpper(int idx) const { return ((*this)[idx] >= 'A' && (*this)[idx] <= 'Z'); }
    inline bool IsNumber(int idx) const { return (*this)[idx] >= '0' && (*this)[idx] <= '9'; }    
    inline bool IsWhitespace(int idx) const
    {
      return (*this)[idx] == ' ' ||   // space
      (*this)[idx] == '\f' ||  // form feed
      (*this)[idx] == '\n' ||  // newline
      (*this)[idx] == '\r' ||  // carriage return
      (*this)[idx] == '\t' ||  // horizontal tab
      (*this)[idx] == '\v';    // vertical tab
    }
    
    
    // Iterators
    Iterator Begin() { return Iterator(m_value); }
    ConstIterator Begin() const { return ConstIterator(m_value); }
    
  protected:
    // Internal Support Methods
    StringBuffer& append(int size, const char* str)
    {
      assert(size >= 0);
      if (size == 0) return *this;
      
      assert(str != NULL);
      copyOnWrite();
      int old_size = m_value->GetSize();
      m_value->Resize(old_size + size);
      for (int i = 0; i < size; i++) m_value->operator[](i + old_size) = str[i];
      return *this;
    }
    
    StringBuffer concat(int size, const char* str)
    {
      assert(size >= 0);
      if (size == 0) return StringBuffer(*this);
      
      StringBuffer rval(*this);
      rval.append(size, str);
      return rval;
    }

    
    class StringBufferRep : public RefCountObject<SingleThreaded>
    {
    private:
      int m_capacity;
      int m_size;
      char* m_data;
      
    public:
      explicit StringBufferRep(int size) : m_capacity(size + 1), m_size(size), m_data(new char[size + 1])
      {
        assert(m_data);
        m_data[0] = '\0';
        m_data[size] = '\0';
      }
      StringBufferRep(int size, const char* str) : m_capacity(size + 1), m_size(size), m_data(new char[size + 1])
      {
        assert(m_data);
        memcpy(m_data, str, m_size);
        m_data[size] = '\0';
      }
      StringBufferRep(const StringBufferRep& rhs)
        : RefCountObject<SingleThreaded>(), m_capacity(rhs.m_size + 1), m_size(rhs.m_size), m_data(new char[m_size + 1])
      {
        assert(m_data);
        memcpy(m_data, rhs.m_data, m_size);
        m_data[m_size] = '\0';
      }
      
      ~StringBufferRep() { delete [] m_data; }
      
      inline int GetSize() const { return m_size; }
      inline const char* GetRep() const { return m_data; }
      
      char& operator[](int index) { return m_data[index]; }
      char operator[](int index) const { return m_data[index]; }

      void Resize(int new_size)
      {
        // If we're already at the size we want, don't bother doing anything.
        if (new_size == m_size) return;
        
        // If new size is 0, clean up and go!
        if (new_size == 0) {
          if (m_data != NULL) delete [] m_data;
          m_data = NULL;
          m_size = 0;
          m_capacity = 0;
          return;
        }
        
        // Determine if we need to adjust the allocated array sizes...
        int new_capacity = new_size + 1;
        int shrink_test = new_capacity * BUFFER_SHRINK_TEST_FACTOR;
        if (new_capacity > m_capacity || shrink_test < m_capacity) {
          new_capacity *= BUFFER_INCREASE_FACTOR;
          int new_capacity_min = new_size + BUFFER_INCREASE_MINIMUM + 1;
          if (new_capacity_min > new_capacity) new_capacity = new_capacity_min;
          
          char* new_data = new char[new_capacity];
          assert(new_data != NULL); // Memory Allocation Error: Out of Memory?
          
          // Copy over old data...
          for (int i = 0; i < m_size && i < new_size; i++) {
            new_data[i] = m_data[i];
          }
          if (m_data != NULL) delete [] m_data;  // remove old data if exists
          m_data = new_data;
          
          m_capacity = new_capacity;
        }
        
        m_size = new_size;
        m_data[new_size] = '\0';  // NULL terminate character buffer
      }
    };
    
    
    class CharProxy
    {
    private:
      StringBuffer& m_str;
      int m_index;
      
    public:
      CharProxy(StringBuffer& str, int index) : m_str(str), m_index(index) { ; }
      
      inline CharProxy& operator=(char rhs) { m_str.copyOnWrite(); m_str.m_value->operator[](m_index) = rhs; return *this; }
      inline CharProxy& operator+=(char rhs) { m_str.copyOnWrite(); m_str.m_value->operator[](m_index) += rhs; return *this; }
      inline CharProxy& operator-=(char rhs) { m_str.copyOnWrite(); m_str.m_value->operator[](m_index) -= rhs; return *this; }
      inline CharProxy& operator++() { m_str.copyOnWrite(); ++m_str.m_value->operator[](m_index); return *this; }
      inline char operator++(int) { m_str.copyOnWrite(); return m_str.m_value->operator[](m_index)++; }
      inline CharProxy& operator--() { m_str.copyOnWrite(); --m_str.m_value->operator[](m_index); return *this; }
      inline char operator--(int) { m_str.copyOnWrite(); return m_str.m_value->operator[](m_index)--; }
      inline operator char() const { return m_str.m_value->operator[](m_index); }
    };
    friend class CharProxy;
    
    
  public:
    class Iterator
    {
      friend class StringBuffer;
      
    private:
      StringBufferRep* m_value;
      int m_index;
      
      inline Iterator(StringBufferRep* value) : m_value(value), m_index(-1) { m_value->AddReference(); }
      
    public:
      inline ~Iterator() { m_value->RemoveReference(); }
      inline const char* Get()
      {
        return (m_index < 0 || m_index >= m_value->GetSize()) ? NULL : &m_value->operator[](m_index);
      }
      
      inline const char* Next()
      {
        if (m_index == m_value->GetSize()) return NULL;
        return (++m_index == m_value->GetSize()) ? NULL : &m_value->operator[](m_index);
      }
    };
    
    class ConstIterator
    {
      friend class StringBuffer;
      
    private:
      StringBufferRep* m_value;
      int m_index;
      
      inline ConstIterator(StringBufferRep* value) : m_value(value), m_index(-1) { m_value->AddReference(); }
      
    public:
      inline ~ConstIterator() { m_value->RemoveReference(); }
      inline const char* Get()
      {
        return (m_index < 0 || m_index >= m_value->GetSize()) ? NULL : &m_value->operator[](m_index);
      }
      
      inline const char* Next()
      {
        if (m_index == m_value->GetSize()) return NULL;
        return (++m_index == m_value->GetSize()) ? NULL : &m_value->operator[](m_index);
      }
    };
  };
  
  inline bool operator==(const char* lhs, const StringBuffer& rhs) { return rhs == lhs; }
  inline bool operator!=(const char* lhs, const StringBuffer& rhs) { return rhs != lhs; }
  
  inline bool operator<(const char* lhs, const StringBuffer& rhs) { return rhs > lhs; }
  inline bool operator>(const char* lhs, const StringBuffer& rhs) { return rhs < lhs; }
  
  inline bool operator<=(const char* lhs, const StringBuffer& rhs) { return rhs >= lhs; }
  inline bool operator>=(const char* lhs, const StringBuffer& rhs) { return rhs <= lhs; }
  
  inline void StringBuffer::copyOnWrite()
  {
    if (m_value->RefCount() != 1) {
      StringBufferRep* old_value = m_value;
      m_value = new StringBufferRep(*old_value);
      old_value->RemoveReference();
    }
  }
};  

#endif
