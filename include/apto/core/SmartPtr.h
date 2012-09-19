/*
 *  SmartPtr.h
 *  Apto
 *
 *  Created by David on 2/7/11.
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
 *  SmartPtr based off of ideas presented in Modern C++ Design (2001) by Andrei Alexandrescu
 */

#ifndef AptoCoreSmartPtr_h
#define AptoCoreSmartPtr_h

#include "apto/core/Atomic.h"
#include "apto/core/Definitions.h"
#include "apto/core/RefTransport.h"
#include "apto/core/StaticCheck.h"
#include "apto/core/TypeUtil.h"

#include "apto/platform/Visibility.h"

#include <cassert>
#include <functional>
#include <string>


namespace Apto {
  
  // Storage Policies
  // --------------------------------------------------------------------------------------------------------------
  template <class T> class HeapStorage
  {
  public:
    typedef T* StoredType;
    typedef T* InitPointerType;
    typedef T* PointerType;
    typedef const T* ConstPointerType;
    typedef T& ReferenceType;
    typedef const T& ConstReferenceType;
    
  protected:
    inline HeapStorage() : m_ptr(Default()) { ; }
    inline HeapStorage(const HeapStorage&) : m_ptr(NULL) { ; }
    template <class U> inline HeapStorage(const HeapStorage<U>&) : m_ptr(NULL) { ; }
    explicit inline HeapStorage(const StoredType& p) : m_ptr(p) { ; }

    
    inline PointerType operator->() const { return m_ptr; }
    inline ReferenceType operator*() const { return *m_ptr; }
    
    void Swap(HeapStorage& rhs)
    {
      StoredType temp = m_ptr;
      m_ptr = rhs.m_ptr;
      rhs.m_ptr = temp;
    }
    
    template <class U> friend typename HeapStorage<U>::PointerType GetInternalPtr(const HeapStorage<U>&); 
    template <class U> friend typename HeapStorage<U>::StoredType& GetInternalPtrRef(HeapStorage<U>&); 
    template <class U> friend const typename HeapStorage<U>::StoredType& GetInternalPtrRef(const HeapStorage<U>&);     
    
    void Destroy()
    {
      if (m_ptr) {
        delete m_ptr;
        m_ptr = NULL;
      }
    }
    
    static StoredType Default() { return NULL; }
    
  private:
    StoredType m_ptr;
  };
  
  template <class T>
  inline typename HeapStorage<T>::PointerType GetInternalPtr(const HeapStorage<T>& sp) { return sp.m_ptr; }
  
  template <class T>
  inline typename HeapStorage<T>::StoredType& GetInternalPtrRef(HeapStorage<T>& sp) { return sp.m_ptr; }
  
  template <class T>
  inline const typename HeapStorage<T>::StoredType& GetInternalPtrRef(const HeapStorage<T>& sp) { return sp.m_ptr; }
  

  template <class T> class ArrayStorage
  {
  public:
    typedef T* StoredType;
    typedef T* InitPointerType;
    typedef T* PointerType;
    typedef const T* ConstPointerType;
    typedef T& ReferenceType;
    typedef const T& ConstReferenceType;
    
  protected:
    inline ArrayStorage() : m_ptr(Default()) { ; }
    inline ArrayStorage(const ArrayStorage&) : m_ptr(NULL) { ; }
    template <class U> inline ArrayStorage(const ArrayStorage<U>&) : m_ptr(NULL) { ; }
    explicit inline ArrayStorage(const StoredType& p) : m_ptr(p) { ; }
    
    
    inline PointerType operator->() const { return m_ptr; }
    inline ReferenceType operator*() const { return *m_ptr; }
    
    void Swap(ArrayStorage& rhs)
    {
      StoredType temp = m_ptr;
      m_ptr = rhs.m_ptr;
      rhs.m_ptr = temp;
    }
    
    template <class U> friend typename ArrayStorage<U>::PointerType GetInternalPtr(const ArrayStorage<U>&); 
    template <class U> friend typename ArrayStorage<U>::StoredType& GetInternalPtrRef(ArrayStorage<U>&); 
    template <class U> friend const typename ArrayStorage<U>::StoredType& GetInternalPtrRef(const ArrayStorage<U>&);     
    
    void Destroy()
    {
      if (m_ptr) {
        delete [] m_ptr;
        m_ptr = NULL;
      }
    }
    
    static StoredType Default() { return NULL; }
    
  private:
    StoredType m_ptr;
  };
  
  template <class T>
  inline typename ArrayStorage<T>::PointerType GetInternalPtr(const ArrayStorage<T>& sp) { return sp.m_ptr; }
  
  template <class T>
  inline typename ArrayStorage<T>::StoredType& GetInternalPtrRef(ArrayStorage<T>& sp) { return sp.m_ptr; }
  
  template <class T>
  inline const typename ArrayStorage<T>::StoredType& GetInternalPtrRef(const ArrayStorage<T>& sp) { return sp.m_ptr; }


  
  // Ownership Policies
  // --------------------------------------------------------------------------------------------------------------
  template <class T> class RefCount
  {
    template <class T1> friend class RefCount;
  private:
    int* m_p_refs;
    
  protected:
    RefCount() : m_p_refs(new int)
    {
      assert(m_p_refs);
      *m_p_refs = 1;
    }
    RefCount(const RefCount& rhs) : m_p_refs(rhs.m_p_refs) { ; }
    
    template <class T1>
    RefCount(const RefCount<T1>& rhs) : m_p_refs(rhs.m_p_refs) { ; }
    
    T Clone(const T& value)
    {
      ++*m_p_refs;
      return value;
    }
    
    bool Release(const T&)
    {
      if (!--*m_p_refs) {
        delete m_p_refs;
        m_p_refs = NULL;
        return true;
      }
      return false;
    }
    
    void Swap(RefCount& rhs)
    {
      int* temp = m_p_refs;
      m_p_refs = rhs.m_p_refs;
      rhs.m_p_refs = temp;
    }
    
    enum { CopyIsDestructive = false };
  };

  
  template <class T> class ThreadSafeRefCount
  {
    template <class T1> friend class ThreadSafeRefCount;
  private:
    volatile int* m_p_refs;
    
  protected:
    ThreadSafeRefCount() : m_p_refs(new int)
    {
      assert(m_p_refs);
      Atomic::Set(m_p_refs, 1);
    }
    ThreadSafeRefCount(const ThreadSafeRefCount& rhs) : m_p_refs(rhs.m_p_refs) { ; }
    
    template <class T1>
    ThreadSafeRefCount(const ThreadSafeRefCount<T1>& rhs) : m_p_refs(rhs.m_p_refs) { ; }
    
    T Clone(const T& value)
    {
      Atomic::Inc(m_p_refs);
      return value;
    }
    
    bool Release(const T&)
    {
      if (Atomic::DecAndTest(m_p_refs)) {
        delete m_p_refs;
        m_p_refs = NULL;
        return true;
      }
      return false;
    }
    
    void Swap(ThreadSafeRefCount& rhs)
    {
      volatile int* temp = m_p_refs;
      m_p_refs = rhs.m_p_refs;
      rhs.m_p_refs = temp;
    }
    
    enum { CopyIsDestructive = false };
  };
  
  
  template <class T> class InternalRCObject
  {
  protected:
    InternalRCObject() { ; }
    InternalRCObject(const InternalRCObject& rhs) { (void)rhs; }
    
    template <class T1>
    InternalRCObject(const InternalRCObject<T1>& rhs) { (void)rhs; }
    
    static T Clone(const T& value)
    {
      if (value) value->AddReference();
      return value;
    }
    
    static bool Release(const T& value)
    {
      if (value) value->RemoveReference();
      return false;
    }
    
    static void Swap(InternalRCObject& rhs) { (void)rhs; }
    
    enum { CopyIsDestructive = false };
  };

  
  template <class T> class DeepCopy
  {
  protected:
    DeepCopy() { ; }
    DeepCopy(const DeepCopy& rhs) { (void)rhs; }
    
    template <class T1>
    DeepCopy(const DeepCopy<T1>& rhs) { (void)rhs; }
    
    static T Clone(const T& value) { return value->Clone(); }
    
    static bool Release(const T& value) { (void)value; return true; }
    
    static void Swap(DeepCopy& rhs) { (void)rhs; }
    
    enum { CopyIsDestructive = false };
  };
  

  template <class T> class DestructiveCopy
  {
  protected:
    DestructiveCopy() { ; }
    DestructiveCopy(const DestructiveCopy& rhs) { (void)rhs; }
    
    template <class T1>
    DestructiveCopy(const DestructiveCopy<T1>& rhs) { (void)rhs; }
    
    template <class T1>
    static T Clone(const T1& value) {
      T rval(value);
      const_cast<T1&>(value) = T1();
      return rval;
    }
    
    template <class T1>
    static T Clone(T1& value) {
      T rval(value);
      value = T1();
      return rval;
    }
    
    static bool Release(const T& value) { (void)value; return true; }
    
    static void Swap(DestructiveCopy& rhs) { (void)rhs; }
    
    enum { CopyIsDestructive = true };
  };
  
  
  template <class T> class NoCopy
  {
  protected:
    NoCopy() { ; }
    NoCopy(const NoCopy& rhs) { (void)rhs; }
    
    static T Clone(const T& value)
    {
      (void)value;
      static const bool DependedFalse = (sizeof(T*) == 0);  // False value dependent upon template parameter
      APTO_STATIC_CHECK(DependedFalse, NoCopy_Policy_Disallows_Value_Copying);
    }
    
    static bool Release(const T& value) { (void)value; return true; }
    
    static void Swap(NoCopy& rhs) { (void)rhs; }
    
    enum { CopyIsDestructive = false };
  };
  
  
  
  // SmartPtr
  // --------------------------------------------------------------------------------------------------------------
  template <
    typename T,
    template <class> class OwnershipPolicy = RefCount,
    template <class> class StoragePolicy = HeapStorage
  >
  class SmartPtr : public StoragePolicy<T>, public OwnershipPolicy<typename StoragePolicy<T>::InitPointerType>
  {
    typedef OwnershipPolicy<typename StoragePolicy<T>::InitPointerType> OP;
    typedef StoragePolicy<T> SP;
    
  public:
    typedef typename SP::PointerType PointerType;
    typedef typename SP::ConstPointerType ConstPointerType;
    typedef typename SP::StoredType StoredType;
    typedef typename SP::ReferenceType ReferenceType;
    typedef typename SP::ConstReferenceType ConstReferenceType;
    typedef typename TypeSelect<OP::CopyIsDestructive, SmartPtr, const SmartPtr>::Result CopyArgType;
    
    SmartPtr() : SP(), OP() { ; }
    explicit SmartPtr(const StoredType& ptr) : SP(ptr), OP() { ; }
    SmartPtr(CopyArgType& rhs) : SP(rhs), OP(rhs) { GetInternalPtrRef(*this) = OP::Clone(GetInternalPtrRef(rhs)); }
    
    template <typename T1, template<class> class OP1, template<class> class SP1>
    SmartPtr(SmartPtr<T1, OP1, SP1>& rhs) : SP(rhs), OP(rhs)
    {
      GetInternalPtrRef(*this) = OP::Clone(GetInternalPtrRef(rhs));
    }
    
    template <typename T1, template<class> class OP1, template<class> class SP1>
    SmartPtr(const SmartPtr<T1, OP1, SP1>& rhs) : SP(rhs), OP(rhs)
    {
      GetInternalPtrRef(*this) = OP::Clone(GetInternalPtrRef(rhs));
    }
    
    // Reference Transport to allow immediate use of SmartPtr return values
    SmartPtr(RefTransport<SmartPtr> rhs) : SP(rhs), OP(rhs)
    {
      GetInternalPtrRef(*this) = OP::Clone(GetInternalPtrRef(static_cast<SmartPtr&>(rhs)));
    }
    inline operator RefTransport<SmartPtr>() { return RefTransport<SmartPtr>(*this); }
    
    SmartPtr& operator=(CopyArgType& rhs)
    {
      SmartPtr temp(rhs);
      temp.Swap(*this);
      return *this;
    }
    
    template <typename T1, template<class> class OP1, template<class> class SP1>
    SmartPtr& operator=(const SmartPtr<T1, OP1, SP1>& rhs)
    {
      SmartPtr temp(rhs);
      temp.Swap(*this);
      return *this;
    }

    template <typename T1, template<class> class OP1, template<class> class SP1>
    SmartPtr& operator=(SmartPtr<T1, OP1, SP1>& rhs)
    {
      SmartPtr temp(rhs);
      temp.Swap(*this);
      return *this;
    }
    
    ~SmartPtr() { if (OP::Release(GetInternalPtr(*static_cast<SP*>(this)))) SP::Destroy(); }

    
    void Swap(SmartPtr& rhs)
    {
      OP::Swap(rhs);
      SP::Swap(rhs);
    }
    
    // Pointer usage
    inline PointerType operator->() { return SP::operator->(); }
    inline ConstPointerType operator->() const { return SP::operator->(); }
    
    inline ReferenceType operator*() { return SP::operator*(); }
    inline ConstReferenceType operator*() const { return SP::operator*(); }
    
    static inline T* GetPointer(const SmartPtr& ptr) { return GetInternalPtr(ptr); }

    
    // Conditional operators
    inline bool operator!() const { return GetInternalPtr(*this) == NULL; }

    template <typename T1, template<class> class OP1, template<class> class SP1>
    inline bool operator==(const SmartPtr<T1, OP1, SP1>& rhs) const { return GetInternalPtr(*this) == GetInternalPtr(rhs); }

    template <typename T1, template<class> class OP1, template<class> class SP1>
    inline bool operator!=(const SmartPtr<T1, OP1, SP1>& rhs) const { return !(*this == rhs); }

    template <typename T1, template<class> class OP1, template<class> class SP1>
    inline bool operator<(const SmartPtr<T1, OP1, SP1>& rhs) const { return GetInternalPtr(*this) < GetInternalPtr(rhs); }

    template <typename T1, template<class> class OP1, template<class> class SP1>
    inline bool operator>(const SmartPtr<T1, OP1, SP1>& rhs) const { return GetInternalPtr(rhs) < GetInternalPtr(*this); }
    
    template <typename T1, template<class> class OP1, template<class> class SP1>
    inline bool operator<=(const SmartPtr<T1, OP1, SP1>& rhs) const { return !(GetInternalPtr(rhs) < GetInternalPtr(*this)); }

    template <typename T1, template<class> class OP1, template<class> class SP1>
    inline bool operator>=(const SmartPtr<T1, OP1, SP1>& rhs) const { return !(GetInternalPtr(*this) < GetInternalPtr(rhs)); }
    
  private:
    struct Tester
    {
      LIB_EXPORT void Placeholder() { ; }
    };
    
    typedef void (Tester::*PlaceholderBooleanType)();

  public:
    inline operator PlaceholderBooleanType() const { return (GetInternalPtr(*this) ? &Tester::Placeholder : NULL); }
    
    
  private:
    class DynamicCastPlaceholder { ; };
    
    template <typename T1, template<class> class OP1, template<class> class SP1>
    SmartPtr(SmartPtr<T1, OP1, SP1>& rhs, const DynamicCastPlaceholder& placeholder) : SP(rhs), OP(rhs)
    {
      (void)placeholder;
      PointerType casted_ptr = dynamic_cast<PointerType>(GetInternalPtrRef(rhs));
      GetInternalPtrRef(*this) = OP::Clone(casted_ptr);
    }
    
    template <typename T1, template<class> class OP1, template<class> class SP1>
    SmartPtr(const SmartPtr<T1, OP1, SP1>& rhs, const DynamicCastPlaceholder& placeholder) : SP(rhs), OP(rhs)
    {
      (void)placeholder;
      PointerType casted_ptr = dynamic_cast<PointerType>(GetInternalPtrRef(rhs));
      GetInternalPtrRef(*this) = OP::Clone(casted_ptr);
    }
    
  public:
    template <typename T1, template<class> class OP1, template<class> class SP1>
    SmartPtr& DynamicCastFrom(SmartPtr<T1, OP1, SP1>& rhs)
    {
      SmartPtr temp(rhs, DynamicCastPlaceholder());
      temp.Swap(*this);
      return *this;
    }

    template <typename T1, template<class> class OP1, template<class> class SP1>
    SmartPtr& DynamicCastFrom(const SmartPtr<T1, OP1, SP1>& rhs)
    {
      SmartPtr temp(rhs, DynamicCastPlaceholder());
      temp.Swap(*this);
      return *this;
    }
  };

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator==(const SmartPtr<T, OP, SP>& lhs, U* rhs) { return GetInternalPtr(lhs) == rhs; }

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator==(U* lhs, const SmartPtr<T, OP, SP>& rhs) { return lhs == GetInternalPtr(rhs); }

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator!=(const SmartPtr<T, OP, SP>& lhs, U* rhs) { return GetInternalPtr(lhs) != rhs; }

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator!=(U* lhs, const SmartPtr<T, OP, SP>& rhs) { return lhs != GetInternalPtr(rhs); }

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator<(const SmartPtr<T, OP, SP>& lhs, U* rhs) { return GetInternalPtr(lhs) < rhs; }

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator<(U* lhs, const SmartPtr<T, OP, SP>& rhs) { return lhs < GetInternalPtr(rhs); }

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator>(const SmartPtr<T, OP, SP>& lhs, U* rhs) { return GetInternalPtr(lhs) > rhs; }

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator>(U* lhs, const SmartPtr<T, OP, SP>& rhs) { return lhs > GetInternalPtr(rhs); }

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator<=(const SmartPtr<T, OP, SP>& lhs, U* rhs) { return GetInternalPtr(lhs) <= rhs; }

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator<=(U* lhs, const SmartPtr<T, OP, SP>& rhs) { return lhs <= GetInternalPtr(rhs); }

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator>=(const SmartPtr<T, OP, SP>& lhs, U* rhs) { return GetInternalPtr(lhs) >= rhs; }

  template <typename T, template<class> class OP, template<class> class SP, typename U>
  inline bool operator>=(U* lhs, const SmartPtr<T, OP, SP>& rhs) { return lhs >= GetInternalPtr(rhs); }
};


namespace std {
  template <typename T, template <class> class OP, template <class> class SP>
  struct less< Apto::SmartPtr<T, OP, SP> >
    : public binary_function<Apto::SmartPtr<T, OP, SP>, Apto::SmartPtr<T, OP, SP>, bool>
  {
  public:
    inline bool operator()(const Apto::SmartPtr<T, OP, SP>& lhs, const Apto::SmartPtr<T, OP, SP>& rhs) const
    {
      return less<typename Apto::SmartPtr<T, OP, SP>::StoredType>()(Apto::GetInternalPtr(lhs), Apto::GetInternalPtr(rhs));
    }
  };
};

#endif
