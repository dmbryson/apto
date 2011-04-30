/*
 *  List.h
 *  Apto
 *
 *  Created by David on 2/10/10.
 *  Copyright 2010-2011 David Michael Bryson. All rights reserved.
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

#ifndef AptoCoreList_h
#define AptoCoreList_h

#include "apto/core/Definitions.h"
#include "apto/core/ListStorage.h"

#include <cassert>


namespace Apto {    
  
  // List
  // --------------------------------------------------------------------------------------------------------------  
  
  template <class T, template <class> class StoragePolicy = DL>
  class List : public StoragePolicy<T>
  {
  protected:
    typedef StoragePolicy<T> SP;
    
  public:
    typedef typename StoragePolicy<T>::EntryHandle EntryHandle;
    typedef typename StoragePolicy<T>::Iterator Iterator;
    typedef typename StoragePolicy<T>::ConstIterator ConstIterator;
    
  public:
    List() { ; }
    List(const List& rhs) : SP() { this->operator=(rhs); }
    
    template <class T1, template <class> class SP1>
    explicit inline List(const List<T1, SP1>& rhs)
    {      
      typename List<T1, SP1>::Iterator it = rhs.Begin();
      while (it.Next()) PushRear(*it.Get());
    }
    
    ~List() { ; }
    
    inline List& operator=(const List& rhs) { SP::operator=(rhs); return *this; }
    
    template <class T1, template <class> class SP1>
    inline List& operator=(const List<T1, SP1>& rhs)
    {
      Clear();
      typename List<T1, SP1>::Iterator it = rhs.Begin();
      while (it.Next()) PushRear(*it.Get());
    }
    
    template <class T1, template <class> class SP1>
    inline List& operator+=(const List<T1, SP1>& rhs)
    {
      int rhs_size = rhs.GetSize();
      typename List<T1, SP1>::ConstIterator it = rhs.Begin();
      for (it.Next(); rhs_size > 0; it.Next(), rhs_size--) PushRear(*it.Get());
      return *this;
    }
    

    template <class T1, template <class> class SP1>
    inline List operator+(const List<T1, SP1>& rhs) const
    {
      List new_list;
      ConstIterator it1 = Begin();
      while (it1.Next()) new_list.PushRear(*it1.Get());
      typename List<T1, SP1>::ConstIterator it2 = rhs.Begin();
      while (it2.Next()) new_list.PushRear(*it2.Get());
      return new_list;
    }
    
    template <class T1, template <class> class SP1>
    inline bool operator==(const List<T1, SP1>& rhs)
    {
      if (SP::GetSize() != rhs.GetSize()) return false;
      Iterator it1 = Begin();
      typename List<T1, SP1>::ConstIterator it2 = rhs.Begin();
      while (it1.Next() && it2.Next()) if (*it1.Get() != *it2.Get()) return false;
      return true;
    }

    template <class T1, template <class> class SP1>
    inline bool operator!=(const List<T1, SP1>& rhs) { return !operator==(rhs); }
    
    
    inline int GetSize() const { return SP::GetSize(); }
    
    inline void Clear() { SP::Clear(); }
    
    inline void Push(const T& value, EntryHandle** handle = NULL) { SP::Push(value, handle); }
    inline void PushRear(const T& value, EntryHandle** handle = NULL) { SP::PushRear(value, handle); }
    inline T Pop() { return SP::Pop(); }
    inline T PopRear() { return SP::PopRear(); }
    
    inline T& GetFirst() { assert(GetSize()); return SP::GetFirst(); }
    inline const T& GetFirst() const { assert(GetSize()); return SP::GetFirst(); }
    inline T& GetLast() { assert(GetSize()); return SP::GetLast(); }
    inline const T& GetLast() const { assert(GetSize()); return SP::GetLast(); }
    
    inline bool Remove(const T& value) { return SP::Remove(value); }
    
    inline bool Contains(const T& value)
    {
      Iterator it = SP::Begin();
      while (it.Next()) if (*it.Get() == value) return true;
      return false;
    }
    
    inline Iterator Begin() { return SP::Begin(); }
    inline ConstIterator Begin() const { return SP::Begin(); }
  };
  
};

#endif
