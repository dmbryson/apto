/*
 *  FileSystem.cc
 *  Apto
 *
 *  Created by David on 12/7/05.
 *  Copyright 2005-2011 David Michael Bryson. All rights reserved.
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

#include "apto/core/Thread.h"

#if APTO_PLATFORM(THREADS) && APTO_PLATFORM(UNIX)

Apto::Thread::~Thread() { if (m_running) pthread_detach(m_thread); }

bool Apto::Thread::Start()
{
  m_mutex.Lock();
  if (!m_running) {
    m_running = true;
    m_mutex.Unlock();
    
    // Create thread, mark as running when successful
    int rval = pthread_create(&m_thread, NULL, &Thread::EntryPoint, this);
    if (rval) m_running = false;
  } else {
    m_mutex.Unlock();
  }
  
  return m_running;
}

void Apto::Thread::Stop()
{
  if (m_running) {
    // Close and clean up thread, set running to false
    pthread_cancel(m_thread);
    pthread_join(m_thread, NULL);
    m_running = false;
  }
}

void Apto::Thread::Join()
{
  if (m_running) {
    pthread_join(m_thread, NULL);
    m_running = false;
  }
}

void* Apto::Thread::EntryPoint(void* arg)
{
  // Common entry point to start Thread objects
  // Calls Run method of appropriate subclass to do the actual work
  Thread* thread = static_cast<Thread*>(arg);
  thread->Run();
  
  pthread_exit(NULL);
  return NULL;
}


#elif APTO_PLATFORM(THREADS) && APTO_PLATFORM(WINDOWS)

#include <process.h>

Apto::Thread::~Thread() { if (m_thread) CloseHandle(m_thread); }

bool Apto::Thread::Start()
{
  MutexAutoLock lock(m_mutex);
  
  if (m_thread == 0) {
    unsigned int tid = 0;
    m_thread = (HANDLE)_beginthreadex(0, 0, &Thread::EntryPoint, this, 0, &tid);
    
    if (m_thread) return true;
    return false;
  }
  
  return true;
}

void Apto::Thread::Stop()
{
  if (m_thread) {
    if (CloseHandle(m_thread) == TRUE) {
      m_thread = 0;
    }
  }
  
}

void Apto::Thread::Join()
{
  if (m_thread) WaitForSingleObject(m_thread, INFINITE);
}

unsigned int __stdcall Apto::Thread::EntryPoint(void* arg)
{
  // Common entry point to start Thread objects
  // Calls Run method of appropriate subclass to do the actual work
  Thread* thread = static_cast<Thread*>(arg);
  thread->Run();
  
  _endthreadex(0);
  return 0;
}


#endif
