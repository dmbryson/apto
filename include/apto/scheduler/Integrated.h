/*
 *  Integrated.h
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

#ifndef AptoSchedulerIntegrated_h
#define AptoSchedulerIntegrated_h

#include "apto/core/Array.h"
#include "apto/core/PriorityScheduler.h"


namespace Apto {
  namespace Scheduler {
  
    // Integrated
    // --------------------------------------------------------------------------------------------------------------
    //  The integrated scheduler method relies on breaking up all merits into sums of powers of 2 (i.e. using the
    //  binary representation of the priority). All items with merits in the highest power of two will get the most
    //  time, and subsequent priority components will have time divided, continuing recursively. The simplest way of
    //  doing this while maximizing evenness of distribution of time slices is to simply alternate executing the best,
    //  and everything else (where in everything else we again alternate with the best of this sub-list recursively).

    class Integrated : public PriorityScheduler
    {
    private:
      struct Node;
      
    private:
      Array<Node*> m_node_array;
      Array<double> m_priority_chart;
      
      Integrated();
      Integrated(const Integrated&);
      Integrated& operator=(const Integrated&);
      
    public:
      LIB_EXPORT Integrated(int entry_count);
      LIB_EXPORT ~Integrated();
      
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
        Array<int> active_array;  // Each entry in this array corresponds to the item with the same ID. if the entry is not
                                  // in the list, its value in the array will be 0. If it is in the list, it will point to
                                  // the cell of the next included entry. The last included entry has a -1 in its cell.
        int first_entry;          // ID of the first active entry
        int active_entry;         // ID of the next scheduled entry
        int node_id;              // A unique ID (representative the relative priority bit
        
        int size;                 // Number of active items in this node
        int process_size;         // Number of times this node should be executed before the next node is.
        int process_count;        // Number of times this node has been executed
        bool execute;             // Should this node execute or pass?
        
        Node* next;
        Node* prev;
        
        
        inline Node(int entry_count = 0, int in_node_id = -1)
          : active_array(entry_count), first_entry(-1), active_entry(-1), node_id(in_node_id), size(0), process_size(1)
          , process_count(0), execute(true), next(NULL), prev(NULL)
        {
          active_array.SetAll(0);
        }
        inline ~Node() { ; }
      
        void Insert(int entry_id);
        void Remove(int entry_id);
        int Next();
      };
    };
    
  };
};

#endif
