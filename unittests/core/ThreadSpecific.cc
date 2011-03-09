/*
 *  unittests/core/ThreadSpecific.cc
 *  Apto
 *
 *  Created by David on 2/14/11.
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

#include "apto/core/ConditionVariable.h"
#include "apto/core/Thread.h"
#include "apto/core/ThreadSpecific.h"

#include "gtest/gtest.h"


TEST(CoreThreadSpecific, Usage) {
  class TestCV : public Apto::Thread
  {
  public:
    Apto::Mutex mutex;
    Apto::ConditionVariable cond;
    Apto::ThreadSpecific<int> value;
    bool done;
    
    TestCV() : done(false) { ; }
    
  protected:
    void Run() {
      mutex.Lock();
      
      // Build thread specific value
      int* local_value = value.Get();
      if (!local_value) {
        local_value = new int;
        value.Set(local_value);
      }
      *local_value = 12;
      
      while (!done) {
        cond.Wait(mutex);
      }
      
      // Destroy thread specific value
      local_value = value.Get();
      value.Set(NULL);
      
      mutex.Unlock();
    }
  };
  
  TestCV tester;
  
  int* local_value = tester.value.Get();
  if (!local_value) {
    local_value = new int;
    tester.value.Set(local_value);
  }
  *local_value = 6;

  
  tester.Start();
  tester.mutex.Lock();
  tester.done = true;
  tester.mutex.Unlock();
  tester.cond.Signal();
  tester.Join();
  
  local_value = tester.value.Get();
  EXPECT_EQ(6, *local_value);
}
