/*
 *  FishersExact.cc
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
#include "apto/stat/Functions.h"

#include "apto/core/Array.h"
#include "apto/core/ArrayUtils.h"
#include "apto/core/Pair.h"

#include <cmath>
#include <limits>


using namespace Apto;


// Internal Function Declarations
// -------------------------------------------------------------------------------------------------------------- 

static double cummulativeGamma(double q, double alpha, bool& fault);
static double logGamma(double x, bool& fault);



// Internal Class/Struct Definitions
// -------------------------------------------------------------------------------------------------------------- 

static struct PastPathLength
{
  double value;
  int observed;
  
  PastPathLength(double in_value) : value(in_value), observed(1) { ; }
};


static struct FExactNode
{
  double shortest_path;
  double longest_path;
  Array<PastPathLength, Smart> past_entries;  
};


static class FExact
{
private:
  Array<double> m_facts; // Log factorials
  Array<int> m_row_marginals;
  Array<int> m_col_marginals;
  
public:
  FExact(const Stat::ContingencyTable& table);
  
  double Calculate();
  
private:
  double logMultinomial(int numerator, const Array<int>& denominator);
  void removeFromVector(const Array<int>& src, int idx_remove, Array<int>& dest);
  void reduceZeroInVector(const Array<int>& src, int value, int idx_start, Array<int>& dest);
  
  double longestPath(const Array<int>& row_marginals, const Array<int>& col_marginals, int marginal_total);
  void shortestPath(const Array<int>& row_marginals, const Array<int>& col_marginals, double& shortest_path);
  bool shortestPathSpecial(const Array<int>& row_marginals, const Array<int>& col_marginals, double& val);
  
  bool generateNewNode(const Array<int>& row_marginals, Array<int>& diff, int& idx_dec, int& idx_inc);
};





// Exported Methods
// -------------------------------------------------------------------------------------------------------------- 

double Stat::FishersExact(const ContingencyTable& table)
{
  if (table.MarginalTotal() == 0.0) return std::numeric_limits<double>::quiet_NaN();  // All elements are 0
  
  FExact fe(table);
  return fe.Calculate();
}



// FExact Methods
// -------------------------------------------------------------------------------------------------------------- 

FExact::FExact(const Stat::ContingencyTable& table)
  : m_facts(table.MarginalTotal())
{
  if (table.NumRows() > table.NumCols()) {
    m_row_marginals = table.ColMarginals();
    m_col_marginals = table.RowMarginals();
  } else {
    m_row_marginals = table.RowMarginals();
    m_col_marginals = table.ColMarginals();
  }
  
  QSort(m_row_marginals);
  QSort(m_col_marginals);
  
  const int marginal_total = table.MarginalTotal();
  m_facts[0] = 0.0;
  m_facts[1] = 0.0;
  m_facts[2] = log(2.0);
  for (int i = 0; i < marginal_total; i++) {
    m_facts[i] = m_facts[i - 1] + log(i);
    if (++i < marginal_total)  m_facts[i] = m_facts[i - 1] + m_facts[2] + m_facts[i / 2] - m_facts[i / 2 - 1];
  }
}

double FExact::Calculate() {
  
  return 0.0;
}


double FExact::logMultinomial(int numerator, const Array<int>& denominator)
{
  double ret_val = m_facts[numerator];
  for (int i = 0; i < denominator.GetSize(); i++) ret_val -= m_facts[denominator[i]];
  return ret_val;
}


void FExact::removeFromVector(const Array<int>& src, int idx_remove, Array<int>& dest)
{
  dest.Resize(src.GetSize() - 1);
  for (int i = 0; i < idx_remove; i++) dest[i] = src[i];
  for (int i = idx_remove + 1; i < src.GetSize(); i++) dest[i - 1] = src[i];
}


void FExact::reduceZeroInVector(const Array<int>& src, int value, int idx_start, Array<int>& dest)
{
  dest.Resize(src.GetSize());
  
  int i = 0;
  for (; i < idx_start; i++) dest[i] = src[i];
  
  for (; i < (src.GetSize() - 1); i++) {
    if (value >= src[i + 1]) {
      
    }
    dest[i] = src[i + 1];
  }
  dest[i] = value;
  
  for (++i; i < src.GetSize(); i++) dest[i] = src[i];
}


double FExact::longestPath(const Array<int>& row_marginals, const Array<int>& col_marginals, int marginal_total)
{
  class ValueHashTable
  {
  private:
    Array<Pair<int, double> > m_table;
    Array<int> m_stack;
    int m_entry_count;
    
  public:
    inline ValueHashTable(int size = 200) : m_table(size), m_stack(size) { ClearTable(); }
    
    int GetEntryCount() const { return m_entry_count; }
    
    bool Find(int key, int& idx)
    {
      int init = key % m_table.GetSize();
      idx = init;
      for (; idx < m_table.GetSize(); idx++) {
        if (m_table[idx].Value1() < 0) {
          m_stack[m_entry_count] = idx;
          m_table[idx].Value1() = key;
          m_entry_count++;
          return false;
        } else if (m_table[idx].Value1() == key) {
          return true;
        }
      }
      for (idx = 0; idx < init; idx++) {
        if (m_table[idx].Value1() < 0) {
          m_stack[m_entry_count] = idx;
          m_table[idx].Value1() = key;
          m_entry_count++;
          return false;
        } else if (m_table[idx].Value1() == key) {
          return true;
        }
      }
      assert(false);
      return false;
    }
    
    inline double& operator[](int idx) { return m_table[idx].Value2(); }
    
    Pair<int, double> Pop()
    {
      Pair<int, double> tmp = m_table[m_stack[--m_entry_count]];
      m_table[m_stack[m_entry_count]].Value1() = -1;
      return tmp;
    }
    
    inline void ClearTable()
    {
      m_entry_count = 0;
      for (int i = 0; i < m_table.GetSize(); i++) m_table[i].Value1() = -1;
    }
  };
  
  
  // 1 x c
  if (row_marginals.GetSize() <= 1) {
    int longest_path = 0.0;
    if (row_marginals.GetSize() > 0) {
      for (int i = 0; i < col_marginals.GetSize(); i++) longest_path -= m_facts[col_marginals[0]];
    }
    return longest_path;
  }
  
  // r x 1
  if (col_marginals.GetSize() <= 1) {
    int longest_path = 0.0;
    if (col_marginals.GetSize() > 0) {
      for (int i = 0; i < row_marginals.GetSize(); i++) longest_path -= m_facts[row_marginals[i]];
    }
    return longest_path;
  }
  
  // 2 x 2
  if (row_marginals.GetSize() == 2 && col_marginals.GetSize() == 2) {
    int n11 = (row_marginals[0] + 1) * (col_marginals[0] + 1) / (marginal_total + 2);
    int n12 = row_marginals[0] - n11;
    return -m_facts[n11] - m_facts[n12] - m_facts[col_marginals[0] - n11] - m_facts[col_marginals[1] - n12];
  }
  
  double val = 0.0;
  bool min = false;
  if (row_marginals[row_marginals.GetSize() - 1] <= row_marginals[0] + col_marginals.GetSize()) {
    min = shortestPathSpecial(row_marginals, col_marginals, val);
  }
  if (!min && col_marginals[col_marginals.GetSize() - 1] <= col_marginals[0] + row_marginals.GetSize()) {
    min = shortestPathSpecial(row_marginals, col_marginals, val);
  }
  
  if (min) {
    return -val;
  }
  

  int ntot = marginal_total;
  Array<int> lrow;
  Array<int> lcol;
  
  if (row_marginals.GetSize() >= col_marginals.GetSize()) {
    lrow = row_marginals;
    lcol = col_marginals;
  } else {
    lrow = col_marginals;
    lcol = row_marginals;
  }
  
  Array<int> nt(lcol.GetSize());
  nt[0] = ntot - lcol[0];
  for (int i = 1; i < lcol.GetSize(); i++) nt[i] = nt[i - 1] - lcol[i];
  

  Array<double> alen(lrow.GetSize());
  alen.SetAll(0.0);
  
  ValueHashTable vht[2];
  int active_vht = 0;
  
  double vmn = 1.0e100;
  int nc1s = lcol.GetSize() - 2;
  int irl = 0;
  int kyy = lcol[nc1s] + 1;
  
  Array<int> lb(lrow.GetSize());
  Array<int> nu(lrow.GetSize());
  Array<int> nr(lrow.GetSize());
  
  
  while (true) {
    bool continue_main = false;
    
    // Setup to generate new node
    int lev = 0;
    int nr1 = lrow.GetSize() - 1;
    int nrt = lrow[irl];
    int nct = lcol[0];
    lb[0] = (int)((((double)nrt + 1.0) * (nct + 1)) / (double)(ntot + nr1 * nc1s + 1)) - 1;
    nu[0] = (int)((((double)nrt + nc1s) * (nct + nr1)) / (double)(ntot + nr1 + nc1s)) - lb[0] + 1;
    nr[0] = nrt - lb[0];
    
    while (true) {
      do {
        nu[lev]--;
        if (nu[lev] == 0) {
          if (lev == 1) {
            do {
              if (vht[(active_vht) ? 0 : 1].GetEntryCount()) {
                Pair<int, double> entry = vht[(active_vht) ? 0 : 1].Pop();
                val = entry.Value2();
                int key = entry.Value1();
                
                // Compute Marginals
                for (int i = lcol.GetSize() - 1; i > 0; i++) {
                  lcol[i] = key % kyy;
                  key = key / kyy;
                }
                lcol[0] = key;
                
                // Set up nt array
                nt[0] = ntot - lcol[0];
                for (int i = 1; i < lcol.GetSize(); i++) nt[i] = nt[i - 1] - lcol[i];
               
                min = false;
                if (lrow[lrow.GetSize() - 1] <= lrow[irl] + lcol.GetSize()) {
                  min = shortestPathSpecial(lrow, lcol, val);
                }
                if (!min && lcol[lcol.GetSize() - 1] <= lcol[0] + lrow.GetSize()) {
                  min = shortestPathSpecial(lrow, lcol, val);
                }
                
                if (min) {
                  if (val < vmn) vmn = val;
                  continue;
                }
                continue_main = true;
              } else if (lrow.GetSize() > 2 && vht[active_vht].GetEntryCount()) {
                // Go to next level
                ntot = ntot - lrow[irl];
                irl++;
                lrow.Resize(lrow.GetSize() - 1);
                continue;
              }
              break;
            } while (true);
            if (!continue_main) return -vmn;
          }
          if (continue_main) break;
          lev--;
          continue;
        }
        break;
      } while (true);
      if (continue_main) break;
      
      lb[lev]++;
      nr[lev]--;
      
      for (alen[lev] = alen[lev - 1] + m_facts[lb[lev]]; lev < nc1s; alen[lev] = alen[lev - 1] + m_facts[lb[lev]]) {
        int nn1 = nt[lev];
        int nrt = nr[lev];
        lev++;
        int nc1 = lcol.GetSize() - lev;
        int nct = lcol[lev];
        lb[lev] = (double)((nrt + 1) * (nct + 1)) / (double)(nn1 + nr1 * nc1 + 1);
        nu[lev] = (double)((nrt + nc1) * (nct + nr1)) / (double)(nn1 + nr1 + nc1) - lb[lev] + 1;
        nr[lev] = nrt - lb[lev];
      }
      alen[lcol.GetSize() - 1] = alen[lev] + m_facts[nr[lev]];
      lb[lcol.GetSize() - 1] = nr[lev];
      
      int v = val + alen[lcol.GetSize() - 1];
      if (lrow.GetSize() == 2) {
        for (int i = 0; i < lcol.GetSize(); i++) v += m_facts[lcol[i] - lb[i]];
        if (v < vmn) vmn = v;
      } else if (lrow.GetSize() == 3 && lcol.GetSize() == 2) {
        int nn1 = ntot - lrow[irl] + 2;
        int ic1 = lcol[0] - lb[0];
        int ic2 = lcol[1] - lb[1];
        int n11 = (lrow[irl + 1] + 1) * (ic1 + 1) / nn1;
        int n12 = lrow[irl + 1] - nn1;
        v += m_facts[n11] + m_facts[n12] + m_facts[ic1 - nn1] + m_facts[ic2 - n12];
        if (v < vmn) vmn = v;
      } else {
        Array<int> it(lcol.GetSize());
        for (int i = 0; i < lcol.GetSize(); i++) it[i] = lcol[i] - lb[i];
        
        if (lcol.GetSize() == 2) {
          if (it[0] > it[1]) it.Swap(0, 1);
        } else {
          QSort(it);
        }
        
        // Compute hash value
        int key = it[0] * kyy + it[1];
        for (int i = 2; i < lcol.GetSize(); i++) key = it[i] + key * kyy;
        
        // Put onto stack (or update stack entry as necessary)
        int t_idx;
        if (vht[active_vht].Find(key, t_idx)) {
          if (v < vht[active_vht][t_idx]) vht[active_vht][t_idx] = v;
        } else {
          vht[active_vht][t_idx] = v;
        }
      }
    }
  }
  
  
  
  return 0.0;
}


void FExact::shortestPath(const Array<int>& row_marginals, const Array<int>& col_marginals, double& shortest_path)
{
  // Take care of easy cases first
  
  // 1 x c
  if (row_marginals.GetSize() == 1) {
    for (int i = 0; i < col_marginals.GetSize(); i++) shortest_path -= m_facts[col_marginals[i]];
    return;
  }
  
  // r x 1
  if (col_marginals.GetSize() == 1) {
    for (int i = 0; i < row_marginals.GetSize(); i++) shortest_path -= m_facts[row_marginals[i]];
    return;
  }
  
  // 2 x 2
  if (row_marginals.GetSize() == 2 && col_marginals.GetSize() == 2) {
    if (row_marginals[1] <= col_marginals[1]) {
      shortest_path -= m_facts[row_marginals[1]] - m_facts[col_marginals[0]] - m_facts[col_marginals[1] - row_marginals[1]];
    } else {
      shortest_path -= m_facts[col_marginals[1]] - m_facts[row_marginals[0]] - m_facts[row_marginals[1] - col_marginals[1]];
    }
    return;
  }
  
  // General Case
  Array<Array<int> > row_stack(row_marginals.GetSize() + col_marginals.GetSize() + 1);
  Array<Array<int> > col_stack(row_marginals.GetSize() + col_marginals.GetSize() + 1);
  
  row_stack[0].Resize(row_marginals.GetSize());
  for (int i = 0; i < row_marginals.GetSize(); i++) row_stack[0][i] = row_marginals[row_marginals.GetSize() - i - 1];
  col_stack[0].Resize(col_marginals.GetSize());
  for (int i = 0; i < col_marginals.GetSize(); i++) col_stack[0][i] = col_marginals[col_marginals.GetSize() - i - 1];

  int istk = 0;

  Array<double> y_stack(row_marginals.GetSize() + col_marginals.GetSize() + 1);
  Array<int> l_stack(row_marginals.GetSize() + col_marginals.GetSize() + 1);
  Array<int> m_stack(row_marginals.GetSize() + col_marginals.GetSize() + 1);
  Array<int> n_stack(row_marginals.GetSize() + col_marginals.GetSize() + 1);
  y_stack[0] = 0.0;
  double y = 0.0;

  int l = 0;
  double amx = 0.0;
  
  int m, n, jrow, jcol;
  
  do {
    int row1 = row_stack[istk][0];
    int col1 = col_stack[istk][0];
    if (row1 > col1) {
      if (row_stack[istk].GetSize() >= col_stack[istk].GetSize()) {
        m = col_stack[istk].GetSize() - 1;
        n = 2;
      } else {
        m = row_stack[istk].GetSize();
        n = 1;
      }
    } else if (row1 < col1) {
      if (row_stack[istk].GetSize() <= col_stack[istk].GetSize()) {
        m = row_stack[istk].GetSize() - 1;
        n = 1;
      } else {
        m = col_stack[istk].GetSize();
        n = 2;
      }
    } else {
      if (row_stack[istk].GetSize() <= col_stack[istk].GetSize()) {
        m = row_stack[istk].GetSize() - 1;
        n = 1;
      } else {
        m = col_stack[istk].GetSize() - 1;
        n = 2;
      }
    }
    
    do {
      if (n == 1) {
        jrow = l;
        jcol = 0;
      } else {
        jrow = 0;
        jcol = l;
      }
      
      int rowt = row_stack[istk][jrow];
      int colt = col_stack[istk][jcol];
      int mn = (rowt > colt) ? colt : rowt;
      y += m_facts[mn];
      if (rowt == colt) {
        removeFromVector(row_stack[istk], jrow, row_stack[istk + 1]);
        removeFromVector(col_stack[istk], jcol, col_stack[istk + 1]); 
      } else if (rowt > colt) {
        removeFromVector(col_stack[istk], jcol, col_stack[istk + 1]);
        reduceZeroInVector(row_stack[istk], rowt - colt, jrow, row_stack[istk + 1]);
      } else {
        removeFromVector(row_stack[istk], jrow, row_stack[istk + 1]);
        reduceZeroInVector(col_stack[istk], colt - rowt, jcol, col_stack[istk + 1]);
      }
      
      if (row_stack[istk + 1].GetSize() == 1) {
        for (int i = 0; i < col_stack[istk + 1].GetSize(); i++) y += m_facts[col_stack[istk + 1][i]];
        break;
      }
      if (col_stack[istk + 1].GetSize() == 1) {
        for (int i = 0; i < row_stack[istk + 1].GetSize(); i++) y += m_facts[row_stack[istk + 1][i]];
        break;
      }
      if (row_stack[istk + 1].GetSize() == 1 || col_stack[istk + 1].GetSize() == 1) {
        if (y > amx) {
          amx = y;
          if (shortest_path - amx <= 0.0) {
            shortest_path = 0.0;
            return;
          }
        }
        
        bool continue_outer = false;
        for (--istk; istk != 0; istk--) {
          for (; l <= m_stack[istk]; l++) {
            n = n_stack[istk];
            y = y_stack[istk];
            if (n == 1) {
              if (row_stack[istk][l] < row_stack[istk][l - 1]) {
                continue_outer = true;
                break;
              }
            } else if (n == 2) {
              if (col_stack[istk][l] < col_stack[istk][l - 1]) {
                continue_outer = true;
                break;
              }
            }
          }
          if (continue_outer) break;
        }
        if (!continue_outer) break;
        
        shortest_path -= amx;
        if (shortest_path - amx <= 0.0) shortest_path = 0.0;
        return;
      }
    } while (true);
    
    l_stack[istk] = l;
    m_stack[istk] = m;
    n_stack[istk] = n;
    istk++;
    y_stack[istk] = y;
    l = 0;
  } while (true);
  
}


bool FExact::shortestPathSpecial(const Array<int>& row_marginals, const Array<int>& col_marginals, double& val)
{
  Array<int> nd(row_marginals.GetSize());
  Array<int> ne(col_marginals.GetSize());
  Array<int> m(col_marginals.GetSize());
  
  nd.SetAll(0);
  int is = col_marginals[0] / row_marginals.GetSize();
  ne[0] = is;
  int ix = col_marginals[0] - row_marginals.GetSize() * is;
  m[0] = ix;
  if (ix != 0) nd[ix] = 1;
  
  for (int i = 1; i < col_marginals.GetSize(); i++) {
    ix = col_marginals[i] / row_marginals.GetSize();
    ne[i] = ix;
    is = is + ix;
    ix = col_marginals[i] - row_marginals.GetSize() * ix;
    m[i] = ix;
    if (ix != 0) nd[ix]++;
  }
  
  for (int i = row_marginals.GetSize() - 3; i >= 0; i--) nd[i] += nd[i + 1];
  
  ix = 0;
  int nrow1 = row_marginals.GetSize();
  for (int i = (row_marginals.GetSize() - 1); i > 0; i--) {
    ix += is + nd[nrow1 - i] - row_marginals[i];
    if (ix < 0) return false;
  }
  
  val = 0.0;
  for (int i = 0; i < col_marginals.GetSize(); i++) {
    ix = ne[i];
    is = m[i];
    val += is * m_facts[ix + 1] + (row_marginals.GetSize() - is) * m_facts[ix];
  }
  
  return true;
}


bool FExact::generateNewNode(const Array<int>& row_marginals, Array<int>& diff, int& idx_dec, int& idx_inc)
{
  if (idx_inc == 0) {
    while (diff[idx_inc] == row_marginals[idx_inc]) idx_inc++;
  }
  
  // Find node to decrement
  if (diff[idx_dec] > 0 && idx_dec > idx_inc) {
    diff[idx_dec]--;
    while (row_marginals[idx_dec] == 0) idx_dec--;
    int m = idx_dec;
    
    // Find node to increment
    while (diff[m] >= row_marginals[m]) m--;
    diff[m]++;
    
    if (m == idx_inc && diff[m] == row_marginals[m]) idx_inc = idx_dec;
  } else {
    int idx = 0;
    do {
      // Check for finish
      idx = idx_dec + 1;
      while (idx < row_marginals.GetSize() && diff[idx] <= 0) idx++;
      if (idx == row_marginals.GetSize()) return false;
      
      int marginal_total = 1;
      for (int i = 0; i <= idx_dec; i++) {
        marginal_total += diff[i];
        diff[i] = 0;
      }
      idx_dec = idx;
      do {
        idx_dec--;
        int m = (marginal_total < row_marginals[idx_dec]) ? marginal_total : row_marginals[idx_dec];
        diff[idx_dec] = m;
        marginal_total -= m;
      } while (marginal_total > 0 && idx_dec != 0);
      
      if (marginal_total > 0) {
        if (idx != (row_marginals.GetSize() - 1)) {
          idx_dec = idx;
          continue;
        }
        return false;
      } else {
        break;
      }
    } while (true);
    diff[idx]--;
    for (idx_inc = 0; diff[idx_inc] >= row_marginals[idx_inc]; idx_inc++) if (idx_inc > idx_dec) break;
  }
  
  return true;
}



// Internal Function Definitions
// -------------------------------------------------------------------------------------------------------------- 

double cummulativeGamma(double q, double alpha, bool& fault)
{
  if (q <= 0.0 || alpha <= 0.0) {
    fault = true;
    return 0.0;
  }
  
  double f = exp(alpha * log(q) - logGamma(alpha + 1.0, fault) - q); // no need to test logGamma fail as an error is impossible
  if (f == 0.0) {
    fault = true;
    return 0.0;
  }
  
  fault = false;
  
  double c = 1.0;
  double ret_val = 1.0;
  double a = alpha;

  do {
    a += 1.0;
    c = c * q / a;
    ret_val += c;
  } while (c / ret_val > (1e-6));
  ret_val *= f;
  
  return ret_val;
}


double logGamma(double x, bool& fault)
{
  const double a1 = .918938533204673;
  const double a2 = 5.95238095238e-4;
  const double a3 = 7.93650793651e-4;
  const double a4 = .002777777777778;
  const double a5 = .083333333333333;
  
  if (x < 0.0) {
    fault = true;
    return 0.0;
  }
  
  fault = false;
  
  double f = 0.0;
  
  if (x < 7.0) {
    f = x;
    
    x += 1.0;
    while (x < 7.0) {
      f *= x;
      x += 1.0;
    }

    f = -log(f);
  }
  
  double z = 1 / (x * x);
  return f + (x - .5) * log(x) - x + a1 + (((-a2 * z + a3) * z - a4) * z + a5) / x;
}
