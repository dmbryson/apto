/*
 *  ContingencyTable.cc
 *  Apto
 *
 *  Created by David on 2/15/11.
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

#include "apto/stat/ContingencyTable.h"


Apto::Stat::ContingencyTable::ContingencyTable(int nrow, int ncol)
  : m_nrow(nrow), m_ncol(ncol), m_table(nrow * ncol), m_row_marginals(nrow), m_col_marginals(ncol), m_total(0)
{
  assert(nrow > 0);
  assert(ncol > 0);
  m_table.SetAll(0);
  m_row_marginals.SetAll(0);
  m_col_marginals.SetAll(0);
}

Apto::Stat::ContingencyTable& Apto::Stat::ContingencyTable::operator=(const Apto::Stat::ContingencyTable& rhs)
{
  m_nrow = rhs.m_nrow;
  m_ncol = rhs.m_ncol;
  m_table = rhs.m_table;
  m_row_marginals = rhs.m_row_marginals;
  m_col_marginals = rhs.m_col_marginals;
  m_total = rhs.m_total;
  return *this;
}


void Apto::Stat::ContingencyTable::SetElement(int row, int col, int value)
{
  assert(value >= 0);
  int diff = value - m_table[(row * m_ncol) + col];
  m_row_marginals[row] += diff;
  m_col_marginals[col] += diff;
  m_total += diff;
  m_table[(row * m_ncol) + col] = value;
}

