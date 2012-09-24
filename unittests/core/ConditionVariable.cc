/*
 *  unittests/core/ConditionVariable.cc
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

#include "gtest/gtest.h"

TEST(CoreConditionVariable, Signal) {
  class TestCV : public Apto::Thread
  {
  public:
    Apto::Mutex mutex;
    Apto::ConditionVariable cond;
    bool done;
    bool completed;
    
    TestCV() : done(false), completed(false) { ; }
    
  protected:
    void Run() {
      mutex.Lock();
      while (!done) {
        cond.Wait(mutex);
      }
      completed = true;
      mutex.Unlock();
    }
  };
  
  TestCV tester;
  tester.Start();
  tester.mutex.Lock();
  tester.done = true;
  tester.mutex.Unlock();
  tester.cond.Signal();
  tester.Join();
  EXPECT_TRUE(tester.completed);
}

TEST(CoreConditionVariable, Broadcast) {
  struct GroupCV {
    Apto::Mutex mutex;
    Apto::ConditionVariable cond;
    bool done;
    bool completed[2];
    
    GroupCV() : done(false) { completed[0] = false; completed[1] = false; }
  } group_cv;
  
  class TestCV : public Apto::Thread
  {
  private:
    int m_id;
    GroupCV& m_group_cv;

  public:
    TestCV(int id, GroupCV& group_cv) : m_id(id), m_group_cv(group_cv) { ; }
    
  protected:
    void Run() {
      m_group_cv.mutex.Lock();
      while (!m_group_cv.done) {
        m_group_cv.cond.Wait(m_group_cv.mutex);
      }
      m_group_cv.completed[m_id] = true;
      m_group_cv.mutex.Unlock();
    }
  };
  
  TestCV* testers[2];
  testers[0] = new TestCV(0, group_cv);
  testers[1] = new TestCV(1, group_cv);
  
  testers[0]->Start();
  testers[1]->Start();
  
  group_cv.mutex.Lock();
  group_cv.done = true;
  group_cv.mutex.Unlock();
  group_cv.cond.Broadcast();
  
  testers[0]->Join();
  testers[1]->Join();
  
  EXPECT_TRUE(group_cv.completed[0]);
  EXPECT_TRUE(group_cv.completed[1]);
  
  delete testers[0];
  delete testers[1];
}

