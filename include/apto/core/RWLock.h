/*
 *  RWLock.h
 *  Apto
 *
 *  Created by David on 3/3/07.
 *  Copyright 2007-2011 David Michael Bryson. All rights reserved.
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

#ifndef AptoCoreRWLock_h
#define AptoCoreRWLock_h

#include "apto/core/Definitions.h"
#include "apto/platform/Platform.h"


#if APTO_PLATFORM(THREADS) && APTO_PLATFORM(UNIX)

// Use POSIX Threads
# include <pthread.h>

namespace Apto {
  class RWLock
  {
  private:
    pthread_rwlock_t m_rwlock;
    
    RWLock(const RWLock&); // @not_implemented
    RWLock& operator=(const RWLock&); // @not_implemented
    
  public:
    inline RWLock() { pthread_rwlock_init(&m_rwlock, NULL); }
    inline ~RWLock() { pthread_rwlock_destroy(&m_rwlock); }
    
    inline void ReadLock() { pthread_rwlock_rdlock(&m_rwlock); }
    inline void ReadUnlock() { pthread_rwlock_unlock(&m_rwlock); }  
    
    inline void WriteLock() { pthread_rwlock_wrlock(&m_rwlock); }
    inline void WriteUnlock() { pthread_rwlock_unlock(&m_rwlock); }  
  };
};


#elif APTO_PLATFORM(THREADS) && APTO_PLATFORM(WINDOWS)

// Use Windows Threading
# include <windows.h>

namespace Apto {
  class RWLock
  {
  private:
    SRWLOCK m_rwlock;
    
    RWLock(const RWLock&); // @not_implemented
    RWLock& operator=(const RWLock&); // @not_implemented
    
  public:
    inline RWLock() { InitializeSRWLock(&m_rwlock); }
    inline ~RWLock() { ; }
    
    inline void ReadLock() { AcquireSRWLockShared(&m_rwlock); }
    inline void ReadUnlock() { ReleaseSRWLockShared(&m_rwlock); }  
    
    inline void WriteLock() { AcquireSRWLockExclusive(&m_rwlock); }
    inline void WriteUnlock() { ReleaseSRWLockExclusive(&m_rwlock); }  
  };
};


#else

// Disable Threading
namespace Apto {
  class RWLock
  {
  private:
    RWLock(const RWLock&); // @not_implemented
    RWLock& operator=(const RWLock&); // @not_implemented
    
  public:
    inline RWLock() { ; }
    inline ~RWLock() { ; }
    
    inline void ReadLock() { ; }
    inline void ReadUnlock() { ; }  
    
    inline void WriteLock() { ; }
    inline void WriteUnlock() { ; }  
  };
};


#endif


namespace Apto {
  template<class T> class RLockPtr
  {
  private:
    T* m_ptr;
    RWLock* m_rwlock;
    
    
  public:
    RLockPtr(T* ptr, RWLock* rwlock) : m_ptr(ptr), m_rwlock(rwlock) { m_rwlock->ReadLock(); }
    RLockPtr(const RLockPtr& p) : m_ptr(p.m_ptr), m_rwlock(p.m_rwlock) { m_rwlock->ReadLock(); }
    ~RLockPtr() { m_rwlock->ReadUnlock(); }
    
    RLockPtr& operator=(const RLockPtr& rhs);
    
    inline bool operator!() const { return !m_ptr; }
    inline operator bool() const { return !operator!(); }
    
    inline T* operator->() const { return m_ptr; }
    inline T& operator*() const { return *m_ptr; }
    
    template<class S> inline S* operator->() const { return m_ptr; }
    template<class S> inline S& operator->() const { return *m_ptr; }
    
    template<class S> operator RLockPtr<S>() { return RLockPtr<S>(m_ptr, m_rwlock); }
  };

  template<class T> RLockPtr<T>& RLockPtr<T>::operator=(const RLockPtr& rhs)
  {
    if (m_ptr != rhs.m_ptr) {
      m_rwlock->ReadUnlock();
      m_ptr = rhs.m_ptr;
      m_rwlock = rhs.m_rwlock;
      m_rwlock->ReadLock();
    }
    
    return *this;
  }
};


#endif
