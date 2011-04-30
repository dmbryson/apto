/*
 *  Mutex.h
 *  Apto
 *
 *  Created by David on 3/2/07.
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

#ifndef AptoCoreMutex_h
#define AptoCoreMutex_h

#include "apto/core/Definitions.h"
#include "apto/platform/Platform.h"

#if APTO_PLATFORM(THREADS) && APTO_PLATFORM(UNIX)

// Use POSIX Threads
# include <pthread.h>

#ifdef DEBUG
# define APTO_PTHREAD_MUTEX_CHKRTN(OP) \
{ \
int ret = OP; \
ASSERT_MSG(ret == 0, "OP failed"); \
}
#else
# define APTO_PTHREAD_MUTEX_CHKRTN(OP) OP
#endif

namespace Apto {  
  class Mutex
  {
    friend class ConditionVariable;
  private:
    pthread_mutex_t m_mutex;
#ifdef DEBUG
    bool m_locked;
#endif
    
    Mutex(const Mutex&); // @not_implemented
    Mutex& operator=(const Mutex&); // @not_implemented
    
    
  public:
#ifdef DEBUG
    Mutex() : m_locked(false)
    {
      APTO_PTHREAD_MUTEX_CHKRTN(pthread_mutex_init(&m_mutex, NULL));
    }
#else
    inline Mutex() { APTO_PTHREAD_MUTEX_CHKRTN(pthread_mutex_init(&m_mutex, NULL)); }
#endif
    
    inline ~Mutex()
    {
      ASSERT_MSG(!m_locked, "destroying locked mutex");
      APTO_PTHREAD_MUTEX_CHKRTN(pthread_mutex_destroy(&m_mutex));
    }
    
#ifdef DEBUG
    void Lock()
    {
      APTO_PTHREAD_MUTEX_CHKRTN(pthread_mutex_lock(&m_mutex));
      m_locked = true;
    }
#else
    inline void Lock() { APTO_PTHREAD_MUTEX_CHKRTN(pthread_mutex_lock(&m_mutex)); }
#endif
  
#ifdef DEBUG
    void Unlock()
    {
      ASSERT_MSG(m_locked, "mutex not locked");
      m_locked = false;
      APTO_PTHREAD_MUTEX_CHKRTN(pthread_mutex_unlock(&m_mutex));
    }
#else
    inline void Unlock() { APTO_PTHREAD_MUTEX_CHKRTN(pthread_mutex_unlock(&m_mutex)); }
#endif
  };
};

#undef APTO_PTHREAD_MUTEX_CHKRTN


#elif APTO_PLATFORM(THREADS) && APTO_PLATFORM(WINDOWS)

// Use Windows Threading
# include <windows.h>

namespace Apto {  
  class Mutex
  {
    friend class ConditionVariable;
  private:
    CRITICAL_SECTION m_mutex;
#ifdef DEBUG
    bool m_locked;
#endif
  
    Mutex(const Mutex&); // @not_implemented
    Mutex& operator=(const Mutex&); // @not_implemented
    
    
  public:
#ifdef DEBUG
    Mutex() : m_locked(false) { InitializeCriticalSection(&m_mutex); }
#else
    inline Mutex() { InitializeCriticalSection(&m_mutex); }
#endif
    
    inline ~Mutex()
    {
      ASSERT_MSG(!m_locked, "destroying locked mutex");
      DeleteCriticalSection(&m_mutex);
    }
    
#ifdef DEBUG
    void Lock()
    {
      EnterCriticalSection(&m_mutex);
      m_locked = true;
    }
#else
    inline void Lock() { EnterCriticalSection(&m_mutex); }
#endif
  
#ifdef DEBUG
    void Unlock()
    {
      ASSERT_MSG(m_locked, "mutex not locked");
      m_locked = false;
      LeaveCriticalSection(&m_mutex);
    }
#else
    inline void Unlock() { LeaveCriticalSection(&m_mutex); }
#endif
  };
};

#else

// Disable Threading
namespace Apto {  
  class Mutex
  {
  private:
#ifdef DEBUG
    bool m_locked;
#endif
  
    Mutex(const Mutex&); // @not_implemented
    Mutex& operator=(const Mutex&); // @not_implemented
  
  
  public:
#ifdef DEBUG
    Mutex() : m_locked(false) { ; }
#else
    inline Mutex() { ; }
#endif
  
    inline ~Mutex() { ASSERT_MSG(!m_locked, "destroying locked mutex"); }
  
#ifdef DEBUG
    void Lock() { m_locked = true; }
#else
    inline void Lock() { ; }
#endif
  
#ifdef DEBUG
    void Unlock()
    {
      ASSERT_MSG(m_locked, "mutex not locked");
      m_locked = false;
    }
#else
    inline void Unlock() { ; }
#endif  
  };
};

#endif


namespace Apto {  
  class MutexAutoLock
  {
  private:
    Mutex& m_mutex;
    
    MutexAutoLock(); // @not_implemented
    MutexAutoLock(const MutexAutoLock&); // @not_implemented
    MutexAutoLock& operator=(const MutexAutoLock&); // @not_implemented
    
  public:
    inline explicit MutexAutoLock(Mutex& mutex) : m_mutex(mutex) { m_mutex.Lock(); }
    inline ~MutexAutoLock() { m_mutex.Unlock(); }
  };
};

#endif
