/*
 *  Probabilistic.h
 *  Apto
 *
 *  Created by David on 10/25/12.
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
 *  Authors: David M. Bryson <david@programerror.com>, Charles Ofria <ofria@msu.edu>
 *
 */

#ifndef AptoSchedulerProbabilistic_h
#define AptoSchedulerProbabilistic_h

#include "apto/core/Array.h"
#include "apto/core/PriorityScheduler.h"
#include "apto/core/Random.h"
#include "apto/core/SmartPtr.h"


namespace Apto {
  namespace Scheduler {
    
    // Probabilistic -
    // --------------------------------------------------------------------------------------------------------------
  
    class Probabilistic : public PriorityScheduler
    {
    private:
      SmartPtr<Random> m_rng;
      
    public:
      LIB_EXPORT inline Probabilistic(int num_entries, SmartPtr<Random> rng) : m_rng(rng), m_index(num_entries) { ; }
      LIB_EXPORT ~Probabilistic();
      
      LIB_EXPORT void AdjustPriority(int entry_id, double priority);
      LIB_EXPORT int Next();
      
      LIB_EXPORT int EntryLimit() const;
      
    private:
      class WeightedIndex
      {
      private:
        int m_size;
        Apto::Array<double> m_item_weight;
        Apto::Array<double> m_subtree_weight;
        
      public:
        LIB_EXPORT WeightedIndex(int size);
        LIB_EXPORT inline ~WeightedIndex() { ; }
        
        LIB_EXPORT inline int Size() const { return m_size; }
        
        LIB_EXPORT void SetWeight(int entry_id, double weight);
        
        LIB_EXPORT inline double TotalWeight() const { return m_subtree_weight[0]; }
        LIB_EXPORT inline int FindPosition(double position) { return findPosition(position, 0); }
        
      private:
        LIB_EXPORT int findPosition(double position, int root_id);
        
        LIB_EXPORT inline int parentOf(int entry_id) { return (entry_id - 1) / 2; }
        LIB_EXPORT inline int leftChildOf(int entry_id) { return 2 * entry_id + 1; }
        LIB_EXPORT inline int rightChildOf(int entry_id) { return 2 * entry_id + 2; }
      };

      WeightedIndex m_index;
    };
  
  };
};

#endif
