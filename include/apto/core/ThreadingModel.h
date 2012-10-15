/*
 *  ThreadingModel.h
 *  Apto
 *
 *  Created by David on 10/10/12.
 *  Copyright 2012 David Michael Bryson. All rights reserved.
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

#ifndef AptoCoreThreadingModel_h
#define AptoCoreThreadingModel_h

#include "apto/core/Atomic.h"
#include "apto/core/Mutex.h"


namespace Apto {
  
  // SingleThreaded
  // --------------------------------------------------------------------------------------------------------------

  template <class T> class SingleThreaded
  {
  public:
    typedef T VolatileType;
    typedef int AtomicInt;
    
    class ClassLock
    {
    public:
      inline ClassLock() { ; }
    };
    
    static inline int Add(AtomicInt& atomic, int value) { return atomic += value; }
    static inline int Get(AtomicInt& atomic) { return atomic; }
    static inline void Set(AtomicInt& atomic, int value) { atomic = value; }
    
    static inline void Inc(AtomicInt& atomic) { atomic++; }
    static inline bool DecAndTest(AtomicInt& atomic) { return (Add(atomic, -1) == 0); }
  };

  
  // ThreadSafe
  // --------------------------------------------------------------------------------------------------------------
  
  template <class T> class ThreadSafe
  {
  private:
    class ClassLockMutex;
    
  public:
    typedef volatile T VolatileType;
    typedef volatile int AtomicInt;
    
    class ClassLock
    {
    public:
      inline ClassLock() { ClassLockMutex::Get().Lock(); }
      inline ~ClassLock() { ClassLockMutex::Get().Unlock(); }
    };
    
    static inline int Add(AtomicInt& atomic, int value) { return Atomic::Add(&atomic, value); }
    static inline int Get(AtomicInt& atomic) { return Atomic::Get(&atomic); }
    static inline void Set(AtomicInt& atomic, int value) { Atomic::Set(&atomic, value); }
    
    static inline void Inc(AtomicInt& atomic) { Atomic::Inc(&atomic); }
    static inline bool DecAndTest(AtomicInt& atomic) { return Atomic::DecAndTest(&atomic); }
    
  private:
    class ClassLockMutex
    {
    public:
      static ClassLockMutex& Get()
      {
        static ClassLockMutex clmutex;
        return clmutex;
      }
      
      inline void Lock() { m_mutex.Lock(); }
      inline void Unlock() { m_mutex.Unlock(); }
      
    private:
      Mutex m_mutex;
      
      inline ClassLockMutex() { ; }
      inline ~ClassLockMutex() { ; }
      
      ClassLockMutex(const ClassLockMutex&);
      ClassLockMutex& operator=(const ClassLockMutex&);
    };
  };

};

#endif
