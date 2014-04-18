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

#include "apto/core/Definitions.h"
#include "apto/core/RefCount.h"
#include "apto/core/SmartPtr.h"
#include "apto/core/TypeUtil.h"

#include <cassert>
#include <string.h>


namespace Apto {
  
  // Internal String Representation Implemenation
  // --------------------------------------------------------------------------------------------------------------
  
  namespace Internal {
    class StringRep
    {
    private:
      int m_size;
      char* m_data;
      
    public:
      LIB_EXPORT inline explicit StringRep(int size = 0) : m_size(size), m_data(new char[size + 1])
      {
        assert(m_data);
        m_data[0] = '\0';
        m_data[size] = '\0';
      }
      LIB_EXPORT inline StringRep(int size, const char* str) : m_size(size), m_data(new char[size + 1])
      {
        assert(m_data);
        memcpy(m_data, str, m_size);
        m_data[size] = '\0';
      }
      LIB_EXPORT inline StringRep(const StringRep& rhs) : m_size(rhs.m_size), m_data(new char[m_size + 1])
      {
        assert(m_data);
        memcpy(m_data, rhs.m_data, m_size);
        m_data[m_size] = '\0';
      }
      
      LIB_EXPORT virtual ~StringRep();
      
      LIB_EXPORT inline int GetSize() const { return m_size; }
      LIB_EXPORT inline const char* GetRep() const { return m_data; }
      
      LIB_EXPORT inline char operator[](int index) const { return m_data[index]; }
      LIB_EXPORT inline char& operator[](int index) { return m_data[index]; }
      
      LIB_EXPORT virtual void AddReference() const = 0;
      LIB_EXPORT virtual void RemoveReference() const = 0;
    };

  };

  
  // Basic String
  // --------------------------------------------------------------------------------------------------------------
  
  template <template <class> class ThreadingModel> class BasicString
  {
    friend class BasicString<SingleThreaded>;
    friend class BasicString<ThreadSafe>;
  public:
    typedef char ValueType;
    class Iterator;
    class ConstIterator;
    
  protected:
    typedef SmartPtr<Internal::StringRep, InternalRCObject> StringRepPtr;
    class TMStringRep : public Internal::StringRep
    {
    private:
      mutable typename ThreadingModel<int>::AtomicInt m_ref_count;
      
    public:
      inline explicit TMStringRep(int size = 0) : StringRep(size) { ThreadingModel<int>::Set(m_ref_count, 1); }
      inline TMStringRep(int size, const char* str) : StringRep(size, str) { ThreadingModel<int>::Set(m_ref_count, 1); }
      inline TMStringRep(const TMStringRep& rhs) : StringRep(rhs) { ThreadingModel<int>::Set(m_ref_count, 1); }
      inline TMStringRep(const StringRep& rhs) : StringRep(rhs) { ThreadingModel<int>::Set(m_ref_count, 1); }
      ~TMStringRep() { ; }

      void AddReference() const { ThreadingModel<int>::Inc(m_ref_count); }
      void RemoveReference() const { if (ThreadingModel<int>::DecAndTest(m_ref_count)) delete this; }
    };
    
  protected:
    StringRepPtr m_value;
    
    inline BasicString(StringRepPtr rep) : m_value(rep) { ; }
    
  public:
    // Construction
    inline BasicString() { ; }
    inline BasicString(const char* str) : m_value((str != NULL) ? new TMStringRep(static_cast<int>(strlen(str)), str) : NULL) { assert(str == NULL || m_value); }
    inline BasicString(int size, const char* str) : m_value(new TMStringRep(size, str)) { assert(m_value); }
    inline BasicString(const BasicString<SingleThreaded>& rhs) : m_value(rhs.m_value) { ; }
    inline BasicString(const BasicString<ThreadSafe>& rhs) : m_value(rhs.m_value) { ; }
    
    inline ~BasicString() { ; }
    
    inline BasicString Clone() { return BasicString(m_value->GetSize(), m_value->GetRep()); }
    
    // Property Access
    inline int GetSize() const { return (m_value) ? m_value->GetSize() : 0; }
    inline const char* GetData() const { return (m_value) ? m_value->GetRep() : ""; }
    inline const char* GetCString() const { return GetData(); }
    inline operator const char*() const { return GetData(); }

    
    // Assignment
    inline BasicString& operator=(const BasicString<SingleThreaded>& rhs) { m_value = rhs.m_value; return *this; }
    inline BasicString& operator=(const BasicString<ThreadSafe>& rhs) { m_value = rhs.m_value; return *this; }
    
    inline BasicString& operator=(const char* rhs)
    {
      assert(rhs);
      m_value = StringRepPtr(new TMStringRep(static_cast<int>(strlen(rhs)), rhs));
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
    
    bool operator==(const BasicString<SingleThreaded>& rhs) const
    {
      if (rhs.GetSize() != GetSize()) return false;
      for (int i = 0; i < GetSize(); i++) if ((*this)[i] != rhs[i]) return false;
      return true;
    }
    bool operator==(const BasicString<ThreadSafe>& rhs) const
    {
      if (rhs.GetSize() != GetSize()) return false;
      for (int i = 0; i < GetSize(); i++) if ((*this)[i] != rhs[i]) return false;
      return true;
    }
    inline bool operator==(const char* rhs) const { return Compare(rhs) == 0; }
    
    template <template <class> class R>
    inline bool operator!=(const BasicString<R>& rhs) const { return !operator==(rhs); }
    inline bool operator!=(const char* rhs) const { return Compare(rhs) != 0; }
    
    template <template <class> class R>
    inline bool operator<(const BasicString<R>& rhs) const { return Compare(rhs) < 0; }
    inline bool operator<(const char* rhs) const { return Compare(rhs) < 0; }
    
    template <template <class> class R>
    inline bool operator>(const BasicString<R>& rhs) const { return Compare(rhs) > 0; }
    inline bool operator>(const char* rhs) const { return Compare(rhs) > 0; }

    template <template <class> class R>
    inline bool operator<=(const BasicString<R>& rhs) const { return Compare(rhs) <= 0; }
    inline bool operator<=(const char* rhs) const { return Compare(rhs) <= 0; }

    template <template <class> class R>
    inline bool operator>=(const BasicString<R>& rhs) const { return Compare(rhs) >= 0; }
    inline bool operator>=(const char* rhs) const { return Compare(rhs) >= 0; }
    
    
    // Character access
    inline char operator[](int index) const { return m_value->operator[](index); }
    
    
    // Concatenation
    inline BasicString& operator+=(const char c) { return append(1, &c); }
    inline BasicString& operator+=(const char* str) { return append(static_cast<int>(strlen(str)), str); }
    inline BasicString& operator+=(const BasicString& str) { return append(str.GetSize(), str.GetData()); }
    inline BasicString operator+(const char c) { return concat(1, &c); }
    inline BasicString operator+(const char* str) { return concat(static_cast<int>(strlen(str)), str); }
    inline BasicString operator+(const BasicString& str) { return concat(str.GetSize(), str.GetData()); }
    
    
    // Substring extraction
    inline BasicString Substring(int idx = 0, int length = -1) const
    {
      assert(idx >= 0);
      assert(idx <= GetSize());
      assert(length <= (GetSize() - idx));
      return BasicString((length >= 0) ? length : (GetSize() - idx), m_value->GetRep() + idx);
    }
    
    BasicString Pop(const char delimeter)
    {
      StringRepPtr value = m_value;
      for (int i = 0; i < GetSize(); i++) {
        if (m_value->operator[](i) == delimeter) {
          *this = Substring(i + 1);
          return BasicString(i, value->GetRep());
        }
      }
      BasicString rtn(*this);
      *this = BasicString();
      return rtn;
    }
    
    BasicString PopWord()
    {
      if (GetSize() == 0) return BasicString();
      
      const int start_pos = WhitespaceAt(0);
      const int word_size = WordSizeAt(start_pos);
      
      BasicString rv(Substring(start_pos, word_size));
      
      // Trim off the front
      const int word_end = start_pos + word_size;
      const int new_start = word_end + WhitespaceAt(word_end);
      (*this) = Substring(new_start);
      
      return rv;
    }
    
    
    // Modified content
    BasicString& ToLower()
    {
      m_value = StringRepPtr(new TMStringRep(*m_value));
      for (int i = 0; i < m_value->GetSize(); i++) {
        if (m_value->operator[](i) >= 'A' && m_value->operator[](i) <= 'Z' )
          m_value->operator[](i) += 'a' - 'A';
      }
      
      return *this;
    }

    BasicString AsLower() const
    {
      StringRepPtr value(new TMStringRep(*m_value));
      for (int i = 0; i < value->GetSize(); i++) {
        if (value->operator[](i) >= 'A' && value->operator[](i) <= 'Z' )
          value->operator[](i) += 'a' - 'A';
      }
      
      return BasicString(value);
    }
    
    BasicString& ToUpper()
    {
      m_value = StringRepPtr(new TMStringRep(*m_value));
      for (int i = 0; i < m_value->GetSize(); i++) {
        if (m_value->operator[](i) >= 'a' && m_value->operator[](i) <= 'z' )
          m_value->operator[](i) += 'A' - 'a';
      }
      
      return *this;
    }

    BasicString AsUpper() const
    {
      StringRepPtr value(new TMStringRep(*m_value));
      for (int i = 0; i < value->GetSize(); i++) {
        if (value->operator[](i) >= 'a' && value->operator[](i) <= 'z' )
          value->operator[](i) += 'A' - 'a';
      }
      
      return BasicString(value);
    }
    
    BasicString& Trim()
    {
      int start_idx = 0;
      while (start_idx < GetSize() && IsWhitespace(start_idx)) start_idx++;
      int end_idx = GetSize() - 1;
      while (end_idx > start_idx && IsWhitespace(end_idx)) end_idx--;
      
      m_value = StringRepPtr(new TMStringRep(end_idx - start_idx + 1, m_value->GetRep() + start_idx));
      
      return *this;
    }
    
    BasicString Trimmed() const
    {
      int start_idx = 0;
      while (start_idx < GetSize() && IsWhitespace(start_idx)) start_idx++;
      int end_idx = GetSize() - 1;
      while (end_idx > start_idx && IsWhitespace(end_idx)) end_idx--;
      
      return BasicString(end_idx - start_idx + 1, m_value->GetRep() + start_idx);
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
    
    
    // Whole string inspection
    bool IsNumber() const
    {
      int start_idx = 0;
      
      // Allow +/- prefix
      if ((*this)[0] == '+' || (*this)[0] == '-') start_idx = 1;

      bool has_decimal = false;
      bool has_exponent = false;
      for (int i = start_idx; i < GetSize(); i++) {
        
        // Handle possible decimal point
        if ((*this)[i] == '.') {
          if (has_decimal) return false; // cannot have more then one decimal point
          has_decimal = true;
          continue;
        }
        
        // Handle possible exponent
        if ((*this)[i] == 'E' || (*this)[i] == 'e') {
          if ((i + 1) >= GetSize()) return false; // malformed exponent
          
          // Check for +/- prefix on exponent
          if ((*this)[i + 1] == '+' || (*this)[i + 1] == '-') i++;
          
          has_exponent = true;
          has_decimal = true; // make sure no decimal points are allowed in the exponent
          continue;
        }
        
        if (!IsNumber(i)) return false;
      }
      
      return true;
    }
    

    // Counting Methods
    int WhitespaceAt(int start) const
    {
      assert(start >= 0);
      if (start >= GetSize()) return 0;
      int count = 0;
      while (start + count < GetSize() && IsWhitespace(start + count)) count++;
      return count;
    }
    
    
    int WordSizeAt(int start) const
    {
      assert(start >= 0);
      if (start >= GetSize()) return 0;
      int count = 0;
      while ((start + count) < GetSize() && !IsWhitespace(start + count)) count++;
      return count;
    }
    
    
    int LineSizeAt(int start) const
    {
      assert(start >= 0);
      if (start >= GetSize())  return 0;
      int count = 0;
      while ((start + count) < GetSize() && (*this)[start + count] != '\n') count++;
      return count;
    }
    
    
    int NumLines() const
    {
      int num_lines = 1;
      for (int i = 0; i < GetSize(); i++) if ((*this)[i] == '\n') num_lines++;
      return num_lines;
    }
    
    
    int NumWords() const
    {
      int num_words = 0;
      int pos = WhitespaceAt(0);     // Skip initial whitespace.
      while (pos < GetSize()) {
        pos += WordSizeAt(pos);
        pos += WhitespaceAt(pos);
        num_words++;
      }
      return num_words;
    }
    
    
    // Searching Operations
    int Find(char search_char, int pos = 0) const
    {
      assert(pos >= 0);
      assert(pos <= GetSize());
      
      if (pos <= 0) pos = 0;
      else if (pos > GetSize()) pos = GetSize();
      
      while (pos < GetSize()) {
        if (this->operator[](pos) == search_char) return pos;
        pos++;
      }
      return -1;
    }
    
    inline int Find(const char* search_str, int pos = 0) const { return findString(search_str, strlen(search_str), pos); }
    inline int Find(const BasicString& search_str, int pos = 0) const { return findString(search_str, search_str.GetSize(), pos); }
    
    
    // Iterators
    Iterator Begin() { return Iterator(m_value); }
    ConstIterator Begin() const { return ConstIterator(m_value); }
    
  protected:
    // Internal Support Methods
    BasicString& append(int size, const char* str)
    {
      assert(size == 0 || str != NULL);
      StringRepPtr newstr(new TMStringRep(size + GetSize()));
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
      StringRepPtr newstr(new TMStringRep(size + GetSize()));
      for (int i = 0; i < GetSize(); i++) newstr->operator[](i) = m_value->operator[](i);
      for (int i = 0; i < size; i++) newstr->operator[](i + GetSize()) = str[i];
      return BasicString(newstr);
    }
    
    
    int findString(const char* search_str, int size, int pos) const
    {
      assert(pos >= 0);
      assert(pos <= GetSize());
      
      while (pos < GetSize()) {
        if (GetSize() - pos < size) return -1;
        if (this->operator[](pos) == search_str[0]) {
          int i = 1;
          for (; i < size; i++ ) {
            assert(pos + i < GetSize());
            assert(search_str[i] != '\0');
            if (this->operator[](pos + i) != search_str[i]) break;
          }
          
          if (i == size) return pos;
        }
        pos++;
      }
      
      return -1;
    }

  
    
    
  public:
    class Iterator
    {
      friend class BasicString<ThreadingModel>;
      
    private:
      StringRepPtr m_value;
      int m_index;
      
      inline Iterator(StringRepPtr value) : m_value(value), m_index(-1) { ; }
      
    public:
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
      friend class BasicString<ThreadingModel>;
      
    private:
      StringRepPtr m_value;
      int m_index;
      
      inline ConstIterator(StringRepPtr value) : m_value(value), m_index(-1) { ; }
      
    public:
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
  
  
  template <> inline BasicString<ThreadSafe>::BasicString(const BasicString<SingleThreaded>& rhs)
    : m_value(new TMStringRep(rhs.GetSize(), rhs.GetData())) { ; }
  template <> inline BasicString<ThreadSafe>& BasicString<ThreadSafe>::operator=(const BasicString<SingleThreaded>& rhs)
  {
    m_value = StringRepPtr(new TMStringRep(rhs.GetSize(), rhs.GetData()));
    assert(m_value);
    return *this;
  }
  

  template <template <class> class ThreadingModel>
  inline bool operator==(const char* lhs, const BasicString<ThreadingModel>& rhs) { return rhs == lhs; }
  template <template <class> class ThreadingModel>
  inline bool operator!=(const char* lhs, const BasicString<ThreadingModel>& rhs) { return rhs != lhs; }
  
  template <template <class> class ThreadingModel>
  inline bool operator<(const char* lhs, const BasicString<ThreadingModel>& rhs) { return rhs > lhs; }
  template <template <class> class ThreadingModel>
  inline bool operator>(const char* lhs, const BasicString<ThreadingModel>& rhs) { return rhs < lhs; }
  
  template <template <class> class ThreadingModel>
  inline bool operator<=(const char* lhs, const BasicString<ThreadingModel>& rhs) { return rhs >= lhs; }
  template <template <class> class ThreadingModel>
  inline bool operator>=(const char* lhs, const BasicString<ThreadingModel>& rhs) { return rhs <= lhs; }
  
  
  // Basic String Hashing Support
  // --------------------------------------------------------------------------------------------------------------
  
  // HASH_TYPE = BasicString<ThreadingModel>
  // We hash a string simply by adding up the individual character values in
  // that string and modding by the hash size.  For most applications this
  // will work fine (and reasonably fast!) but some patterns will cause all
  // strings to go into the same cell.  For example, "ABC"=="CBA"=="BBB".
  template <class T, int HashFactor> class HashKey;
  template <template <class> class ThreadingModel, int HashFactor> class HashKey<BasicString<ThreadingModel>, HashFactor>
  {
  public:
    static int Hash(const BasicString<ThreadingModel>& key)
    {
      unsigned int out_hash = 0;
      for (int i = 0; i < key.GetSize(); i++)
        out_hash += (unsigned int) key[i];
      return out_hash % HashFactor;
    }
  };


  // Apto::String
  // --------------------------------------------------------------------------------------------------------------
  
  typedef BasicString<SingleThreaded> String;
};

#endif
