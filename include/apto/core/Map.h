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

#include "apto/core/Array.h"
#include "apto/core/ArrayUtils.h"
#include "apto/core/MapStorage.h"
#include "apto/core/Pair.h"


namespace Apto {
    
  // Map
  // --------------------------------------------------------------------------------------------------------------
  
  template<class K, class V, template <class, class, int> class StoragePolicy = HashBTree, int HashFactor = 42,
           template <class, int> class HashFunctor = HashKey>
  class Map : public StoragePolicy<K, V, HashFactor>
  {
  protected:
    typedef StoragePolicy<K, V, HashFactor> SP;
    typedef HashFunctor<K, HashFactor> HF;
    
  public:
    typedef K KeyType;
    typedef V ValueType;
    typedef typename StoragePolicy<KeyType, ValueType, HashFactor>::Iterator Iterator;
    typedef typename StoragePolicy<KeyType, ValueType, HashFactor>::ConstIterator ConstIterator;
    typedef typename StoragePolicy<KeyType, ValueType, HashFactor>::KeyIterator KeyIterator;
    typedef typename StoragePolicy<KeyType, ValueType, HashFactor>::ValueIterator ValueIterator;

  public:
    Map() { ; }
    Map(const Map& rhs) { this->operator=(rhs); }
    
    template <class K1, class V1, template <class, class, int> class SP1, int F, template <class, int> class H>
    explicit Map(const Map<K1, V1, SP1, F, H>& rhs) { this->operator=(rhs); }
    
    inline int GetSize() const { return SP::GetSize(); }
    
    template <class K1, class V1, template <class, class, int> class SP1, int F, template <class, int> class H>
    Map& operator=(const Map<K1, V1, SP1, F, H>& rhs)
    {
      if (this == &rhs) return *this;
      
      Clear();
      typename Map<K, V, SP1, F, H>::ConstIterator it = rhs.Begin();
      while (it.Next()) Get(*it.Key()) = *it.Value();
      return *this;
    }
    
    template <class K1, class V1, template <class, class, int> class SP1, int F, template <class, int> class H>
    bool operator==(const Map<K1, V1, SP1, F, H>& rhs)
    {
      if (SP::GetSize() != rhs.GetSize()) return false;
      
      // Get and sort local keys
      KeyIterator kit1 = Keys();
      Array<KeyType> keys1(SP::GetSize());
      for (int i = 0; kit1.Next(); i++) keys1[i] = *kit1.Get();
      QSort(keys1);
      
      // Get and sort rhs keys
      typename Map<K1, V1, SP1, F, H>::KeyIterator kit2 = rhs.Keys();
      Array<K1> keys2(rhs.GetSize());
      for (int i = 0; kit2.Next(); i++) keys2[i] = *kit2.Get();
      
      for (int i = 0; i < keys1.GetSize(); i++) {
        ValueType v;
        if (keys1[i] != keys2[i] || !rhs.Get(keys2[i], v) || Get(keys1[i]) != v) return false;
      }
      
      return true;
    }

    template <class K1, class V1, template <class, class, int> class SP1, int F, template <class, int> class H>
    inline bool operator!=(const Map<K1, V1, SP1, F, H>& rhs) { return !operator==(rhs); }

    inline void Clear() { SP::Clear(); }
    
    inline bool Has(const KeyType& key) const { return (SP::Find(key, HF::Hash(key))); }
    
    inline ValueType& Get(const KeyType& key) { return SP::Get(key, HashFunctor<KeyType, HashFactor>::Hash(key)); }
    inline ValueType& operator[](const KeyType& key) { return Get(key); }
            
    
    inline bool Get(const KeyType& key, ValueType& out_value) const { 
      const ValueType* entry_value = SP::Find(key, HF::Hash(key));
      if (entry_value) {
        out_value = *entry_value;
        return true;
      }
      return false;
    }
    
    ValueType& GetWithDefault(const KeyType& key, const ValueType& default_value)
    {
      ValueType* entry_value = SP::Find(key, HF::Hash(key));
      if (entry_value) return *entry_value;
      ValueType& new_value = Get(key);
      new_value = default_value;
      return new_value;
    }
    
    inline void Set(const KeyType& key, const ValueType& value) { Get(key) = value; }
    
    
    inline bool Remove(const KeyType& key) { return SP::Remove(key, HashFunctor<KeyType, HashFactor>::Hash(key)); }
    
    
    Iterator Begin() { return SP::Begin(); }
    ConstIterator Begin() const { return SP::Begin(); }
    
    KeyIterator Keys() const { return SP::Keys(); }
    ValueIterator Values() const { return SP::Values(); }
  };
};

#endif
