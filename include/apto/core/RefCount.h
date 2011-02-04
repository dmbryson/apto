/*
 *  RefCount.h
 *  Apto
 *
 *  Created by David on 11/12/08.
 *  Copyright 2008-2011 David Michael Bryson. All rights reserved.
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

#ifndef AptoCoreRefCount_h
#define AptoCoreRefCount_h

#include "apto/core/Atomic.h"


namespace Apto {
  class RCObject
  {
  private:
    volatile int m_ref_count;
    
  public:
    RCObject() { Atomic::Set(&m_ref_count, 0); }
    RCObject(const RCObject&) { Atomic::Set(&m_ref_count, 0); }
    virtual ~RCObject() = 0;
    
    RCObject& operator=(const RCObject&) { return *this; }
    
    void AddReference() { Atomic::Inc(&m_ref_count); }
    void RemoveReference() { if (Atomic::DecAndTest(&m_ref_count)) delete this; }
  };
  
  
  template<class T> class RCPtr
  {
  private:
    T* m_ptr;
    
    void handleNewPointer();
    
  public:
    RCPtr(T* ptr = NULL) : m_ptr(ptr) { m_ptr->AddReference(); }
    RCPtr(const RCPtr& rhs) : m_ptr(rhs.m_ptr) { m_ptr->AddReference(); }
    ~RCPtr() { if (m_ptr) m_ptr->RemoveReference(); }
    
    RCPtr& operator=(const RCPtr& rhs);
    
    inline bool operator!() const { return !m_ptr; }
    inline operator bool() const { return !operator!(); }
    
    inline T* operator->() const { return m_ptr; }
    inline T& operator*() const { return *m_ptr; }
    
    template<class S> inline S* operator->() const { return m_ptr; }
    template<class S> inline S& operator*() const { return *m_ptr; }
    
    template<class S> operator RCPtr<S>() { return RCPtr<S>(m_ptr); }
  };
  
  template<class T> RCPtr<T>& RCPtr<T>::operator=(const RCPtr& rhs)
  {
    if (m_ptr != rhs.m_ptr) {
      rhs.m_ptr->AddReference();
      if (m_ptr) m_ptr->RemoveReference();
      m_ptr = rhs.m_ptr;
    }
    
    return *this;
  }
  
};

#endif
