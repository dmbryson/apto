/*
 *  ProbabilisticIntegrated.cc
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

#include "apto/scheduler/ProbabilisticIntegrated.h"

#include "apto/core/Algorithms.h"
#include "apto/scheduler/Util.h"

#include <cmath>


Apto::Scheduler::ProbabilisticIntegrated::ProbabilisticIntegrated(int entry_count, SmartPtr<Random> rng)
  : m_rng(rng), m_priority_chart(entry_count), m_total_weight(0.0)
{
  m_priority_chart.SetAll(0.0);
}

Apto::Scheduler::ProbabilisticIntegrated::~ProbabilisticIntegrated()
{
  for (int i = 0; i < m_node_array.GetSize(); i++) {
    if (m_node_array[i] != NULL) delete m_node_array[i];
  }
}


void Apto::Scheduler::ProbabilisticIntegrated::AdjustPriority(int entry_id, double priority)
{
  double old_priority = m_priority_chart[entry_id];
  if (old_priority == priority) return;
  
  m_priority_chart[entry_id] = priority;
  
  // Re-adjust the lists.
  Util::Priority old_p_comp(old_priority);
  Util::Priority new_p_comp(priority);
  
  int priority_magnitude = Max(old_p_comp.NumBits(), new_p_comp.NumBits());
  for (int i = 0; i < priority_magnitude; i++) {
    bool old_bit = old_p_comp.Bit(i);
    bool new_bit = new_p_comp.Bit(i);
    
    if (old_bit && !new_bit) {
      // Remove the item from this node...
      m_total_weight -= m_node_weight[i];
      m_node_array[i]->Remove(entry_id);
      if (m_node_array[i]->Size() == 0) {
        m_node_weight[i] = 0.0;
        removeNode(i);
      } else {
        m_node_weight[i] = pow(2.0, i) * m_node_array[i]->Size();
        m_total_weight += m_node_weight[i];
      }
    } else if (!old_bit && new_bit) {
      // Add the item from this node...
      if (i >= m_node_array.GetSize() || !m_node_array[i] || !m_node_array[i]->Size()) insertNode(i);
      m_total_weight -= m_node_weight[i];
      m_node_array[i]->Insert(entry_id);
      m_node_weight[i] = pow(2.0, i) * m_node_array[i]->Size();
      m_total_weight += m_node_weight[i];
    }
  }
}


int Apto::Scheduler::ProbabilisticIntegrated::Next()
{
  assert(m_node_array.GetSize() > 0);  // Running scheduler w/ no entries!  
  
  double position = m_rng->GetDouble(m_total_weight);
  
  int node_id = m_node_array.GetSize() - 1;
  while (position > m_node_weight[node_id]) {
    position -= m_node_weight[node_id];
    node_id--;
  }
  
  Node* cur_node = m_node_array[node_id];
//int relative_position = round((position / m_node_weight[node_id]) * (cur_node->Size() - 1));
  int relative_position = floor(((position / m_node_weight[node_id]) * (cur_node->Size() - 1)) + 0.5);  
  return cur_node->active_entries[relative_position];
}


int Apto::Scheduler::ProbabilisticIntegrated::EntryLimit() const
{
  return m_priority_chart.GetSize();
}



void Apto::Scheduler::ProbabilisticIntegrated::insertNode(int node_id)
{
  // Test if trying to create node that already exists.
  assert(node_id >= m_node_array.GetSize() || (m_node_array[node_id] && m_node_array[node_id]->Size() == 0) || m_node_array[node_id] == NULL);
  
  if (node_id >= m_node_array.GetSize()) resizeNodes(node_id);
  
  Node* new_node = m_node_array[node_id];
  if (!new_node) {
    new_node = new Node(m_priority_chart.GetSize(), node_id);
    m_node_array[node_id] = new_node;
  }
  
  // Find the node to mark as the 'prev'.
  for (int prev_id = node_id + 1; prev_id < m_node_array.GetSize(); prev_id++) {
    Node* prev_node = m_node_array[prev_id];
    if (prev_node && prev_node->Size()) {
      new_node->prev = prev_node;
      prev_node->next = new_node;
      break;
    }
  }
  
  // And find the node to mark as the 'next'.
  for (int next_id = node_id - 1; next_id >= 0; next_id--) {
    Node* next_node = m_node_array[next_id];
    if (next_node && next_node->Size()) {
      new_node->next = next_node;
      next_node->prev = new_node;
      break;
    }
  }
}

void Apto::Scheduler::ProbabilisticIntegrated::removeNode(int node_id)
{
  assert(m_node_array[node_id] != NULL); // Trying to remove non-existant node.
  
  Node* old_node = m_node_array[node_id];
  Node* next_node = old_node->next;
  Node* prev_node = old_node->prev;
  
  if (next_node) next_node->prev = prev_node;
  if (prev_node) {
    prev_node->next = next_node;
  }
  
  if (node_id == m_node_array.GetSize() - 1) {
    if (!old_node->next) resizeNodes(0);
    else resizeNodes(old_node->next->node_id);
  }
}

void Apto::Scheduler::ProbabilisticIntegrated::resizeNodes(int new_max)
{
  int old_size = m_node_array.GetSize();
  int new_size = new_max + 1;  // 0 to new_max...
  
  // Clean up tail portions of the array being cut off.
  for (int i = new_size; i < old_size; i++) {
    if (m_node_array[i]) delete m_node_array[i];
  }
  
  m_node_array.Resize(new_size);
  m_node_weight.Resize(new_size);
  
  // Mark as NULL any new cells added to the array.
  for (int i = old_size; i < new_size; i++) {
    m_node_array[i] = NULL;
    m_node_weight[i] = 0.0;
  }
}


void Apto::Scheduler::ProbabilisticIntegrated::Node::Insert(int item_id)
{
  assert(item_id >= 0 && item_id < active_index.GetSize());  // Illegal ID
  
  // If this item is already active in this node, ignore this call...
  if (active_index[item_id] >= 0) return;
  
  int new_idx = active_entries.GetSize();
  active_entries.Push(item_id);
  active_index[item_id] = new_idx;
}

void Apto::Scheduler::ProbabilisticIntegrated::Node::Remove(int item_id)
{
  assert(item_id >= 0 && item_id < active_index.GetSize()); // Illegal ID
  
  // If this item is already inactive, ignore this call...
  if (active_index[item_id] == -1) return;

  int cur_idx = active_index[item_id];
  active_entries.Swap(cur_idx, active_entries.GetSize() - 1);
  active_index[active_entries[cur_idx]] = cur_idx;
  active_entries.Pop();
  
  active_index[item_id] = -1;
}
