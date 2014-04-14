/*
 *  Probabilistic.cc
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

#include "apto/scheduler/Probabilistic.h"


Apto::Scheduler::Probabilistic::~Probabilistic() { ; }

void Apto::Scheduler::Probabilistic::AdjustPriority(int entry_id, double priority)
{
  m_index.SetWeight(entry_id, priority);
}

int Apto::Scheduler::Probabilistic::Next()
{
  if (m_index.TotalWeight() == 0) return -1;

  return m_index.FindPosition(m_rng->GetDouble(m_index.TotalWeight()));
}

int Apto::Scheduler::Probabilistic::EntryLimit() const
{
  return m_index.Size();
}




Apto::Scheduler::Probabilistic::WeightedIndex::WeightedIndex(int size)
  : m_size(size), m_item_weight(size), m_subtree_weight(size)
{
  m_item_weight.SetAll(0.0);
  m_subtree_weight.SetAll(0.0);
}


void Apto::Scheduler::Probabilistic::WeightedIndex::SetWeight(int entry_id, double weight)
{
  m_item_weight[entry_id] = weight;
  
  while (true) {
    const int left_id = leftChildOf(entry_id);
    const int right_id = rightChildOf(entry_id);
    const double left_subtree = (left_id >= m_size) ? 0.0 : m_subtree_weight[left_id];
    const double right_subtree = (right_id >= m_size) ? 0.0 : m_subtree_weight[right_id];
    m_subtree_weight[entry_id] = m_item_weight[entry_id] + left_subtree + right_subtree;
    if (entry_id == 0) break;
    entry_id = parentOf(entry_id);
  }
}

int Apto::Scheduler::Probabilistic::WeightedIndex::findPosition(double position, int root_id)
{
  assert(position < m_subtree_weight[root_id]);
  
  // First, see if we should just return this node.
  if (position < m_item_weight[root_id]) {
    return root_id;
  }
  
  // If not, then see if we should search in the left subtree...
  position -= m_item_weight[root_id];
  const int left_id = leftChildOf(root_id);
  assert (left_id < m_size);
  if (position < m_subtree_weight[left_id]) {
    return findPosition(position, left_id);
  }
  
  // Otherwise we must look in the right subtree...
  position -= m_subtree_weight[left_id];
  const int right_id = rightChildOf(root_id);
  assert (right_id < m_size);
  assert (position < m_subtree_weight[right_id]);
  return findPosition(position, right_id);
}
