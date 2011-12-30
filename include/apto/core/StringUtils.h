/*
 *  StringUtils.h
 *  Apto
 *
 *  Created by David on 3/15/11.
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

#ifndef AptoCoreStringUtils_h
#define AptoCoreStringUtils_h

#include "apto/platform.h"
#include "apto/core/Definitions.h"
#include "apto/core/String.h"

#include <cstdarg>
#include <cstdlib>
#include <cstdio>


namespace Apto {

  // StrAs - Convert string into various native types
  // --------------------------------------------------------------------------------------------------------------
  
  class StrAs
  {
  private:
    const char* m_str;
    char* m_str_copy;
    
    StrAs(); // @not_implemented
    
  public:
    inline StrAs(const char* str) : m_str(str), m_str_copy(NULL) { ; }
    inline StrAs(const BasicString<SingleThreaded>& str) : m_str_copy(new char[str.GetSize()])
    {
      memcpy(m_str_copy, str.GetCString(), str.GetSize());
      m_str = m_str_copy;
    }
    inline StrAs(const BasicString<ThreadSafe>& str) : m_str_copy(new char[str.GetSize()])
    {
      memcpy(m_str_copy, str.GetCString(), str.GetSize());
      m_str = m_str_copy;
    }
    inline ~StrAs() { delete [] m_str_copy; }
    
    inline operator bool() const;
    
#if APTO_PLATFORM(WINDOWS)
    inline operator int() const { return strtol(m_str, NULL, 0); }
    inline operator long() const { return strtol(m_str, NULL, 0); }
    inline operator long long() const { return _strtoi64(m_str, NULL, 0); }
    inline operator unsigned int() const { return strtoul(m_str, NULL, 0); }
    inline operator unsigned long() const { return strtoul(m_str, NULL, 0); }
    inline operator unsigned long long() const { return _strtoui64(m_str, NULL, 0); }
      
    inline operator float() const { return (float)strtod(m_str, NULL); }
    inline operator double() const { return strtod(m_str, NULL); }
    inline operator long double() const { return (long double)strtod(m_str, NULL); }
#else
    inline operator int() const { return (int)strtol(m_str, NULL, 0); }
    inline operator long() const { return strtol(m_str, NULL, 0); }
    inline operator long long() const { return strtoll(m_str, NULL, 0); }
    inline operator unsigned int() const { return (unsigned int)strtoul(m_str, NULL, 0); }
    inline operator unsigned long() const { return strtoul(m_str, NULL, 0); }
    inline operator unsigned long long() const { return strtoull(m_str, NULL, 0); }
      
    inline operator float() const { return strtof(m_str, NULL); }
    inline operator double() const { return strtod(m_str, NULL); }
    inline operator long double() const { return strtold(m_str, NULL); }
#endif

    
    inline operator const char*() const { return m_str; }
  };
  
  inline StrAs::operator bool() const
  {
    size_t size = strlen(m_str);
    if (size == 1) {
      if (m_str[0] == '1' || m_str[0] == 'T' || m_str[0] == 't') return true;
      else return false;
    }
    if (size == 4 && (m_str[0] == 'T' || m_str[0] == 't') && (m_str[1] == 'R' || m_str[1] == 'r') &&
                     (m_str[2] == 'U' || m_str[2] == 'u') && (m_str[3] == 'E' || m_str[3] == 'e')) {
      return true;
    }
    return false;
  }
  
  
  // AsStr - Convert native types into strings
  // --------------------------------------------------------------------------------------------------------------
  
  template <class T> class ConvertToStr
  {
  private:
    T m_value;
    
  public:
    ConvertToStr(T value) : m_value(value) { ; }
    
    template <class M> inline operator Apto::BasicString<M>() const { return Apto::BasicString<M>(m_value); }
  };
  
  
  template <> class ConvertToStr<char>
  {
  private:
    char m_str[2];
    
  public:
    ConvertToStr(char value) { m_str[0] = value; m_str[1] = '\0'; }
    
    template <class M> inline operator Apto::BasicString<M>() const { return Apto::BasicString<M>(m_str); }
  };
  
  template <> class ConvertToStr<int>
  {
  private:
    char m_str[15];
    
  public:
    ConvertToStr(int value) { sprintf(m_str, "%d", value); }
    
    template <class M> inline operator Apto::BasicString<M>() const { return Apto::BasicString<M>(m_str); }
  };
  
  template <> class ConvertToStr<double>
  {
  private:
    char m_str[20];
    
  public:
    ConvertToStr(double value) { sprintf(m_str, "%f", value); }
    
    template <class M> inline operator Apto::BasicString<M>() const { return Apto::BasicString<M>(m_str); }
  };
  
  template <> class ConvertToStr<bool>
  {
  private:
    const char* m_str;
    
  public:
    ConvertToStr(bool value) : m_str(value ? "true" : "false") { ; }
    
    template <class M> inline operator Apto::BasicString<M>() const { return Apto::BasicString<M>(m_str); }
  };

  template <class M> class ConvertToStr<Apto::BasicString<M> >
  {
  private:
    const Apto::BasicString<M>& m_str;
    
  public:
    ConvertToStr(const Apto::BasicString<M>& value) : m_str(value) { ; }
    
    inline operator const Apto::BasicString<M>&() const { return m_str; }
  };

  template <class T> inline ConvertToStr<T> AsStr(T value) { return ConvertToStr<T>(value); }
  
  
  // String formatting
  // --------------------------------------------------------------------------------------------------------------
  
  class FormatStr
  {
  private:
    char* m_buffer;
    
  public:
    FormatStr(const char* format, ...) : m_buffer(NULL)
    {
      va_list args;
      va_start(args, format);
      
      // Determine the string length after formatting
#if APTO_PLATFORM(WINDOWS)
      int len = _vscprintf(format, args) + 1;
#else
      int len = vsnprintf(NULL, 0, format, args) + 1;
#endif
      
      va_end(args);
      va_start(args, format);
      
      // Allocate buffer of appropriate size
      m_buffer = new char[len];
      
      // Format string into newly allocated buffer
      if (vsnprintf(m_buffer, len, format, args) < 0) {
        // set buffer to empty string if formatting fails
        m_buffer[0] = '\0';          
      }
      
      va_end(args);
    }
    inline ~FormatStr() { delete [] m_buffer; }
    
    template <class T>
    inline operator BasicString<T>() const { return BasicString<T>(m_buffer); }
  };
  
};

#endif
