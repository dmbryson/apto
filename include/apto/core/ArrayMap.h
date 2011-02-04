/*
 *  ArrayMap.h
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

#ifndef AptoCoreArrayMap_h
#define AptoCoreArrayMap_h

#include "apto/core/Array.h"
#include "apto/core/Pair.h"


namespace Apto {
  template<class KeyType, class ValueType> class tArrayMap
  {
  private:
    Array<Pair<KeyType, ValueType> > m_map;
    
  public:
    ArrayMap() { ; }
    ArrayMap(const ArrayMap& am) : m_map(am.m_map) { ; }
    
    int GetSize() const { return m_map.GetSize(); }
    
    void Set(const KeyType& key, const ValueType& value)
    {
      for (int i = 0; i < m_map.GetSize(); i++) {
        if (m_map[i].Value1() == key) {
          m_map[i].Value2() = value;
          return;
        }
      }
      m_map.Push(Pair<KeyType, ValueType>(key, value));
    }
    
    bool Get(const KeyType& key, ValueType& out_value) const
    {
      for (int i = 0; i < m_map.GetSize(); i++) {
        if (m_map[i].Value1() == key) {
          out_value = m_map[i].Value2();
          return true;
        }
      }
      return false;
    }
    
    const ValueType& GetWithDefault(const KeyType& key, const ValueType& default_value)
    {
      for (int i = 0; i < m_map.GetSize(); i++) {
        if (m_map[i].Value1() == key) return m_map[i].Value2();
      }
      m_map.Push(Pair<KeyType, ValueType>(key, default_value));
      return m_map[m_map.GetSize() - 1].Value2();
    }
    
    ValueType& ValueFor(const KeyType& key)
    {
      for (int i = 0; i < m_map.GetSize(); i++) {
        if (m_map[i].Value1() == key) {
          return m_map[i].Value2();
        }
      }
      m_map.Push(Pair<KeyType, ValueType>(key));
      return m_map[m_map.GetSize() - 1].Value2();
    }
    
    const ValueType& ValueFor(const KeyType& key) const
    {
      for (int i = 0; i < m_map.GetSize(); i++) {
        if (m_map[i].Value1() == key) {
          return m_map[i].Value2();
        }
      }
      m_map.Push(Pair<KeyType, ValueType>(key));
      return m_map[m_map.GetSize() - 1].Value2();
    }
    
    ValueType& operator[](const KeyType& key) { return ValueFor(key); }
    const ValueType& operator[](const KeyType& key) const { return ValueFor(key); }
    
    Array<KeyType> GetKeys() const
    {
      Array<KeyType> keys(m_map.GetSize());
      for (int i = 0; i < m_map.GetSize(); i++) keys[i] = m_map[i].Value1();
      return keys;
    }  
    
    void Remove(const KeyType& key)
    {
      for (int i = 0; i < m_map.GetSize(); i++) {
        if (m_map[i].Value1() == key) {
          int lastkv = m_map.GetSize() - 1;
          if (i != lastkv) m_map[i] = m_map[lastkv];
          m_map.Resize(lastkv);
        }
      }
    }
    
    bool Remove(const KeyType& key, ValueType& out_value)
    {
      for (int i = 0; i < m_map.GetSize(); i++) {
        if (m_map[i].Value1() == key) {
          int lastkv = m_map.GetSize() - 1;
          out_value = m_map[i].Value2();
          if (i != lastkv) m_map[i] = m_map[lastkv];
          m_map.Resize(lastkv);
          return true;
        }
      }
      return false;
    }    
  };
};

#endif
