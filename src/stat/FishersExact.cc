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
 *  Fishers Exact Test for r x c Contingency Tables
 *  Based on:
 *    "A Network Algorithm for Performing Fisher's Exact Test in r x c Contingency Tables" by Mehta and Patel
 *      Journal of the American Statistical Association, June 1983, Volume 87, Number 382, Pages 427-434
 *    "Algorithm 643: FEXACT: A FORTRAN Subroutine for Fisher's Exact Test on Unordered r x c Contingency Tables"
 *      by Mehta and Patel, ACM Transactions on Mathematical Software, June 1986, Volume 12, No. 2, Pages 154-161
 *    "A Remark on Algorithm 643: FEXACT: A FORTRAN Subroutine for Fisher's Exact Test on Unordered r x c Contingency Tables"
 *      by Clarkson, Fan, and Joe, ACM Transactions on Mathematical Software, Dec. 1993, Volume 19, No. 4, Pages 484-488
 *
 */

#include "apto/platform.h"

#include "apto/stat/ContingencyTable.h"
#include "apto/stat/Functions.h"

#include "apto/core/Array.h"
#include "apto/core/ArrayUtils.h"
#include "apto/core/ConditionVariable.h"
#include "apto/core/Mutex.h"
#include "apto/core/Pair.h"
#include "apto/core/RefCount.h"
#include "apto/core/SmartPtr.h"
#include "apto/core/Thread.h"

#include <cmath>
#include <limits>

#include <iostream>
#include <stdio.h>

using namespace Apto;


// Constant Declarations
// -------------------------------------------------------------------------------------------------------------- 

static const double TOLERANCE = 3.4525e-7;  // Tolerance, as used in Algorithm 643
static const int THREADING_THRESHOLD = 20;
static const int DEFAULT_TABLE_SIZE = 300;



// Internal Function Declarations
// -------------------------------------------------------------------------------------------------------------- 

static double cummulativeGamma(double q, double alpha, bool& fault);
static double logGamma(double x, bool& fault);



// Internal Class/Struct Definitions
// -------------------------------------------------------------------------------------------------------------- 

template <class T> class ManualBuffer
{
private:
  T* m_data;    // Data Array
  int m_size;   // Array Size
  
protected:  
  typedef T StoredType;
  
  explicit ManualBuffer(int size = 0) : m_data(NULL), m_size(size) { ; }
  ManualBuffer(const ManualBuffer& rhs) : m_data(NULL), m_size(0) { this->operator=(rhs); }
  ~ManualBuffer() { ; }
  
  int GetSize() const { return m_size; }
  void ResizeClear(const int in_size) { m_size = in_size; }
  void Resize(int new_size) { m_size = new_size; }
  
  ManualBuffer& operator=(const ManualBuffer& rhs)
  {
    m_size = rhs.m_size;
    for (int i = 0; i < rhs.m_size; i++) m_data[i] = rhs.m_data[i];
    return *this;
  }
  
  inline T& operator[](const int index) { return m_data[index]; }
  inline const T& operator[](const int index) const { return m_data[index]; }
  
  void Swap(int idx1, int idx2)
  {
    T v = m_data[idx1];
    m_data[idx1] = m_data[idx2];
    m_data[idx2] = v;
  }
public:
  void SetBuffer(T* data) { m_data = data; }
};

typedef Array<int, Smart> MarginalArray;


class FExact
{
public:
  // FExact Public Methods
  // ------------------------------------------------------------------------------------------------------------ 
  
  FExact(const Stat::ContingencyTable& table, double tolerance);
  
  double Calculate();
  double ThreadedCalculate();
  

private:
  // FExact Internal Type Declarations
  // ------------------------------------------------------------------------------------------------------------ 
  
  // Main Node
  class FExactNode;
  struct PastPathLength;
  class NodeHashTable;
  typedef SmartPtr<FExactNode, InternalRCObject> NodePtr;

  
  // Path Extremes
  struct PathExtremes;
  class PathExtremesHashTable;
  struct PendingPathExtremes;
  class PendingPathExtremesTable;
  struct PendingPathNode;
  class PathExtremesCalc;

  
private:
  // FExact Internal Methods
  // ------------------------------------------------------------------------------------------------------------ 
  
  inline double logMultinomial(int numerator, const MarginalArray& denominator);
  inline double logMultinomial(int numerator, const MarginalArray::Slice& denominator);
  
  // Core Algorithm
  inline bool generateFirstDaughter(const MarginalArray& row_marginals, int n, MarginalArray& row_diff, int& kmax, int& kd);
  bool generateNewDaughter(int kmax, const MarginalArray& row_marginals, MarginalArray& row_diff, int& idx_dec, int& idx_inc);
  NodePtr handlePastPaths(NodePtr& cur_node, double obs2, double obs3, double ddf, double drn, int kval, NodeHashTable& nht);
  void recordPath(double path_length, int path_freq, Array<PastPathLength, Smart>& past_entries);

  void handleNode(int k, NodePtr cur_node);
  inline void unpackMarginals(int key, MarginalArray& row_marginals);
  
  
  // Longest Path
  double longestPath(const MarginalArray::Slice& row_marginals, const MarginalArray::Slice& col_marginals, int marginal_total);
  bool longestPathSpecial(const MarginalArray::Slice& row_marginals, const MarginalArray::Slice& col_marginals, double& val);
  
  
  // Shortest Path
  void shortestPath(const MarginalArray::Slice& row_marginals, const MarginalArray::Slice& col_marginals, double& shortest_path);
  inline void removeFromVector(const Array<int, ManualBuffer>& src, int idx_remove, Array<int, ManualBuffer>& dest);
  inline void reduceZeroInVector(const Array<int, ManualBuffer>& src, int value, int idx_start, Array<int, ManualBuffer>& dest);


private:
  // FExact Internal Type Definitions
  // ------------------------------------------------------------------------------------------------------------ 

  struct PastPathLength
  {
    double value;
    int observed;
    int next_left;
    int next_right;
    
    PastPathLength(double in_value = 0.0) : value(in_value), observed(1), next_left(-1), next_right(-1) { ; }
    PastPathLength(double in_value, int in_freq) : value(in_value), observed(in_freq), next_left(-1), next_right(-1) { ; }
  };
  
  class FExactNode : public RefCountObject<SingleThreaded>
  {
  public:
    int key;
    Array<PastPathLength, Smart> past_entries;
    
    FExactNode(int in_key) : key(in_key) { ; }
    virtual ~FExactNode() { ; }
  };
  
  class NodeHashTable
  {
  public:
    typedef SmartPtr<FExactNode, InternalRCObject> NodePtr;
  private:
    Array<NodePtr> m_table;
    int m_last;
    
  public:
    inline NodeHashTable(int size = DEFAULT_TABLE_SIZE) : m_table(size), m_last(-1) { ; }
    
    bool Find(int key, int& idx)
    {
      int init = key % m_table.GetSize();
      idx = init;
      for (; idx < m_table.GetSize(); idx++) {
        if (!m_table[idx]) {
          m_table[idx] = NodePtr(new FExactNode(key));
          return false;
        } else if (m_table[idx]->key == key) {
          return true;
        }
      }
      for (idx = 0; idx < init; idx++) {
        if (!m_table[idx]) {
          m_table[idx] = NodePtr(new FExactNode(key));
          return false;
        } else if (m_table[idx]->key == key) {
          return true;
        }
      }
      Rehash(key, idx);
      return false;
    }
    
    inline FExactNode& operator[](int idx) { return *m_table[idx]; }
    inline NodePtr Get(int idx) { return m_table[idx]; }
    
    NodePtr Pop()
    {
      for (++m_last; m_last < m_table.GetSize(); m_last++) {
        if (m_table[m_last]) {
          NodePtr tmp = m_table[m_last];
          m_table[m_last] = NodePtr(NULL);
          return tmp;
        }
      }
      m_last = -1;
      return NodePtr(NULL);
    }
  private:
    void Rehash(int key, int& idx)
    {
      Array<NodePtr> old_table(m_table);
      m_table.ResizeClear(old_table.GetSize() * 2);
      for (int i = 0; i < old_table.GetSize(); i++) {
        int t_idx;
        Find(old_table[i]->key, t_idx);
        m_table[t_idx] = old_table[i];
      }
      Find(key, idx);
    }
  };

  
  
  struct PathExtremes {
    int key;
    double longest_path;
    double shortest_path;
    PathExtremes() : key(-1) { ; }
    
    void Set(int in_key, double lp, double sp) { key = in_key; longest_path = lp; shortest_path = sp; }
  };
  
  class PathExtremesHashTable
  {
  private:
    Array<PathExtremes> m_table;
    
  public:
    inline PathExtremesHashTable(int size = DEFAULT_TABLE_SIZE) : m_table(size) { ClearTable(); }
    
    bool Find(int key, int& idx)
    {
      int init = key % m_table.GetSize();
      idx = init;
      for (; idx < m_table.GetSize(); idx++) {
        if (m_table[idx].key < 0) {
          m_table[idx].key = key;
          return false;
        } else if (m_table[idx].key == key) {
          return true;
        }
      }
      for (idx = 0; idx < init; idx++) {
        if (m_table[idx].key < 0) {
          m_table[idx].key = key;
          return false;
        } else if (m_table[idx].key == key) {
          return true;
        }
      }
      Rehash(key, idx);
      return false;
    }
    
    inline PathExtremes& operator[](int idx) { return m_table[idx]; }
    
    inline void ClearTable()
    {
      for (int i = 0; i < m_table.GetSize(); i++) m_table[i].key = -1;
    }
    
  private:
    void Rehash(int key, int& idx)
    {
      Array<PathExtremes> old_table(m_table);
      m_table.ResizeClear(old_table.GetSize() * 2);
      for (int i = 0; i < old_table.GetSize(); i++) {
        int t_idx;
        Find(old_table[i].key, t_idx);
        m_table[t_idx].longest_path = old_table[i].longest_path;
        m_table[t_idx].shortest_path = old_table[i].shortest_path;
      }
      Find(key, idx);
    }
  };
  
  
  struct PendingPathExtremes {
    int key;
    bool inprogress;
    MarginalArray rows;
    MarginalArray cols;
    double obs2;
    double ddf;
    int ntot;
    PendingPathExtremes() : key(-1), inprogress(false) { ; }
    
    inline void Set(const MarginalArray::Slice& in_rows, const MarginalArray::Slice& in_cols,
                    double in_obs2, double in_ddf, int in_ntot)
    {
      rows.ResizeClear(in_rows.GetSize());
      for (int i = 0; i < in_rows.GetSize(); i++) rows[i] = in_rows[i];
      cols.ResizeClear(in_cols.GetSize());
      for (int i = 0; i < in_cols.GetSize(); i++) cols[i] = in_cols[i];
      ntot = in_ntot;
      obs2 = in_obs2;
      ddf = in_ddf;
    }
  };
  
  class PendingPathExtremesTable
  {
  private:
    Array<PendingPathExtremes> m_table;
    int m_size;
    
  public:
    inline PendingPathExtremesTable(int size = DEFAULT_TABLE_SIZE) : m_table(size), m_size(0) { ; }
    
    int GetSize() { return m_size; }
    
    bool Find(int key, int& idx)
    {
      int init = key % m_table.GetSize();
      idx = init;
      for (; idx < m_table.GetSize(); idx++) {
        if (m_table[idx].key < 0) {
          m_size++;
          m_table[idx].key = key;
          return false;
        } else if (m_table[idx].key == key) {
          return true;
        }
      }
      for (idx = 0; idx < init; idx++) {
        if (m_table[idx].key < 0) {
          m_size++;
          m_table[idx].key = key;
          return false;
        } else if (m_table[idx].key == key) {
          return true;
        }
      }
      Rehash(key, idx);
      return false;
    }
    
    int Pop()
    {
      assert(m_size > 0);
      for (int i = 0; i < m_table.GetSize(); i++) if (m_table[i].key >= 0 && !m_table[i].inprogress) {
        m_table[i].inprogress = true;
        return i;
      }
      return -1;
    }
    void Remove(int idx) { m_table[idx].key = -1; m_table[idx].inprogress = false; m_size--; }
    
    inline PendingPathExtremes& operator[](int idx) { return m_table[idx]; }
    
  private:
    void Rehash(int key, int& idx)
    {
      Array<PendingPathExtremes> old_table(m_table);
      m_table.ResizeClear(old_table.GetSize() * 2);
      for (int i = 0; i < old_table.GetSize(); i++) {
        int t_idx;
        Find(old_table[i].key, t_idx);
        m_table[t_idx].rows = old_table[i].rows;
        m_table[t_idx].cols = old_table[i].cols;
      }
      Find(key, idx);
    }
  };
  
  
  struct PendingPathNode {
    NodePtr node;
    double obs2;
    double drn;
    double ddf;
    int kval;
    bool k1;
    bool handled;
    
    PendingPathNode() : node(NULL), handled(false) { ; }
    inline void Set(NodePtr in_node, double in_obs2, double in_drn, double in_ddf, int in_kval, bool in_k1 = false)
    {
      node = in_node; obs2 = in_obs2; drn = in_drn; ddf = in_ddf; kval = in_kval; k1 = in_k1;
    }
  };
  
    
  class PathExtremesCalc : public Thread
  {
  private:
    FExact* m_fexact;
    bool m_run;
    
  public:
    PathExtremesCalc(FExact* fexact) : m_fexact(fexact), m_run(true) { ; }
    
    void SetFinished() { m_run = false; }
  protected:
    void Run();
  };
  
  
private:
  // FExact Class Variable Declarations
  // ------------------------------------------------------------------------------------------------------------ 
  
  // Constants
  const double m_tolerance;
  
  
  // Pre-calculated Values
  MarginalArray m_row_marginals;
  MarginalArray m_col_marginals;
  Array<double> m_facts; // Log factorials
  Array<int> m_key_multipliers;
  double m_observed_path;
  double m_den_observed_path;

  
  // Main Calculated Value
  double m_pvalue;
  
  
  // Core Algorithm Support
  
  
  // Threaded Core Algorithm Support
  Array<NodeHashTable> m_nht;
  Array<Array<PendingPathNode, Smart> >  m_pending_path_nodes;
  Array<PathExtremesHashTable> m_path_extremes;
  
  // Path Extremes Threading Support
  Mutex m_path_extremes_mutex;
  ConditionVariable m_path_extremes_cond;
  ConditionVariable m_path_extremes_cond_complete;
  Array<PendingPathExtremesTable> m_pending_path_extremes;
  Array<Array<PathExtremes, Smart> > m_completed_path_extremes;
  Array<int, Smart> m_path_extremes_queue;
  bool m_pending_paths_term;
};



// Exported Function Definitions
// -------------------------------------------------------------------------------------------------------------- 

double Apto::Stat::FishersExact(const ContingencyTable& table)
{
  if (table.MarginalTotal() == 0.0) return std::numeric_limits<double>::quiet_NaN();  // All elements are 0
  
  FExact fe(table, TOLERANCE);

  // Use threaded calculate for larger tables
  if (table.NumRows() * table.NumCols() > THREADING_THRESHOLD) return fe.ThreadedCalculate();
  
  return fe.Calculate();
}



// FExact Constructor and Support Methods
// -------------------------------------------------------------------------------------------------------------- 

FExact::FExact(const Stat::ContingencyTable& table, double tolerance)
  : m_tolerance(tolerance)
  , m_facts(table.MarginalTotal() + 1)
  , m_pvalue(0.0)
{
  // Store table marginals for use in calculation
  if (table.NumRows() > table.NumCols()) {
    m_row_marginals = table.ColMarginals();
    m_col_marginals = table.RowMarginals();
  } else {
    m_row_marginals = table.RowMarginals();
    m_col_marginals = table.ColMarginals();
  }
  
  
  // Sort row and column marginals
  QSort(m_row_marginals);
  QSort(m_col_marginals);

  
  // Set up key multipliers
  m_key_multipliers.Resize(m_row_marginals.GetSize());
  m_key_multipliers[0] = 1;
  for (int i = 1; i < m_row_marginals.GetSize(); i++)
    m_key_multipliers[i] = m_key_multipliers[i - 1] * (m_row_marginals[i - 1] + 1);
  

  // Ensure that the maximum key will fit within an integer
  assert((m_row_marginals[m_row_marginals.GetSize() - 2] + 1) <=
         (std::numeric_limits<int>::max() / m_key_multipliers[m_row_marginals.GetSize() - 2]));
  

  // Pre-calculate log factorials
  const int marginal_total = table.MarginalTotal();
  m_facts[0] = 0.0;
  m_facts[1] = 0.0;
  if (marginal_total > 1) {
    m_facts[2] = log(2.0);
    for (int i = 3; i <= marginal_total; i++) {
      m_facts[i] = m_facts[i - 1] + log((double)i);
      if (++i <= marginal_total)  m_facts[i] = m_facts[i - 1] + m_facts[2] + m_facts[i / 2] - m_facts[i / 2 - 1];
    }
  }
  
  
  // Calculate Observed Path Numerator
  m_observed_path = m_tolerance;
  for (int j = 0; j < m_col_marginals.GetSize(); j++) {
    double dd = 0.0;
    for (int i = 0; i < m_row_marginals.GetSize(); i++) {
      if (m_row_marginals.GetSize() == table.NumRows()) {
        dd += m_facts[table[i][j]];
      } else {
        dd += m_facts[table[j][i]];
      }
    }
    m_observed_path += m_facts[m_col_marginals[j]] - dd;
  }
  
  // Calculate Observed Path Denominator
  m_den_observed_path = logMultinomial(marginal_total, m_row_marginals);

  
//  
//  double prt = exp(m_observed_path - m_den_observed_path);
//  std::cout << "prt = " << prt << std::endl;
}


inline double FExact::logMultinomial(int numerator, const MarginalArray& denominator)
{
  double ret_val = m_facts[numerator];
  for (int i = 0; i < denominator.GetSize(); i++) ret_val -= m_facts[denominator[i]];
  return ret_val;
}


inline double FExact::logMultinomial(int numerator, const MarginalArray::Slice& denominator)
{
  double ret_val = m_facts[numerator];
  for (int i = 0; i < denominator.GetSize(); i++) ret_val -= m_facts[denominator[i]];
  return ret_val;
}



// FExact Core Algorithm Calculate Method
// -------------------------------------------------------------------------------------------------------------- 

double FExact::Calculate()
{
  NodeHashTable nht[2];
  PathExtremesHashTable path_extremes;
  
  int k = m_col_marginals.GetSize();
  NodePtr cur_node(new FExactNode(0));
  cur_node->past_entries.Push(PastPathLength(0));
  
  MarginalArray row_diff(m_row_marginals.GetSize());
  MarginalArray irn(m_row_marginals.GetSize());
  
//  printf("k = %d\n", k);

  while (true) {
    int kb = m_col_marginals.GetSize() - k;
    int ks = -1;
    int kmax;
    int kd;
    
    if (generateFirstDaughter(m_row_marginals, m_col_marginals[kb], row_diff, kmax, kd)) {    
      int ntot = 0;
      for (int i = kb + 1; i < m_col_marginals.GetSize(); i++) ntot += m_col_marginals[i];
      
      do {
        for (int i = 0; i < m_row_marginals.GetSize(); i++) irn[i] = m_row_marginals[i] - row_diff[i];
        
        int nrb;
        if (k > 2) {
          if (irn.GetSize() == 2) {
            if (irn[0] > irn[1]) irn.Swap(0, 1);
          } else {
            QSort(irn);
          }
          
          // Adjust for zero start
          int i = 0;
          for (; i < irn.GetSize(); i++) if (irn[i] != 0) break;
          nrb = i;
        } else {
          nrb = 0;
        }
        
        // Build adjusted row array
        MarginalArray::Slice sub_rows = irn.Range(nrb, irn.GetSize() - 1);
        MarginalArray::Slice sub_cols = m_col_marginals.Range(kb + 1, m_col_marginals.GetSize() - 1);
        
        double ddf = logMultinomial(m_col_marginals[kb], row_diff);
        double drn = logMultinomial(ntot, sub_rows) - m_den_observed_path + ddf;
        
        int kval = 0;
        int path_idx = -1;
        double obs2, obs3;
        
        if (k > 2) {
          // compute hash table key for current table
          kval = irn[0] + irn[1] * m_key_multipliers[1];
          for (int i = 2; i < irn.GetSize(); i++) kval += irn[i] * m_key_multipliers[i];

          if (!path_extremes.Find(kval, path_idx)) {
            path_extremes[path_idx].longest_path = 1.0;
          }
          
          obs2 = m_observed_path - m_facts[m_col_marginals[kb + 1]] - m_facts[m_col_marginals[kb + 2]] - ddf;
          for (int i = 3; i <= (k - 1); i++) obs2 -= m_facts[m_col_marginals[kb + i]];
          
          if (path_extremes[path_idx].longest_path > 0.0) {
            
            path_extremes[path_idx].longest_path = longestPath(sub_rows, sub_cols, ntot);
            if (path_extremes[path_idx].longest_path > 0.0) path_extremes[path_idx].longest_path = 0.0;
            
            double dspt = m_observed_path - obs2 - ddf;
            path_extremes[path_idx].shortest_path = dspt;
            shortestPath(sub_rows, sub_cols, path_extremes[path_idx].shortest_path);
            path_extremes[path_idx].shortest_path -= dspt;
            if (path_extremes[path_idx].shortest_path > 0.0) path_extremes[path_idx].shortest_path = 0.0;
          }
          obs3 = obs2 - path_extremes[path_idx].longest_path;
          obs2 = obs2 - path_extremes[path_idx].shortest_path;
        } else {
          obs2 = m_observed_path - drn - m_den_observed_path;
          obs3 = obs2;
        }
        
        handlePastPaths(cur_node, obs2, obs3, ddf, drn, kval, nht[k & 0x1]);
      } while (generateNewDaughter(kmax, m_row_marginals, row_diff, kd, ks));
    }
    
    do {
      cur_node = nht[(k + 1) & 0x1].Pop();
      if (!cur_node) {
        k--;
//        printf("k = %d\n", k);
        path_extremes.ClearTable();
        if (k < 2) return m_pvalue;
      }
    } while (!cur_node);
    
    // Unpack node row marginals from key
    int kval = cur_node->key;
    for (int i = m_row_marginals.GetSize() - 1; i > 0; i--) {
      m_row_marginals[i] = kval / m_key_multipliers[i];
      kval -= m_row_marginals[i] * m_key_multipliers[i];
    }
    m_row_marginals[0] = kval;
  }
  
  return m_pvalue;
}



// FExact Threaded Core Algorithm Calculate Method
// -------------------------------------------------------------------------------------------------------------- 

double FExact::ThreadedCalculate()
{
  int k = m_col_marginals.GetSize();

  // Setup Multi-Stage Data Structures
  m_nht.Resize(k + 1);
  m_pending_path_nodes.Resize(k + 1);
  m_path_extremes.Resize(k + 1);
  m_pending_path_extremes.Resize(k + 1);
  m_completed_path_extremes.Resize(k + 1);
  
  
  // Setup Path Calculation Worker Thread
  Array<PathExtremesCalc*> path_calcs(Platform::AvailableCPUs());
  for (int i = 0; i < path_calcs.GetSize(); i++) {
    path_calcs[i] = new PathExtremesCalc(this);
    path_calcs[i]->Start();
  }
  
  
  // Single Stage Support Variables
  int kval = m_row_marginals[0] + m_row_marginals[1] * m_key_multipliers[1];
  for (int i = 2; i < m_row_marginals.GetSize(); i++) kval += m_row_marginals[i] * m_key_multipliers[i];
  NodePtr cur_node(new FExactNode(kval));
  cur_node->past_entries.Push(PastPathLength(0));


  handleNode(k, cur_node);
  
  Array<PathExtremes, Smart> temp_completed;
  for (; k >= 1; k--) {
//    printf("k = %d\n", k);
    if (m_pending_path_nodes[k].GetSize()) {
      m_path_extremes_mutex.Lock();
      while (m_pending_path_extremes[k].GetSize()) {
        m_path_extremes_cond_complete.Wait(m_path_extremes_mutex);
        if (m_completed_path_extremes[k].GetSize()) {
          temp_completed = m_completed_path_extremes[k];
          m_completed_path_extremes[k].Resize(0);
          m_path_extremes_mutex.Unlock();
          for (int i = 0; i < temp_completed.GetSize(); i++) {
            PathExtremes& path = temp_completed[i];
            int path_idx;
            m_path_extremes[k].Find(path.key, path_idx);
            m_path_extremes[k][path_idx].longest_path = path.longest_path;
            m_path_extremes[k][path_idx].shortest_path = path.shortest_path;
            
            for (int i = 0; i < m_pending_path_nodes[k].GetSize(); i++) {
              PendingPathNode& p = m_pending_path_nodes[k][i];
              if (p.kval != path.key) continue;
              if (p.handled) break;
              double obs2, obs3;
              if (p.k1) {
                obs2 = p.obs2;
                obs3 = p.obs2;
              } else {
                int path_idx;
                m_path_extremes[k].Find(p.kval, path_idx);
                obs3 = p.obs2 - m_path_extremes[k][path_idx].longest_path;
                obs2 = p.obs2 - m_path_extremes[k][path_idx].shortest_path;
              }
              cur_node = handlePastPaths(p.node, obs2, obs3, p.ddf, p.drn, p.kval, m_nht[k  - 1]);
              p.node = NodePtr(NULL);
              if (cur_node) handleNode(k - 1, cur_node);
              p.handled = true;
            }
          }
          m_path_extremes_mutex.Lock();
        }
      }
      for (int i = 0; i < m_completed_path_extremes[k].GetSize(); i++) {
        PathExtremes& path = m_completed_path_extremes[k][i];
        int path_idx;
        m_path_extremes[k].Find(path.key, path_idx);
        m_path_extremes[k][path_idx].longest_path = path.longest_path;
        m_path_extremes[k][path_idx].shortest_path = path.shortest_path;
      }
      m_completed_path_extremes[k].Resize(0);
      m_path_extremes_mutex.Unlock();

      for (int i = 0; i < m_pending_path_nodes[k].GetSize(); i++) {
        PendingPathNode& p = m_pending_path_nodes[k][i];
        if (p.handled) continue;
        double obs2, obs3;
        if (p.k1) {
          obs2 = p.obs2;
          obs3 = p.obs2;
        } else {
          int path_idx;
          m_path_extremes[k].Find(p.kval, path_idx);
          obs3 = p.obs2 - m_path_extremes[k][path_idx].longest_path;
          obs2 = p.obs2 - m_path_extremes[k][path_idx].shortest_path;
        }
        cur_node = handlePastPaths(p.node, obs2, obs3, p.ddf, p.drn, p.kval, m_nht[k  - 1]);
        p.node = NodePtr(NULL);
        if (cur_node) handleNode(k - 1, cur_node);
      }
      m_pending_path_nodes[k].Resize(0);
    }
  }
  
  for (int i = 0; i < path_calcs.GetSize(); i++) path_calcs[i]->SetFinished();
  m_path_extremes_cond.Broadcast();
  for (int i = 0; i < path_calcs.GetSize(); i++) path_calcs[i]->Join();
  
  return m_pvalue;
}


void FExact::handleNode(int k, NodePtr cur_node)
{
  MarginalArray row_marginals(m_row_marginals.GetSize());
  MarginalArray row_diff(row_marginals.GetSize());
  MarginalArray irn(row_marginals.GetSize());
  
  unpackMarginals(cur_node->key, row_marginals);

  int kb = m_col_marginals.GetSize() - k;
  int ks = -1;
  int kmax;
  int kd;
  
  if (!generateFirstDaughter(row_marginals, m_col_marginals[kb], row_diff, kmax, kd)) return;
  
  int ntot = 0;
  for (int i = kb + 1; i < m_col_marginals.GetSize(); i++) ntot += m_col_marginals[i];
  
  do {
    for (int i = 0; i < row_marginals.GetSize(); i++) irn[i] = row_marginals[i] - row_diff[i];
    
    int nrb = 0;
    if (k > 2) {
      QSort(irn);
      
      // Adjust for zero start
      for (; nrb < irn.GetSize(); nrb++) if (irn[nrb] != 0) break;
    }
    
    // Build adjusted row array
    MarginalArray::Slice sub_rows = irn.Range(nrb, irn.GetSize() - 1);
    MarginalArray::Slice sub_cols = m_col_marginals.Range(kb + 1, m_col_marginals.GetSize() - 1);
    
    double ddf = logMultinomial(m_col_marginals[kb], row_diff);
    double drn = logMultinomial(ntot, sub_rows) - m_den_observed_path + ddf;
    
    int kval = 0;
    int path_idx = -1;
    double obs2;
    
    if (k > 2) {
      // compute hash table key for current table
      kval = irn[0] + irn[1] * m_key_multipliers[1];
      for (int i = 2; i < irn.GetSize(); i++) kval += irn[i] * m_key_multipliers[i];
      
      obs2 = m_observed_path - m_facts[m_col_marginals[kb + 1]] - m_facts[m_col_marginals[kb + 2]] - ddf;
      for (int i = 3; i <= (k - 1); i++) obs2 -= m_facts[m_col_marginals[kb + i]];
      
      if (!m_path_extremes[k].Find(kval, path_idx)) {
        m_path_extremes_mutex.Lock();
        {
          // Move completed paths to the local table
          bool found = false;
          for (int i = 0; i < m_completed_path_extremes[k].GetSize(); i++) {
            PathExtremes& path = m_completed_path_extremes[k][i];
            m_path_extremes[k].Find(path.key, path_idx);
            m_path_extremes[k][path_idx].longest_path = path.longest_path;
            m_path_extremes[k][path_idx].shortest_path = path.shortest_path;
            if (path.key == kval) found = true;
          }
          m_completed_path_extremes[0].Resize(0);
          
          // Add new pending path if not found and not already in pending table
          if (!found && !m_pending_path_extremes[k].Find(kval, path_idx)) {
            m_pending_path_extremes[k][path_idx].Set(sub_rows, sub_cols, obs2, ddf, ntot);
            m_path_extremes_queue.Push(k);
          }
        }
        m_path_extremes_mutex.Unlock();
        m_path_extremes_cond.Signal();
      }
      
      // Push node onto pending stack
      int pending_idx = m_pending_path_nodes[k].GetSize();
      m_pending_path_nodes[k].Resize(pending_idx + 1);
      m_pending_path_nodes[k][pending_idx].Set(cur_node, obs2, drn, ddf, kval);
    } else {
      obs2 = m_observed_path - drn - m_den_observed_path;
      int pending_idx = m_pending_path_nodes[k].GetSize();
      m_pending_path_nodes[k].Resize(pending_idx + 1);
      m_pending_path_nodes[k][pending_idx].Set(cur_node, obs2, drn, ddf, kval, true);      
    }
    
  } while (generateNewDaughter(kmax, row_marginals, row_diff, kd, ks));
  
}

// Path Extremes Threading Methods
// -------------------------------------------------------------------------------------------------------------- 

void FExact::PathExtremesCalc::Run()
{
  while (true) {
    int k;
    int path_idx;

    m_fexact->m_path_extremes_mutex.Lock();
    {
      while (m_fexact->m_path_extremes_queue.GetSize() == 0 && m_run) {
        m_fexact->m_path_extremes_cond.Wait(m_fexact->m_path_extremes_mutex);
      }
      if (!m_run) {
        m_fexact->m_path_extremes_mutex.Unlock();
        return;
      }
      k = m_fexact->m_path_extremes_queue.Pop();      
      path_idx = m_fexact->m_pending_path_extremes[k].Pop();
    }
    m_fexact->m_path_extremes_mutex.Unlock();
    
    if (path_idx < 0) continue;
    
    PendingPathExtremes& p = m_fexact->m_pending_path_extremes[k][path_idx];
    
    double longest_path = m_fexact->longestPath(p.rows.Range(0, p.rows.GetSize() - 1), p.cols.Range(0, p.cols.GetSize() - 1), p.ntot);
    if (longest_path > 0.0) longest_path = 0.0;
    
    double dspt = m_fexact->m_observed_path - p.obs2 - p.ddf;
    double shortest_path = dspt;
    m_fexact->shortestPath(p.rows.Range(0, p.rows.GetSize() - 1), p.cols.Range(0, p.cols.GetSize() - 1), shortest_path);
    shortest_path -= dspt;
    if (shortest_path > 0.0) shortest_path = 0.0;
    
    m_fexact->m_path_extremes_mutex.Lock();
    {
      // Record completed calculation
      int completed_idx = m_fexact->m_completed_path_extremes[k].GetSize();
      m_fexact->m_completed_path_extremes[k].Resize(completed_idx + 1);
      m_fexact->m_completed_path_extremes[k][completed_idx].Set(p.key, longest_path, shortest_path);
      
      // Clear out the pending record
      m_fexact->m_pending_path_extremes[k].Remove(path_idx);
    }
    m_fexact->m_path_extremes_mutex.Unlock();
    m_fexact->m_path_extremes_cond_complete.Signal();
  }
}



// FExact Core Algorithm Support Methods
// -------------------------------------------------------------------------------------------------------------- 

inline bool FExact::generateFirstDaughter(const MarginalArray& row_marginals, int n, MarginalArray& row_diff, int& kmax, int& kd)
{
  row_diff.SetAll(0);
  
  kmax = row_marginals.GetSize() - 1;
  kd = row_marginals.GetSize();
  do {
    kd--;
    int ntot = (n < row_marginals[kd]) ? n : row_marginals[kd];
    row_diff[kd] = ntot;
    if (row_diff[kmax] == 0) kmax--;
    n -= ntot;
  } while (n > 0 && kd > 0);
  
  if (n != 0) return false;
  
  return true;
}


bool FExact::generateNewDaughter(int kmax, const MarginalArray& row_marginals, MarginalArray& row_diff, int& idx_dec, int& idx_inc)
{
  if (idx_inc == -1) {
    idx_inc++;
    while (row_diff[idx_inc] == row_marginals[idx_inc]) idx_inc++;
  }
  
  // Find node to decrement
  if (row_diff[idx_dec] > 0 && idx_dec > idx_inc) {
    row_diff[idx_dec]--;
    while (row_marginals[--idx_dec] == 0) ;
    int m = idx_dec;
    
    // Find node to increment
    while (row_diff[m] >= row_marginals[m]) m--;
    row_diff[m]++;
    
    if (m == idx_inc && row_diff[m] == row_marginals[m]) idx_inc = idx_dec;
  } else {
    int idx = 0;
    do {
      // Check for finish
      idx = idx_dec + 1;
      bool found = false;
      for (; idx < row_diff.GetSize(); idx++) {
        if (row_diff[idx] > 0) {
          found = true;
          break;
        }
      }
      if (!found) return false;
      
      int marginal_total = 1;
      for (int i = 0; i <= idx_dec; i++) {
        marginal_total += row_diff[i];
        row_diff[i] = 0;
      }
      idx_dec = idx;
      do {
        idx_dec--;
        int m = (marginal_total < row_marginals[idx_dec]) ? marginal_total : row_marginals[idx_dec];
        row_diff[idx_dec] = m;
        marginal_total -= m;
      } while (marginal_total > 0 && idx_dec != 0);
      
      if (marginal_total > 0) {
        if (idx != (kmax)) {
          idx_dec = idx;
          continue;
        }
        return false;
      } else {
        break;
      }
    } while (true);
    row_diff[idx]--;
    for (idx_inc = 0; row_diff[idx_inc] >= row_marginals[idx_inc]; idx_inc++) if (idx_inc > idx_dec) break;
  }
  
  return true;
}


FExact::NodePtr FExact::handlePastPaths(NodePtr& cur_node, double obs2, double obs3, double ddf, double drn, int kval,
                             NodeHashTable& nht)
{
  NodePtr new_node(NULL);
  for (int i = 0; i < cur_node->past_entries.GetSize(); i++) {
    double past_path = cur_node->past_entries[i].value;
    int path_freq = cur_node->past_entries[i].observed;
    if (past_path <= obs3) {
      // Path shorter than longest path, add to the pvalue and continue
      m_pvalue += (double)(path_freq) * exp(past_path + drn);
    } else if (past_path < obs2) {
      int nht_idx;
      double new_path = past_path + ddf;
      if (nht.Find(kval, nht_idx)) {
        // Existing Node was found            
        recordPath(new_path, path_freq, nht[nht_idx].past_entries);
      } else {
        // New Node added, insert this observed path
        new_node = nht.Get(nht_idx);
        new_node->past_entries.Push(PastPathLength(new_path, path_freq));
      }
    }
  }
  return new_node;
}


void FExact::recordPath(double path_length, int path_freq, Array<PastPathLength, Smart>& past_entries)
{
  // Search for past path within m_tolerance and add observed frequency to it
  double test1 = path_length - m_tolerance;
  double test2 = path_length + m_tolerance;
  
  int j = 0;
  int old_j = 0;
  while (true) {
    double test_path = past_entries[j].value;
    if (test_path < test1) {
      old_j = j;
      j = past_entries[j].next_left;
      if (j >= 0) continue;
    } else if (test_path > test2) {
      old_j = j;
      j = past_entries[j].next_right;
      if (j >= 0) continue;
    } else {
      past_entries[j].observed += path_freq;
      return;
    }
    break;
  }
  
  // If no path within m_tolerance is found, add new past path length to the node
  int new_idx = past_entries.GetSize();
  past_entries.Push(PastPathLength(path_length, path_freq)); 
  
  double test_path = past_entries[old_j].value;
  if (test_path < test1) {
    past_entries[old_j].next_left = new_idx;
  } else if (test_path > test2) {
    past_entries[old_j].next_right = new_idx;
  } else {
    assert(false);
  }  
}


void FExact::unpackMarginals(int key, MarginalArray& row_marginals)
{
  for (int i = row_marginals.GetSize() - 1; i > 0; i--) {
    row_marginals[i] = key / m_key_multipliers[i];
    key -= row_marginals[i] * m_key_multipliers[i];
  }
  row_marginals[0] = key;
}



// FExact Longest Path Methods
// -------------------------------------------------------------------------------------------------------------- 

double FExact::longestPath(const MarginalArray::Slice& row_marginals, const MarginalArray::Slice& col_marginals, int marginal_total)
{
  class ValueHashTable
  {
  private:
    Array<Pair<int, double> >* m_table;
    Array<int> m_stack;
    int m_entry_count;
    
  public:
    inline ValueHashTable(int size = 200) : m_table(new Array<Pair<int, double> >(size)), m_stack(size) { ClearTable(); }
    inline ~ValueHashTable() { delete m_table; }
    
    int GetEntryCount() const { return m_entry_count; }
    
    bool Find(int key, int& idx)
    {
      int init = key % m_table->GetSize();
      idx = init;
      for (; idx < m_table->GetSize(); idx++) {
        if ((*m_table)[idx].Value1() < 0) {
          m_stack[m_entry_count] = idx;
          (*m_table)[idx].Value1() = key;
          m_entry_count++;
          return false;
        } else if ((*m_table)[idx].Value1() == key) {
          return true;
        }
      }
      for (idx = 0; idx < init; idx++) {
        if ((*m_table)[idx].Value1() < 0) {
          m_stack[m_entry_count] = idx;
          (*m_table)[idx].Value1() = key;
          m_entry_count++;
          return false;
        } else if ((*m_table)[idx].Value1() == key) {
          return true;
        }
      }
      Rehash(key, idx);
      return false;
    }
    
    inline double& operator[](int idx) { return (*m_table)[idx].Value2(); }
    
    Pair<int, double> Pop()
    {
      Pair<int, double> tmp = (*m_table)[m_stack[--m_entry_count]];
      (*m_table)[m_stack[m_entry_count]].Value1() = -1;
      return tmp;
    }
    
    inline void ClearTable()
    {
      m_entry_count = 0;
      for (int i = 0; i < m_table->GetSize(); i++) (*m_table)[i].Value1() = -1;
    }
    
  private:
    void Rehash(int key, int& idx)
    {
      Array<Pair<int, double> >* old_table = m_table;
      m_table = new Array<Pair<int, double> >(old_table->GetSize() * 2);
      for (int i = 0; i < m_table->GetSize(); i++) (*m_table)[i].Value1() = -1;
      m_stack.Resize(old_table->GetSize() * 2);
      for (int i = 0; i < old_table->GetSize(); i++) {
        int t_idx;
        Find((*old_table)[i].Value1(), t_idx);
        (*m_table)[t_idx].Value2() = (*old_table)[i].Value2();
      }
      Find(key, idx);
      delete old_table;
    }
  };
  
  // 1 x c
  if (row_marginals.GetSize() <= 1) {
    double longest_path = 0.0;
    for (int i = 0; i < col_marginals.GetSize(); i++) longest_path -= m_facts[col_marginals[i]];
    return longest_path;
  }
  
  // r x 1
  if (col_marginals.GetSize() <= 1) {
    double longest_path = 0.0;
    for (int i = 0; i < row_marginals.GetSize(); i++) longest_path -= m_facts[row_marginals[i]];
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
    min = longestPathSpecial(row_marginals, col_marginals, val);
  }
  if (!min && col_marginals[col_marginals.GetSize() - 1] <= col_marginals[0] + row_marginals.GetSize()) {
    min = longestPathSpecial(col_marginals, row_marginals, val);
  }
  
  if (min) {
    return -val;
  }
  

  int ntot = marginal_total;
  MarginalArray lrow;
  MarginalArray lcol;
  
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
  

  Array<double> alen(col_marginals.GetSize() + 1);
  alen.SetAll(0.0);
  
  ValueHashTable vht[2];
  int active_vht = 0;
  
  double vmn = 1.0e10;
  int nc1s = lcol.GetSize() - 2;
  int kyy = lcol[lcol.GetSize() - 1] + 1;
  
  Array<int> lb(lrow.GetSize());
  Array<int> nu(lrow.GetSize());
  Array<int> nr(lrow.GetSize());
  
  
  while (true) {
    bool continue_main = false;
    
    // Setup to generate new node
    int lev = 0;
    int nr1 = lrow.GetSize() - 1;
    int nrt = lrow[0];
    int nct = lcol[0];
    lb[0] = (int)((((double)nrt + 1.0) * (nct + 1)) / (double)(ntot + nr1 * (nc1s + 1) + 1) - m_tolerance) - 1;
    nu[0] = (int)((((double)nrt + nc1s + 1.0) * (nct + nr1)) / (double)(ntot + nr1 + nc1s + 1)) - lb[0] + 1;
    nr[0] = nrt - lb[0];
    
    while (true) {
      do {
        nu[lev]--;
        if (nu[lev] == 0) {
          if (lev == 0) {
            do {
              if (vht[(active_vht) ? 0 : 1].GetEntryCount()) {
                Pair<int, double> entry = vht[(active_vht) ? 0 : 1].Pop();
                val = entry.Value2();
                int key = entry.Value1();
                
                // Compute Marginals
                for (int i = lcol.GetSize() - 1; i > 0; i--) {
                  lcol[i] = key % kyy;
                  key = key / kyy;
                }
                lcol[0] = key;
                
                // Set up nt array
                nt[0] = ntot - lcol[0];
                for (int i = 1; i < lcol.GetSize(); i++) nt[i] = nt[i - 1] - lcol[i];
               
                min = false;
                if (lrow[lrow.GetSize() - 1] <= lrow[0] + lcol.GetSize()) {
                  min = longestPathSpecial(lrow.Range(0, lrow.GetSize() - 1), lcol.Range(0, lcol.GetSize() - 1), val);
                }
                if (!min && lcol[lcol.GetSize() - 1] <= lcol[0] + lrow.GetSize()) {
                  min = longestPathSpecial(lrow.Range(0, lrow.GetSize() - 1), lcol.Range(0, lcol.GetSize() - 1), val);
                }
                
                if (min) {
                  if (val < vmn)
                    vmn = val;
                  continue;
                }
                continue_main = true;
              } else if (lrow.GetSize() > 2 && vht[active_vht].GetEntryCount()) {
                // Go to next level
                ntot -= lrow[0];
                Array<int> tmp(lrow);
                lrow.ResizeClear(lrow.GetSize() - 1);
                for (int i = 0; i < lrow.GetSize(); i++) lrow[i] = tmp[i + 1];
                active_vht = (active_vht) ? 0 : 1;
                continue;
              }
              break;
            } while (true);
            if (!continue_main)
              return -vmn;
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
      
      for (alen[lev + 1] = alen[lev] + m_facts[lb[lev]]; lev < nc1s; alen[lev + 1] = alen[lev] + m_facts[lb[lev]]) {
        int nn1 = nt[lev];
        int nrt = nr[lev];
        lev++;
        int nc1 = lcol.GetSize() - lev - 1;
        int nct = lcol[lev];
        lb[lev] = (int)((double)((nrt + 1) * (nct + 1)) / (double)(nn1 + nr1 * nc1 + 1) - m_tolerance);
        nu[lev] = (int)((double)((nrt + nc1) * (nct + nr1)) / (double)(nn1 + nr1 + nc1) - lb[lev] + 1);
        nr[lev] = nrt - lb[lev];
      }
      alen[lcol.GetSize()] = alen[lev + 1] + m_facts[nr[lev]];
      lb[lcol.GetSize() - 1] = nr[lev];
            
      double v = val + alen[lcol.GetSize()];
      if (lrow.GetSize() == 2) {
        for (int i = 0; i < lcol.GetSize(); i++) v += m_facts[lcol[i] - lb[i]];
        if (v < vmn)
          vmn = v;
      } else if (lrow.GetSize() == 3 && lcol.GetSize() == 2) {
        int nn1 = ntot - lrow[0] + 2;
        int ic1 = lcol[0] - lb[0];
        int ic2 = lcol[1] - lb[1];
        int n11 = (lrow[1] + 1) * (ic1 + 1) / nn1;
        int n12 = lrow[1] - n11;
        v += m_facts[n11] + m_facts[n12] + m_facts[ic1 - n11] + m_facts[ic2 - n12];
        if (v < vmn)
          vmn = v;
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


bool FExact::longestPathSpecial(const MarginalArray::Slice& row_marginals, const MarginalArray::Slice& col_marginals, double& val)
{
  Array<int> nd(row_marginals.GetSize() - 1);
  Array<int> ne(col_marginals.GetSize());
  Array<int> m(col_marginals.GetSize());
  
  nd.SetAll(0);
  int is = col_marginals[0] / row_marginals.GetSize();
  ne[0] = is;
  int ix = col_marginals[0] - row_marginals.GetSize() * is;
  m[0] = ix;
  if (ix != 0) nd[ix - 1] = 1;
  
  for (int i = 1; i < col_marginals.GetSize(); i++) {
    ix = col_marginals[i] / row_marginals.GetSize();
    ne[i] = ix;
    is += ix;
    ix = col_marginals[i] - row_marginals.GetSize() * ix;
    m[i] = ix;
    if (ix != 0) nd[ix - 1]++;
  }
  
  for (int i = nd.GetSize() - 2; i >= 0; i--) nd[i] += nd[i + 1];
  
  ix = 0;
  int nrow1 = row_marginals.GetSize() - 1;
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




// FExact Shortest Path Methods
// -------------------------------------------------------------------------------------------------------------- 

inline void FExact::removeFromVector(const Array<int, ManualBuffer>& src, int idx_remove, Array<int, ManualBuffer>& dest)
{
  dest.Resize(src.GetSize() - 1);
  for (int i = 0; i < idx_remove; i++) dest[i] = src[i];
  for (int i = idx_remove + 1; i < src.GetSize(); i++) dest[i - 1] = src[i];
}


inline void FExact::reduceZeroInVector(const Array<int, ManualBuffer>& src, int value, int idx_start, Array<int, ManualBuffer>& dest)
{
  dest.Resize(src.GetSize());
  
  int i = 0;
  for (; i < idx_start; i++) dest[i] = src[i];
  
  for (; i < (src.GetSize() - 1); i++) {
    if (value >= src[i + 1]) {
      break;
    }
    dest[i] = src[i + 1];
  }
  dest[i] = value;
  
  for (++i; i < src.GetSize(); i++) dest[i] = src[i];
}


void FExact::shortestPath(const MarginalArray::Slice& row_marginals, const MarginalArray::Slice& col_marginals, double& shortest_path)
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
      shortest_path += -m_facts[row_marginals[1]] - m_facts[col_marginals[0]] - m_facts[col_marginals[1] - row_marginals[1]];
    } else {
      shortest_path += -m_facts[col_marginals[1]] - m_facts[row_marginals[0]] - m_facts[row_marginals[1] - col_marginals[1]];
    }
    return;
  }
  
  // General Case
  
  
  const int ROW_BUFFER_SIZE = (row_marginals.GetSize() + col_marginals.GetSize() + 1) * row_marginals.GetSize();
  const int COL_BUFFER_SIZE = (row_marginals.GetSize() + col_marginals.GetSize() + 1) * col_marginals.GetSize();
  SmartPtr<int, NoCopy, ArrayStorage> row_data(new int[ROW_BUFFER_SIZE]);
  SmartPtr<int, NoCopy, ArrayStorage> col_data(new int[COL_BUFFER_SIZE]);
  Array<Array<int, ManualBuffer> > row_stack(row_marginals.GetSize() + col_marginals.GetSize() + 1);
  Array<Array<int, ManualBuffer> > col_stack(row_marginals.GetSize() + col_marginals.GetSize() + 1);
  for (int i = 0; i < row_stack.GetSize(); i++) {
    row_stack[i].SetBuffer(GetInternalPtr(row_data) + (i * row_marginals.GetSize()));
    col_stack[i].SetBuffer(GetInternalPtr(col_data) + (i * col_marginals.GetSize()));
  }
  
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
      
      if (row_stack[istk + 1].GetSize() == 1 || col_stack[istk + 1].GetSize() == 1) {
        if (row_stack[istk + 1].GetSize() == 1) {
          for (int i = 0; i < col_stack[istk + 1].GetSize(); i++) y += m_facts[col_stack[istk + 1][i]];
        }
        if (col_stack[istk + 1].GetSize() == 1) {
          for (int i = 0; i < row_stack[istk + 1].GetSize(); i++) y += m_facts[row_stack[istk + 1][i]];
        }
        
        if (y > amx) {
          amx = y;
          if (shortest_path - amx <= m_tolerance) {
            shortest_path = 0.0;
            return;
          }
        }
        
        bool continue_outer = false;
        for (--istk; istk >= 0; istk--) {
          l = l_stack[istk] + 1;
          for (; l < m_stack[istk]; l++) {
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
        if (continue_outer) continue;
        
        shortest_path -= amx;
        if (shortest_path - amx <= m_tolerance) shortest_path = 0.0;
        return;
      } else {
        break;
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
