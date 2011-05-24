/*
 *  unittests/core/RWLock.cc
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

#include "apto/core/List.h"
#include "apto/core/Mutex.h"
#include "apto/core/RWLock.h"
#include "apto/core/Thread.h"
#include "apto/platform.h"

#if APTO_PLATFORM(WINDOWS)
# include <io.h>
# define sleep(x) Sleep(x ## 000)
#else
# include <unistd.h>
#endif

#include "gtest/gtest.h"

TEST(CoreRWLock, Usage) {
  
  struct TestInfo {
    Apto::RWLock rwlock;
    Apto::Mutex mutex;
    Apto::List<int> seq;
  } tinfo;

  class TestReadThread : public Apto::Thread
  {
  private:
    TestInfo& m_tinfo;
    
  public:
    TestReadThread(TestInfo& tinfo) : m_tinfo(tinfo)
    {
      m_tinfo.rwlock.ReadLock();
    }
    
  protected:
    void Run() {
      m_tinfo.mutex.Lock();
      m_tinfo.seq.PushRear(1);
      m_tinfo.mutex.Unlock();
      
      sleep(1);
      m_tinfo.rwlock.ReadUnlock();
    }
  };
  
  class TestWriteThread : public Apto::Thread
  {
  private:
    TestInfo& m_tinfo;
    
  public:
    TestWriteThread(TestInfo& tinfo) : m_tinfo(tinfo) { ; }
    
  protected:
    void Run() {
      m_tinfo.rwlock.WriteLock();
      m_tinfo.mutex.Lock();
      m_tinfo.seq.PushRear(2);
      m_tinfo.mutex.Unlock();
      m_tinfo.rwlock.WriteUnlock();
    }
  };
  
  
  
  TestReadThread tr(tinfo);
  TestWriteThread tw(tinfo);
  
  tinfo.rwlock.ReadLock();
  tinfo.mutex.Lock();
  tinfo.seq.PushRear(0);
  tinfo.mutex.Unlock();
  tinfo.rwlock.ReadLock();
  tinfo.mutex.Lock();
  tinfo.seq.PushRear(0);
  tinfo.mutex.Unlock();
  
  tr.Start();
  tinfo.rwlock.ReadUnlock();
  
  tw.Start();
  sleep(1);
  tinfo.rwlock.ReadUnlock();
  
  tr.Join();
  tw.Join();  
  
  tinfo.mutex.Lock();
  EXPECT_EQ(0, tinfo.seq.Pop());
  EXPECT_EQ(0, tinfo.seq.Pop());
  EXPECT_EQ(1, tinfo.seq.Pop());
  EXPECT_EQ(2, tinfo.seq.Pop());
  
  tinfo.mutex.Unlock();
}
