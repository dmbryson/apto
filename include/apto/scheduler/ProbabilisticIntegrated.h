/*
 *  ProbabilisticIntegrated.h
 *  Apto
 *
 *  Created by David on 10/26/12.
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

#ifndef AptoSchedulerProbabilisticIntegrated_h
#define AptoSchedulerProbabilisticIntegrated_h

#include "apto/core/Array.h"
#include "apto/core/PriorityScheduler.h"
#include "apto/core/Random.h"
#include "apto/core/SmartPtr.h"


namespace Apto {
  namespace Scheduler {
    
    // ProbabilisticIntegrated
    // --------------------------------------------------------------------------------------------------------------
    
    class ProbabilisticIntegrated : public PriorityScheduler
    {
    private:
      struct Node;
      
    private:
      SmartPtr<Random> m_rng;
      Array<Node*> m_node_array;
      Array<double> m_node_weight;
      Array<double> m_priority_chart;
      double m_total_weight;
      
      
      ProbabilisticIntegrated();
      ProbabilisticIntegrated(const ProbabilisticIntegrated&);
      ProbabilisticIntegrated& operator=(const ProbabilisticIntegrated&);
      
    public:
      LIB_EXPORT ProbabilisticIntegrated(int entry_count, SmartPtr<Random> rng);
      LIB_EXPORT ~ProbabilisticIntegrated();
      
      LIB_EXPORT void AdjustPriority(int entry_id, double priority);
      LIB_EXPORT int Next();

      LIB_EXPORT int EntryLimit() const;

    private:
      void insertNode(int node_id);
      void removeNode(int node_id);
      void resizeNodes(int new_size);
      
    private:
      struct Node
      {
        Array<int> active_index;  // Each entry in this array corresponds to the item with the same ID. if the entry is not
        // in the list, its value in the array will be 0. If it is in the list, it will point to
        // the cell of the next included entry. The last included entry has a -1 in its cell.
        Array<int, Smart> active_entries;
        int node_id;
        
        Node* next;
        Node* prev;
        
        
        inline Node(int entry_count, int in_node_id)
          : active_index(entry_count), active_entries(0), node_id(in_node_id), next(NULL), prev(NULL)
        {
          active_index.SetAll(-1);
          active_entries.SetReserve(entry_count);
        }
        inline ~Node() { ; }
        
        inline int Size() { return active_entries.GetSize(); }
        
        void Insert(int entry_id);
        void Remove(int entry_id);
      };
    };
    
  };
};

#endif
