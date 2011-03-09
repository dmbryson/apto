/*
 *  Set.h
 *  Apto
 *
 *  Created by David on 1/11/09.
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

#ifndef AptoCoreSet_h
#define AptoCoreSet_h

#include "apto/core/ArrayStorage.h"


namespace Apto {
  template<class T, template <class> class StoragePolicy = Basic>
  class Set : public StoragePolicy<T>
  {
  protected:
    typedef StoragePolicy<T> SP;
    
  public:
    Set() { ; }
    template <class T1, template <class> class SP1> Set(const Set<T1, SP1>& rhs) { this->operator=(rhs); }
    
    inline int GetSize() const { return SP::GetSize(); }
    
    
    template <class T1, template <class> class SP1>
    Set& operator=(const Set<T1, SP1>& rhs)
    {
      SP::Resize(rhs.GetSize());
      for (int i = 0; i < rhs.GetSize(); i++) SP::operator[](i) = rhs[i];
      return *this;
    }
    
    void Add(const T& value)
    {
      for (int i = 0; i < GetSize(); i++) {
        if (SP::operator[](i) == value) {
          return;
        }
      }
      SP::Resize(GetSize() + 1);
      SP::operator[](GetSize()) = value;
    }
    
    bool Has(const T& value)
    {
      for (int i = 0; i < GetSize(); i++) if (SP::operator[](i) == value) return true;
      return false;
    }    
    
    
    inline const T& operator[](int index) const { return SP::operator[](index); }
    
    inline ConstIterator<T> Iterator() const { return ValueIterator(*this); }
    
    bool Remove(const T& value)
    {
      for (int i = 0; i < GetSize(); i++) {
        if (SP::operator[](i) == value) {
          int lastkv = GetSize() - 1;
          if (i != lastkv) SP::operator[](i) = SP::operator[](lastkv);
          SP::Resize(lastkv);
          return true;
        }
      }
      return false;
    }
    
  protected:    
    class ValueIterator : public Apto::ConstIterator<T>
    {
    private:
      const Set& m_set;
      int m_index;
      
      ValueIterator(); // @not_implemented
      
    public:
      ValueIterator(const Set& set) : m_set(set), m_index(0) { ; }
      
      const T* Get() { return (m_index < m_set.GetSize()) ? &(m_set.SP::operator[](m_index).Value2()) : NULL; }
      const T* Next() { return (++m_index < m_set.GetSize()) ? &(m_set.SP::operator[](m_index).Value2()) : NULL; }
    };
    
  };
};
    
#endif
