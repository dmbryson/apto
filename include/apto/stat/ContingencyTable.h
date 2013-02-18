/*
 *  ContingencyTable.h
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

#ifndef AptoStatContingencyTable_h
#define AptoStatContingencyTable_h

#include <cassert>

#include "apto/core/Array.h"
#include "apto/platform/Visibility.h"


namespace Apto {
  namespace Stat {
    class ContingencyTable
    {
    protected:
      int m_nrow;
      int m_ncol;
      Array<int> m_table;
      Array<int> m_row_marginals;
      Array<int> m_col_marginals;
      int m_total;
      
      class RowProxy;
      class ConstRowProxy;
      class ElementProxy;
      
      ContingencyTable(); // @not_implemented
      
    public:
      LIB_EXPORT ContingencyTable(int nrow, int ncol);
      inline ContingencyTable(const ContingencyTable& rhs) { this->operator=(rhs); }
      
      int NumRows() const { return m_nrow; }
      int NumCols() const { return m_ncol; }
      
      LIB_EXPORT ContingencyTable& operator=(const ContingencyTable& rhs);
      
      inline ElementProxy ElementAt(int row, int col) { return ElementProxy(*this, row, col); }
      inline int ElementAt(int row, int col) const { return m_table[(row * m_ncol) + col]; }
      LIB_EXPORT void SetElement(int row, int col, int value);
      
      RowProxy operator[](int row) { return RowProxy(*this, row); }
      ConstRowProxy operator[](int row) const { return ConstRowProxy(*this, row); }
      
      const Array<int>& RowMarginals() const { return m_row_marginals; }
      const Array<int>& ColMarginals() const { return m_col_marginals; }
      int MarginalTotal() const { return m_total; }
      
    protected:
      class RowProxy
      {
      private:
        ContingencyTable& m_table;
        int m_row;
        
      public:
        inline RowProxy(ContingencyTable& table, int row) : m_table(table), m_row(row) { ; }
        
        inline ContingencyTable::ElementProxy operator[](int col) { return m_table.ElementAt(m_row, col); }
        inline int operator[](int col) const { return m_table.ElementAt(m_row, col); }
      };
      
      class ConstRowProxy
      {
      private:
        const ContingencyTable& m_table;
        int m_row;
        
      public:
        inline ConstRowProxy(const ContingencyTable& table, int row) : m_table(table), m_row(row) { ; }
        
        inline int operator[](int col) const { return m_table.ElementAt(m_row, col); }
      };
      
      class ElementProxy
      {
      private:
        ContingencyTable& m_table;
        int m_row;
        int m_col;
        
      public:
        inline ElementProxy(ContingencyTable& table, int row, int col) : m_table(table), m_row(row), m_col(col) { ; }
        
        inline ElementProxy& operator=(int value);
        inline ElementProxy& operator+=(int value);
        inline ElementProxy& operator-=(int value);
        inline ElementProxy& operator++();
        inline int operator++(int);
        inline ElementProxy& operator--();
        inline int operator--(int);
        inline operator int() const;
      };
      
      
    };
    
    
    inline ContingencyTable::ElementProxy& ContingencyTable::ElementProxy::operator=(int value)
    {
      m_table.SetElement(m_row, m_col, value);
      return *this;
    }
    
    inline ContingencyTable::ElementProxy& ContingencyTable::ElementProxy::operator+=(int value)
    {
      m_table.SetElement(m_row, m_col, value + m_table.ElementAt(m_row, m_col));
      return *this;
    }
    
    inline ContingencyTable::ElementProxy& ContingencyTable::ElementProxy::operator-=(int value)
    {
      m_table.SetElement(m_row, m_col, value - m_table.ElementAt(m_row, m_col));
      return *this;
    }
    
    inline ContingencyTable::ElementProxy& ContingencyTable::ElementProxy::operator++()
    {
      m_table.SetElement(m_row, m_col, m_table.ElementAt(m_row, m_col) + 1);
      return *this;
    }
    
    inline int ContingencyTable::ElementProxy::operator++(int)
    {
      int v = m_table.ElementAt(m_row, m_col);
      m_table.SetElement(m_row, m_col, m_table.ElementAt(m_row, m_col) + 1);
      return v;
    }
    
    inline ContingencyTable::ElementProxy& ContingencyTable::ElementProxy::operator--()
    {
      m_table.SetElement(m_row, m_col, m_table.ElementAt(m_row, m_col) - 1);
      return *this;
    }
    
    inline int ContingencyTable::ElementProxy::operator--(int)
    {
      int v = m_table.ElementAt(m_row, m_col);
      m_table.SetElement(m_row, m_col, m_table.ElementAt(m_row, m_col) - 1);
      return v;
    }
    
    inline ContingencyTable::ElementProxy::operator int() const
    {
      return static_cast<const ContingencyTable&>(m_table).ElementAt(m_row, m_col);
    }

  
  };
};

#endif
