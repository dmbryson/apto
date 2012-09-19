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

#include "apto/core/Array.h"
#include "apto/core/ArrayUtils.h"
#include "apto/core/Definitions.h"
#include "apto/core/MapStorage.h"
#include "apto/core/TypeUtil.h"


namespace Apto {

  // Set -- Internals
  // --------------------------------------------------------------------------------------------------------------
  
  namespace Internal {
    class VoidSetEntry
    {
    public:
      inline void Inc() { ; }
      inline void Dec() { ; }
      inline int Count() const { return 1; }
      inline void Clear() { ; }
    };
    
    class CountSetEntry
    {
    private:
      int m_count;
    public:
      inline CountSetEntry() : m_count(0) { ; }
      inline void Inc() { m_count++; }
      inline void Dec() { m_count--; }
      inline int Count() const { return m_count; }
      inline void Clear() { m_count = 0; }
    };
  }

  
  // MultiSet
  // --------------------------------------------------------------------------------------------------------------
  
  enum { Multi = true };  
  
  // Set
  // --------------------------------------------------------------------------------------------------------------
  
  template<class T, template <class, class> class StoragePolicy = DefaultHashBTree, bool MultiSet = false>
  class Set : public StoragePolicy<T, typename TypeSelect<MultiSet, Internal::CountSetEntry, Internal::VoidSetEntry>::Result>
  {
  protected:
    typedef typename TypeSelect<MultiSet, Internal::CountSetEntry, Internal::VoidSetEntry>::Result Entry;
    typedef StoragePolicy<T, Entry> SP;
    
    int m_multiset_size;
    
  public:
    typedef T ValueType;
    class Iterator;
    class ConstIterator;
    
  public:
    Set() : m_multiset_size(0) { ; }
    Set(const Set& rhs) : SP(), m_multiset_size(0) { this->operator=(rhs); }

    template <class T1, template <class, class> class SP1, bool M>
    explicit Set(const Set<T1, SP1, M>& rhs) { this->operator=(rhs); }
    
    inline int GetSize() const { return (MultiSet) ? m_multiset_size : SP::GetSize(); }
    
    inline void Clear()
    {
      if (MultiSet) {
        typename SP::ValueIterator it = SP::Values();
        while (it.Next()) it.Get()->Clear();
      }
      SP::Clear();
    }

    
    template <class T1, template <class, class> class SP1, bool M>
    Set& operator=(const Set<T1, SP1, M>& rhs)
    {
      Clear();
      typename Set<T1, SP1, M>::ConstIterator it = rhs.Begin();
      while (it.Next(MultiSet)) {
        if (MultiSet) m_multiset_size++;
        SP::Get(*it.Get()).Inc();
      }
      return *this;
    }
    
    template <class T1, template <class, class> class SP1>
    inline bool operator==(const Set<T1, SP1, !MultiSet>& rhs) const { (void)rhs; return false; }
    
    template <class T1, template <class, class> class SP1>
    bool operator==(const Set<T1, SP1, MultiSet>& rhs) const
    {
      if (GetSize() != rhs.GetSize()) return false;
      
      if (SP::Sorted && Set<T1, SP1, MultiSet>::Sorted) {
        ConstIterator it1 = Begin();
        typename Set<T1, SP1, MultiSet>::ConstIterator it2 = rhs.Begin();
        while (it1.Next() && it2.Next()) if (*it1.Get() != *it2.Get()) return false;
        return true;
      } else if (SP::Sorted) {
        ConstIterator it1 = Begin();
        typename Set<T1, SP1, MultiSet>::ConstIterator it2 = rhs.Begin();
        Array<T1> array2(rhs.GetSize());
        for (int i = 0; it2.Next(); i++) array2[i] = *it2.Get();
        QSort(array2);
        for (int i = 0; it1.Next(); i++) if (*it1.Get() != array2[i]) return false;
        return true;
      } else if (Set<T1, SP1, MultiSet>::Sorted) {
        ConstIterator it1 = Begin();
        Array<T> array1(GetSize());
        for (int i = 0; it1.Next(); i++) array1[i] = *it1.Get();
        QSort(array1);
        typename Set<T1, SP1, MultiSet>::ConstIterator it2 = rhs.Begin();
        for (int i = 0; it2.Next(); i++) if (array1[i] != *it2.Get()) return false;
        return true;
      }

      ConstIterator it1 = Begin();
      typename Set<T1, SP1, MultiSet>::ConstIterator it2 = rhs.Begin();
      Array<T> array1(GetSize());
      Array<T1> array2(rhs.GetSize());
      for (int i = 0; it1.Next() && it2.Next(); i++) {
        array1[i] = *it1.Get();
        array2[i] = *it2.Get();
      }
      QSort(array1);
      QSort(array2);
      
      for (int i = 0; i < array1.GetSize(); i++)
        if (array1[i] != array2[i])
          return false;
      return true;
    }

    template <class T1, template <class, class> class SP1, bool M>
    bool operator!=(const Set<T1, SP1, M>& rhs) const { return !operator==(rhs); }
    
    inline void Insert(const T& key)
    {
      if (MultiSet) m_multiset_size++;
      SP::Get(key).Inc();
    }
    
    inline bool Has(const T& key) const { return (SP::Find(key)); }
    
    inline int Count(const T& key) { return SP::Get(key).Count(); }
    
    
    inline Iterator Begin() { return Iterator(this); }
    inline ConstIterator Begin() const { return ConstIterator(this); }
    
    bool Remove(const T& key)
    {
      if (MultiSet) {
        Entry* entry = SP::Find(key);
        if (!entry) return false;
        entry->Dec();
        m_multiset_size--;
        if (entry->Count() == 0) SP::Remove(key);
        return true;
      } else {
        return SP::Remove(key);
      }
    }
    
  public:
    class Iterator
    {
      friend class Set<T, StoragePolicy, MultiSet>;
    private:
      typename Set<T, StoragePolicy, MultiSet>::SP::Iterator m_it;
      int m_count;
      
      Iterator(); // @not_implemented
      Iterator(Set<T, StoragePolicy, MultiSet>* set) : m_it(set->SP::Begin()), m_count(0) { ; }
      
    public:      
      T* Get() { return (m_it.Get()) ? &m_it.Get()->Value1() : NULL; }
      T* Next(bool use_multi = MultiSet)
      {
        if (MultiSet && use_multi) {
          if (m_count > 1) {
            m_count--;
            return &m_it.Get()->Value1();
          } else {
            if (m_it.Next()) {
              m_count = m_it.Get()->Value2()->Count();
              return &m_it.Get()->Value1();
            } else {
              return NULL;
            }
          }
        } else {
          m_count = 0;
          return (m_it.Next()) ? &m_it.Get()->Value1() : NULL;
        }
      }
    };

    class ConstIterator
    {
      friend class Set<T, StoragePolicy, MultiSet>;
    private:
      typename Set<T, StoragePolicy, MultiSet>::SP::ConstIterator m_it;
      int m_count;
      
      ConstIterator(); // @not_implemented
      ConstIterator(const Set<T, StoragePolicy, MultiSet>* set) : m_it(set->SP::Begin()), m_count(0) { ; }
      
    public:      
      const T* Get() { return (m_it.Get()) ? &m_it.Get()->Value1() : NULL; }
      const T* Next(bool use_multi = MultiSet)
      {
        if (MultiSet && use_multi) {
          if (m_count > 1) {
            m_count--;
            return &m_it.Get()->Value1();
          } else {
            if (m_it.Next()) {
              m_count = m_it.Get()->Value2()->Count();
              return &m_it.Get()->Value1();
            } else {
              return NULL;
            }
          }
        } else {
          m_count = 0;
          return (m_it.Next()) ? &m_it.Get()->Value1() : NULL;
        }
      }
    };
  };
};
    
#endif
