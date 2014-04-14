/*
 *  Integrated.cc
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

#include "apto/scheduler/Integrated.h"

#include "apto/core/Algorithms.h"
#include "apto/scheduler/Util.h"


Apto::Scheduler::Integrated::Integrated(int entry_count) : m_priority_chart(entry_count)
{
  m_priority_chart.SetAll(0.0);
}

Apto::Scheduler::Integrated::~Integrated()
{
  for (int i = 0; i < m_node_array.GetSize(); i++) {
    if (m_node_array[i] != NULL) delete m_node_array[i];
  }
}


void Apto::Scheduler::Integrated::AdjustPriority(int entry_id, double priority)
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
      m_node_array[i]->Remove(entry_id);
      if (m_node_array[i]->size == 0) removeNode(i);
    }
    
    if (!old_bit && new_bit) {
      // Add the item from this node...
      if (i >= m_node_array.GetSize() || !m_node_array[i]) insertNode(i);
      m_node_array[i]->Insert(entry_id);
    }
  }
}


int Apto::Scheduler::Integrated::Next()
{
  assert(m_node_array.GetSize() > 0);  // Running scheduler w/ no entries!
  
  const int last_id = m_node_array.GetSize() - 1;
  
  // Make sure there are entries in the scheduler!
  if (m_node_array[last_id] == NULL) return -1;
  
  int next_id = -1;
  while (next_id < 0) {
    next_id = m_node_array[last_id]->Next();
  }
  
  return next_id;
}


int Apto::Scheduler::Integrated::EntryLimit() const
{
  return m_priority_chart.GetSize();
}


void Apto::Scheduler::Integrated::insertNode(int node_id)
{
  // Test if trying to create node that already exists.
  assert(node_id >= m_node_array.GetSize() || m_node_array[node_id] == NULL);
  
  Node* new_node = new Node(m_priority_chart.GetSize(), node_id);
  
  if (node_id >= m_node_array.GetSize()) resizeNodes(node_id);
  
  m_node_array[node_id] = new_node;
  
  // Find the node to mark as the 'prev'.
  for (int prev_id = node_id + 1; prev_id < m_node_array.GetSize(); prev_id++) {
    Node* prev_node = m_node_array[prev_id];
    if (prev_node) {
      new_node->prev = prev_node;
      prev_node->next = new_node;
      prev_node->process_size = (1 << (prev_id - node_id - 1));
      break;
    }
  }
  
  // And find the node to mark as the 'next'.
  for (int next_id = node_id - 1; next_id >= 0; next_id--) {
    Node* next_node = m_node_array[next_id];
    if (next_node) {
      new_node->next = next_node;
      next_node->prev = new_node;
      new_node->process_size = (1 << (node_id - next_id - 1));
      break;
    }
  }
}

void Apto::Scheduler::Integrated::removeNode(int node_id)
{
  assert(m_node_array[node_id] != NULL); // Trying to remove non-existant node.
  
  Node* old_node = m_node_array[node_id];
  Node* next_node = old_node->next;
  Node* prev_node = old_node->prev;
  m_node_array[node_id] = NULL;
  
  if (next_node) next_node->prev = prev_node;
  if (prev_node) {
    prev_node->next = next_node;
    prev_node->process_size = old_node->process_size * prev_node->process_size * 2;
  }
  
  if (node_id == m_node_array.GetSize() - 1) {
    if (!old_node->next) resizeNodes(0);
    else resizeNodes(old_node->next->node_id);
  }
  
  delete old_node;
}

void Apto::Scheduler::Integrated::resizeNodes(int new_max)
{
  int old_size = m_node_array.GetSize();
  int new_size = new_max + 1;  // 0 to new_max...
  
  // Clean up tail portions of the array being cut off.
  for (int i = new_size; i < old_size; i++) {
    if (m_node_array[i]) delete m_node_array[i];
  }
  
  m_node_array.Resize(new_size);
  
  // Mark as NULL any new cells added to the array.
  for (int i = old_size; i < new_size; i++) m_node_array[i] = NULL;
}


void Apto::Scheduler::Integrated::Node::Insert(int item_id)
{
  assert(item_id >= 0 && item_id < active_array.GetSize());  // Illegal ID
  
  // If this item is already active in this node, ignore this call...
  if (active_array[item_id] != 0) return;
  
  // See if we're dealing with a new first_entry...
  if (first_entry == -1 || item_id < first_entry) {
    active_array[item_id] = first_entry;
    first_entry = item_id;
  }
  else {
    // Otherwise find the predecessor to this item in the list...
    int prev_item;
    for (prev_item = item_id - 1; prev_item >= 0; prev_item--) {
      if (active_array[prev_item] != 0) break;
    }
    assert(prev_item >= 0);  // prev_item is first, but not identified.
    
    // Make the predecessor point to it, and have it point to the CPU that
    // the old predecessor pointed to.
    active_array[item_id] = active_array[prev_item];
    active_array[prev_item] = item_id;
  }
  
  size++;
}

void Apto::Scheduler::Integrated::Node::Remove(int item_id)
{
  assert(item_id >= 0 && item_id < active_array.GetSize()); // Illegal ID
  
  // If this item is already inactive, ignore this call...
  if (active_array[item_id] == 0) return;
  
  // If this is the first_entry, adjust it!
  if (first_entry == item_id) {
    first_entry = active_array[item_id];
  }
  else {
    // Find the predecessor to this item in the list...
    int prev_item;
    for (prev_item = item_id - 1; prev_item >= 0; prev_item--) {
      if (active_array[prev_item] != 0) break;
    }
    assert(prev_item >= 0);  // prev_item is first, but not identified.
    
    // Make the predecessor point to the item removed used to point to.
    active_array[prev_item] = active_array[item_id];
  }
  
  active_array[item_id] = 0;
  size--;
}


// Execute everything on list, and then shift to calling the next node.
// Wait for the next node to return a -1 before shifting back to this one.

int Apto::Scheduler::Integrated::Node::Next()
{
  // Alternate between this node's Process and the next's.
  if (execute == false) {
    // If there is a next node, we may be working on it...
    int next_id = -1;
    if (next != NULL) next_id = next->Next();
    
    // If next_id is a -1, either we don't have a next node, or else it
    // is finished with its execution.
    
    if (next_id == -1) {
      execute = true;
      process_count = 0;
      active_entry = -1;
    }
    
    return next_id;
  }
  
  // Find the next active_entry...
  
  // If we were at the end of the list, start over...
  if (active_entry == -1) active_entry = first_entry;
  
  // If this entry no longer exists, hunt for the next active entry manually...
  else if (active_array[active_entry] == 0) {
    while (active_entry < active_array.GetSize() &&
           active_array[active_entry] == 0) {
      active_entry++;
    }
    if (active_entry == active_array.GetSize()) active_entry = -1;
  }
  
  // Otherwise, if the entry does exist, we can just look the next one up.
  else active_entry = active_array[active_entry];
  
  
  // If we have now hit the end of this list, move on to the next node.
  
  if (active_entry == -1) {
    process_count++;
    if (process_count >= process_size) execute = false;
  }
  
  return active_entry;
}
