/*
 *  TCFreeList.h
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

#ifndef AptoMallocTCFreeList_h
#define AptoMallocTCFreeList_h

#include "apto/core/Singleton.h"
#include "apto/core/ThreadSpecific.h"

#include <cstdlib>


namespace Apto {
  namespace Malloc {
    
    // TCFreeList - Thread Caching Free List - Assumes equal sized requests
    // --------------------------------------------------------------------------------------------------------------
    
    template <class SuperAlloc> class TCFreeList : public SuperAlloc
    {
    private:
      struct FreeBlock;
      struct FreeList;
      
    public:      
      static inline void* Allocate(std::size_t size)
      {
        FreeList* fl = SingletonHolder<ThreadSpecific<FreeList>, CreateWithNew, DestroyAtExit, ThreadSafe>::Instance().Get();
        if (!fl) {
          fl = new FreeList;
          SingletonHolder<ThreadSpecific<FreeList>, CreateWithNew, DestroyAtExit, ThreadSafe>::Instance().Set(fl);
        }
        
        if (fl->head) {
          FreeBlock* fb = fl->head;
          fl->head = fb->next;
          return fb;
        }
        
        return SuperAlloc::Allocate(size);
      }
    
      static inline void Deallocate(void* ptr)
      {
        FreeList* fl = SingletonHolder<ThreadSpecific<FreeList>, CreateWithNew, DestroyAtExit, ThreadSafe>::Instance().Get();
        if (!fl) {
          fl = new FreeList;
          SingletonHolder<ThreadSpecific<FreeList>, CreateWithNew, DestroyAtExit, ThreadSafe>::Instance().Set(fl);
        }

        FreeBlock* fb = static_cast<FreeBlock*>(ptr);
        
        // Add the now free block to the free list
        fb->next = fl->head;
        fl->head = fb;
        fl->count++;
      }
      
      static inline void Deallocate(void* ptr, std::size_t size)
      {
        (void)size;
        Deallocate(ptr);
      }
    
    private:
      struct FreeBlock
      {
        FreeBlock* next;
      };
      
      struct FreeList
      {
        int count;
        FreeBlock* head;
        
        inline FreeList() : count(0), head(NULL) { ; }
        
        ~FreeList()
        {
          FreeBlock* cur = head;
          while (cur) {
            void* block = static_cast<void*>(cur);
            cur = cur->next;
            SuperAlloc::Deallocate(block);
          }
          head = NULL;
          count = 0;
        }
      };
    };
  };
};


#endif
