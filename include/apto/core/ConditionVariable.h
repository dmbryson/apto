/*
 *  ConditionVariable.h
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

#ifndef AptoCoreConditionVariable_h
#define AptoCoreConditionVariable_h

#include "apto/core/Mutex.h"
#include "apto/platform/Platform.h"

#if APTO_PLATFORM(THREADS) && APTO_PLATFORM(UNIX)

// Use POSIX Threads
# include <pthread.h>

namespace Apto {
  class ConditionVariable
  {
  private:
    pthread_cond_t m_cond;
    
    ConditionVariable(const ConditionVariable&); // @not_implemented
    ConditionVariable& operator=(const ConditionVariable&); // @not_implemented
    
    
  public:
    inline ConditionVariable() { pthread_cond_init(&m_cond, NULL); }
    inline ~ConditionVariable() { pthread_cond_destroy(&m_cond); }
    
#ifdef DEBUG
    void Wait(Mutex& mutex)
    {
      mutex.m_locked = false;
      pthread_cond_wait(&m_cond, &(mutex.m_mutex));
      mutex.m_locked = true;
    }
#else
    inline void Wait(Mutex& mutex) { pthread_cond_wait(&m_cond, &(mutex.m_mutex)); }
#endif
    
    inline void Signal() { pthread_cond_signal(&m_cond); }
    inline void Broadcast() { pthread_cond_broadcast(&m_cond); }
  };
};

#elif APTO_PLATFORM(THREADS) && APTO_PLATFORM(WINDOWS)

// Use Windows Threading
# include <windows.h>

namespace Apto {
  class ConditionVariable
  {
  private:
    CONDITION_VARIABLE m_cond;
    
    ConditionVariable(const ConditionVariable&); // @not_implemented
    ConditionVariable& operator=(const ConditionVariable&); // @not_implemented
    
    
  public:
    inline ConditionVariable() { InitializeConditionVariable(&m_cond); }
    inline ~ConditionVariable() { ; }
    
#ifdef DEBUG
    void Wait(Mutex& mutex)
    {
      mutex.m_locked = false;
      SleepConditionVariableCS(&m_cond, &(mutex.m_mutex));
      mutex.m_locked = true;
    }
#else
    inline void Wait(Mutex& mutex) { SleepConditionVariableCS(&m_cond, &(mutex.m_mutex), INFINITE); }
#endif
    
    inline void Signal() { WakeConditionVariable(&m_cond); }
    inline void Broadcast() { WakeAllConditionVariable(&m_cond); }
  };
};


#else

// Disable Threading
namespace Apto {
  class ConditionVariable
  {
  private:
    ConditionVariable(const ConditionVariable&); // @not_implemented
    ConditionVariable& operator=(const ConditionVariable&); // @not_implemented
    
  public:
    inline ConditionVariable() { ; }
    inline ~ConditionVariable() { ; }
    
    inline void Wait(Mutex& mutex) { ; }
    
    inline void Signal() { ; }
    inline void Broadcast() { ; }
  };
};

#endif

#endif
