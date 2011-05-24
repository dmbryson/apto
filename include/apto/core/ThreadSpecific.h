/*
 *  ThreadSpecific.h
 *  Apto
 *
 *  Created by David on 10/14/08.
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

#ifndef AptoCoreThreadSpecific_h
#define AptoCoreThreadSpecific_h

#include "apto/core/Definitions.h"
#include "apto/platform/Platform.h"


#if APTO_PLATFORM(THREADS) && APTO_PLATFORM(UNIX)

// Use POSIX Threads
#include <pthread.h>

namespace Apto {
  template <typename T> class ThreadSpecific
  {
  private:
    pthread_key_t m_key;
    static void destroySpecific(void* data) { delete (T*)data; }
    
    ThreadSpecific(const ThreadSpecific&); // @not_implemented
    ThreadSpecific& operator=(const ThreadSpecific&); // @not_implemented
    
  public:
    ThreadSpecific() { pthread_key_create(&m_key, &destroySpecific); }
    ~ThreadSpecific() { destroySpecific(pthread_getspecific(m_key)); pthread_key_delete(m_key); }
    
    T* Get() { return (T*)pthread_getspecific(m_key); }
    void Set(T* value) { destroySpecific(pthread_getspecific(m_key)); pthread_setspecific(m_key, (void*)value); }
  };
};

#elif APTO_PLATFORM(THREADS) && APTO_PLATFORM(WINDOWS)

// Use Windows Threads
#include <Windows.h>

namespace Apto {
  template <typename T> class ThreadSpecific
  {
  private:
     DWORD m_key;
     static void destroySpecific(void* data) { delete (T*)data; }

     ThreadSpecific(const ThreadSpecific&); // @not_implemented
     ThreadSpecific& operator=(const ThreadSpecific&); // @not_implemented

  public:
    ThreadSpecific() { m_key = TlsAlloc(); }
    ~ThreadSpecific() { destroySpecific(TlsGetValue(m_key)); TlsFree(m_key); }

    T* Get() { return (T*)TlsGetValue(m_key); }
    void Set(T* value) { destroySpecific(TlsGetValue(m_key)); TlsSetValue(m_key, value); }
  };
};

#else

// Disable Threading
namespace Apto {
  template <typename T> class ThreadSpecific
  {
  private:
    T* m_value;

    ThreadSpecific(const ThreadSpecific&); // @not_implemented
    ThreadSpecific& operator=(const ThreadSpecific&); // @not_implemented
    
  public:
    ThreadSpecific() : m_value(NULL) { ; }
    ~ThreadSpecific() { delete m_value; }
    
    T* Get() { return m_value; }
    void Set(T* value) { delete m_value; m_value = value; }
  };
};

#endif


#endif
