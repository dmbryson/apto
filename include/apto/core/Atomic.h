/*
 *  Atomic.h
 *  Apto
 *
 *  Created by David on 2/4/11.
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
 */

#ifndef AptoCoreAtomic_h
#define AptoCoreAtomic_h

#include "apto/platform/Platform.h"


namespace Apto {
  namespace Atomic {
    inline int Add(volatile int* atomic, int value);
    inline int Get(volatile int* atomic);
    inline void Set(volatile int* atomic, int value);
    
    inline void Inc(volatile int* atomic) { Add(atomic, 1); }
    inline bool DecAndTest(volatile int* atomic) { return (Add(atomic, -1) == 0); }
  };
};


#if APTO_PLATFORM(APPLE)

#include <libkern/OSAtomic.h>

inline int Apto::Atomic::Add(volatile int* atomic, int value)
{
  return OSAtomicAdd32Barrier(value, atomic);
}

inline int Apto::Atomic::Get(volatile int* atomic)
{
  OSMemoryBarrier();
  return *atomic;
}

inline void Apto::Atomic::Set(volatile int* atomic, int value)
{
  OSMemoryBarrier();
  *atomic = value;
}

#elif APTO_PLATFORM(WINDOWS)

#include <Windows.h>

inline int Apto::Atomic::Add(volatile int* atomic, int value)
{
  return InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(atomic), static_cast<LONG>(value)) + value;
}

inline int Apto::Atomic::Get(volatile int* atomic)
{
  MemoryBarrier();
  return *atomic;
}

inline void Apto::Atomic::Set(volatile int* atomic, int value)
{
  MemoryBarrier();
  *atomic = value;
}

#else

// Assuming x86
inline int Apto::Atomic::Add(volatile int* atomic, int value)
{
  int temp = value;
  __asm__ __volatile__("lock; xaddl %0,%1"
                       : "+r" (temp), "+m" (*atomic)
                       : : "memory");
  return temp + value;
}

inline int Apto::Atomic::Get(volatile int* atomic)
{
  __asm__ __volatile__("" : : : "memory");
  return *atomic;
}

inline void Apto::Atomic::Set(volatile int* atomic, int value)
{
  __asm__ __volatile__("mfence" : : : "memory");
  *atomic = value;
}

#endif


#endif
