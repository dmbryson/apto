/*
 *  String.h
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

#ifndef AptoCoreString_h
#define AptoCoreString_h

#include <cassert>
#include <string.h>

#include "apto/core/RefCount.h"
#include "apto/core/SmartPtr.h"
#include "apto/core/TypeSelect.h"


namespace Apto {
  
  template <class ThreadingModel = SingleThreaded> class BasicString
  {
  protected:
    class StringRep : public TypeSelect<ThreadingModel::UseThreadSafe, MTRefCountObject, RefCountObject>::Result
    {
    private:
      int m_size;
      char* m_data;
      
    public:
      explicit StringRep(int size = 0) : m_size(size), m_data(new char[size + 1])
      {
        assert(m_data);
        m_data[0] = '\0';
        m_data[size] = '\0';
      }
      StringRep(int size, const char* str) : m_size(size), m_data(new char[size + 1])
      {
        assert(m_data);
        memcpy(m_data, str, m_size);
        m_data[size] = '\0';
      }
      StringRep(const StringRep& rhs) : m_size(rhs.m_size), m_data(new char[m_size + 1])
      {
        assert(m_data);
        memcpy(m_data, rhs.m_data, m_size);
        m_data[m_size] = '\0';
      }
      
      ~StringRep() { delete [] m_data; }
      
      inline int GetSize() const { return m_size; }
      inline const char* GetRep() const { return m_data; }
      
      char operator[](int index) const { return m_data[index]; }
      char& operator[](int index) { return m_data[index]; }
    };
    
    typedef SmartPtr<StringRep, InternalRCObject> StringRepPtr;
    StringRepPtr m_value;
    
    
    BasicString(StringRepPtr rep) : m_value(rep) { ; }
    
  public:
    BasicString(const char* str = "") : m_value((str) ? new StringRep(strlen(str), str) : new StringRep(0)) { assert(m_value); }
    BasicString(int size, const char* str) : m_value(new StringRep(size, str)) { assert(m_value); }
    explicit BasicString(int size) : m_value(new StringRep(size)) { assert(m_value); }
    BasicString(const BasicString& rhs) : m_value(rhs.m_value) { ; }
    template <class T1> BasicString(const BasicString<T1>& rhs) : m_value(new StringRep(rhs.GetSize(), rhs.GetData())) { assert(false); }
    
    ~BasicString() { ; }
        
    inline int GetSize() const { return m_value->GetSize(); }
    const char* GetData() const { return m_value->GetRep(); }
    
    inline operator const char*() const { return m_value->GetRep(); }

    inline BasicString& operator=(const BasicString& rhs) { m_value = rhs.m_value; return *this; }
    template <class T1> inline BasicString& operator=(const BasicString<T1>& rhs)
    {
      m_value = new StringRep(rhs.GetSize(), rhs.GetData());
      assert(m_value);
      return *this;
    }
    
    inline BasicString& operator=(const char* rhs)
    {
      assert(rhs);
      m_value = StringRepPtr(new StringRep(strlen(rhs), rhs));
      assert(m_value);
      return *this;
    }
    
    
    // Comparison Operators
    int Compare(const char* str) const
    {
      assert(str);
      int i;
      for (i = 0; i < GetSize() && str[i] != '\0' && (*this)[i] == str[i]; i++);
      
      if (i == GetSize() && str[i] == '\0') return 0;
      if (i < GetSize() && str[i] < (*this)[i]) return 1;
      return -1;
    }
    
    bool operator==(const BasicString& rhs) const
    {
      if (rhs.GetSize() != GetSize()) return false;
      for (int i = 0; i < GetSize(); i++) if ((*this)[i] != rhs[i]) return false;
      return true;
    }
    inline bool operator==(const char* rhs) const { return Compare(rhs) == 0; }
    inline bool operator!=(const BasicString& rhs) const { return !operator==(rhs); }
    inline bool operator!=(const char* rhs) const { return Compare(rhs) != 0; }
    inline bool operator<(const char* rhs) const { return Compare(rhs) < 0; }
    inline bool operator>(const char* rhs) const { return Compare(rhs) > 0; }
    inline bool operator<=(const char* rhs) const { return Compare(rhs) <= 0; }
    inline bool operator>=(const char* rhs) const { return Compare(rhs) >= 0; }
    
    inline char operator[](int index) const { return m_value->operator[](index); }
    
    inline BasicString& operator+=(const char c) { return append(1, &c); }
    inline BasicString& operator+=(const char* str) { return append(strlen(str), str); }
    template <class R> BasicString& operator+=(const BasicString<R>& str) { return append(str.GetSize(), str.GetData()); }
    inline BasicString operator+(const char c) { return concat(1, &c); }
    inline BasicString operator+(const char* str) { return concat(strlen(str), str); }
    template <class R> BasicString operator+(const BasicString<R>& str) { return concat(str.GetSize(), str.GetData()); }
    
  protected:
    BasicString& append(int size, const char* str)
    {
      assert(size == 0 || str != NULL);
      StringRepPtr newstr(new StringRep(size + GetSize()));
      assert(newstr);
      for (int i = 0; i < GetSize(); i++) newstr->operator[](i) = m_value->operator[](i);
      for (int i = 0; i < size; i++) newstr->operator[](i + GetSize()) = str[i];
      m_value = newstr;
      return (*this);
    }
    
    BasicString concat(int size, const char* str)
    {
      if (size == 0) return BasicString(*this);
      assert(str != NULL);
      StringRepPtr newstr(new StringRep(size + GetSize()));
      for (int i = 0; i < GetSize(); i++) newstr->operator[](i) = m_value->operator[](i);
      for (int i = 0; i < size; i++) newstr->operator[](i + GetSize()) = str[i];
      return BasicString(newstr);
    }
  };
  
  
  // HASH_TYPE = BasicString<ThreadingModel>
  // We hash a string simply by adding up the individual character values in
  // that string and modding by the hash size.  For most applications this
  // will work fine (and reasonably fast!) but some patterns will cause all
  // strings to go into the same cell.  For example, "ABC"=="CBA"=="BBB".
  template <class T, int HashFactor> class HashKey;
  template <class ThreadingModel, int HashFactor> class HashKey<BasicString<ThreadingModel>, HashFactor>
  {
  public:
    int Hash(const BasicString<ThreadingModel>& key)
    {
      unsigned int out_hash = 0;
      for (int i = 0; i < key.GetSize(); i++)
        out_hash += (unsigned int) key[i];
      return out_hash % HashFactor;
    }
  };

  typedef BasicString<> String;
};

#endif
