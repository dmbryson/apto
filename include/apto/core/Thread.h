/*
 *  Thread.h
 *  Apto
 *
 *  Created by David on 2/18/06.
 *  Copyright 2006-2011 David Michael Bryson. All rights reserved.
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

#ifndef AptoCoreThread_h
#define AptoCoreThread_h

#include "apto/core/Mutex.h"
#include "apto/platform/Platform.h"


#if APTO_PLATFORM(THREADS) && APTO_PLATFORM(UNIX)

// Use POSIX Threads
#include <pthread.h>

namespace Apto {
  class Thread
  {
  protected:
    pthread_t m_thread;
    Mutex m_mutex;
    bool m_running;
    
    void Stop();
    
    virtual void Run() = 0;
    
    static void* EntryPoint(void* arg);
    
    Thread(const Thread&); // @not_implemented
    Thread& operator=(const Thread&); // @not_implemented
    
  public:
    Thread() : m_running(false) { ; }
    virtual ~Thread();
    
    bool Start();
    void Join();
  };
};

#elif APTO_PLATFORM(THREADS) && APTO_PLATFORM(WINDOWS)

// Use Windows Threading
#include <Windows.h>

namespace Apto {
  class Thread
  {
  protected:
    HANDLE m_thread;
    Mutex m_mutex;
    
    LIB_EXPORT void Stop();
    
    LIB_EXPORT virtual void Run() = 0;
    
    LIB_EXPORT static unsigned int __stdcall EntryPoint(void* arg);
    
    Thread(const Thread&); // @not_implemented
    Thread& operator=(const Thread&); // @not_implemented
    
  public:
    LIB_EXPORT inline Thread() : m_thread(0) { ; }
    LIB_EXPORT virtual ~Thread();
    
    LIB_EXPORT bool Start();
    LIB_EXPORT void Join();
  };
};

#else

// Disable Threading
namespace Apto {
  class Thread
  {
  protected:
    virtual void Run() = 0;
    
    Thread(const Thread&); // @not_implemented
    Thread& operator=(const Thread&); // @not_implemented
    
  public:
    inline Thread() { ; }
    virtual ~Thread() { ; }
    
    inline bool Start() { Run(); return true; }
    inline void Join() { ; }
  };
};

#endif


#endif
