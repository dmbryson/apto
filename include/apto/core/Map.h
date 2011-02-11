/*
 *  Map.h
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

#ifndef AptoCoreMap_h
#define AptoCoreMap_h

#include "apto/core/ArrayStorage.h"
#include "apto/core/Iterator.h"
#include "apto/core/Pair.h"


namespace Apto {
  template<class KeyType, class ValueType, template <class> StoragePolicy = Basic>
  class Map : public StoragePolicy<Pair<KeyType, ValueType> >
  {
  protected:
    typedef StoragePolicy<Pair<KeyType, ValueType> > SP;
    
  public:
    Map() { ; }
    template <template <class> SP1> Map(const Map<KeyType, ValueType, SP1>& rhs) { this->operator=(rhs); }
    
    inline int GetSize() const { return SP::GetSize(); }
    
    void Set(const KeyType& key, const ValueType& value)
    {
      for (int i = 0; i < GetSize(); i++) {
        if (SP::operator[](i).Value1() == key) {
          SP::operator[](i).Value2() = value;
          return;
        }
      }
      SP::Resize(GetSize() + 1);
      SP::operator[](GetSize()) = Pair<KeyType, ValueType>(key, value);
    }
    
    bool Get(const KeyType& key, ValueType& out_value) const
    {
      for (int i = 0; i < GetSize(); i++) {
        if (SP::operator[](i).Value1() == key) {
          out_value = SP::operator[](i).Value2();
          return true;
        }
      }
      return false;
    }
    
    const ValueType& GetWithDefault(const KeyType& key, const ValueType& default_value)
    {
      for (int i = 0; i < GetSize(); i++) {
        if (SP::operator[](i).Value1() == key) {
          return SP::operator[](i).Value2();
        }
      }
      SP::Resize(GetSize() + 1);
      SP::operator[](GetSize()) = Pair<KeyType, ValueType>(key, default_value);
      return SP::operator[](GetSize() - 1).Value2();
    }
    
    ValueType& ValueFor(const KeyType& key)
    {
      for (int i = 0; i < GetSize(); i++) {
        if (SP::operator[](i).Value1() == key) {
          return SP::operator[](i).Value2();
        }
      }
      SP::Resize(GetSize() + 1);
      SP::operator[](GetSize()) = Pair<KeyType, ValueType>(key);
      return SP::operator[](GetSize() - 1).Value2();
    }
    
    const ValueType& ValueFor(const KeyType& key) const
    {
      for (int i = 0; i < GetSize(); i++) {
        if (SP::operator[](i).Value1() == key) {
          return SP::operator[](i).Value2();
        }
      }
      SP::Resize(GetSize() + 1);
      SP::operator[](GetSize()) = Pair<KeyType, ValueType>(key);
      return SP::operator[](GetSize() - 1).Value2();
    }
    
    ValueType& operator[](const KeyType& key) { return ValueFor(key); }
    const ValueType& operator[](const KeyType& key) const { return ValueFor(key); }
    
    ConstIterator<KeyType> Keys() const { return KeyIterator(*this); }
    ConstIterator<ValueType> Values() const { return ValueIterator(*this); }
    ConstIterator<Pair<KeyType, ValueType> Iterator() const { return PairIterator(*this); }
    
    void Remove(const KeyType& key)
    {
      for (int i = 0; i < GetSize(); i++) {
        if (SP::operator[](i).Value1() == key) {
          int lastkv = GetSize() - 1;
          if (i != lastkv) SP::operator[](i) = SP::operator[](lastkv);
          SP::Resize(lastkv);
        }
      }
    }
    
    bool Remove(const KeyType& key, ValueType& out_value)
    {
      for (int i = 0; i < GetSize(); i++) {
        if (SP::operator[](i).Value1() == key) {
          int lastkv = GetSize() - 1;
          out_value = SP::operator[](i).Value2();
          if (i != lastkv) SP::operator[](i) = SP::operator[](lastkv);
          SP::Resize(lastkv);
          return true;
        }
      }
      return false;
    }
    
  protected:
    class KeyIterator : public Apto::ConstIterator<KeyType>
    {
    private:
      const Map& m_map;
      int m_index;
      
      KeyIterator(); // @not_implemented
      
    public:
      KeyIterator(const Map& map) : m_map(map), m_index(0) { ; }
      
      const KeyType* Get() { return (m_index < m_map.GetSize()) ? &(m_map.SP::operator[](m_index).Value1()) : NULL; }
      const KeyType* Next() { return (++m_index < m_map.GetSize()) ? &(m_map.SP::operator[](m_index).Value1()) : NULL; }
    };
    
    class ValueIterator : public Apto::ConstIterator<ValueType>
    {
    private:
      const Map& m_map;
      int m_index;
      
      ValueIerator(); // @not_implemented
      
    public:
      ValueIterator(const Map& map) : m_map(map), m_index(0) { ; }
      
      const ValueType* Get() { return (m_index < m_map.GetSize()) ? &(m_map.SP::operator[](m_index).Value2()) : NULL; }
      const ValueType* Next() { return (++m_index < m_map.GetSize()) ? &(m_map.SP::operator[](m_index).Value2()) : NULL; }
    };
    
    class PairIterator : public Apto::ConstIterator<Pair<KeyType, ValueType> >
    {
    private:
      const Map& m_map;
      int m_index;
      
      PairIterator(); // @not_implemented
      
    public:
      PairIterator(const Map& map) : m_map(map), m_index(0) { ; }
      
      const Pair<KeyType, ValueType>* Get() { return (m_index < m_map.GetSize()) ? &(m_map.SP::operator[](m_index)) : NULL; }
      const Pair<KeyType, ValueType>* Next() { return (++m_index < m_map.GetSize()) ? &(m_map.SP::operator[](m_index)) : NULL; }
    };
  };
};

#endif
