/*
 *  Array.h
 *  Apto
 *
 *  Created by David on 2/11/11.
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

#ifndef AptoCoreArrayStorage_h
#define AptoCoreArrayStorage_h

#include "apto/core/Definitions.h"

#include <cassert>

namespace Apto {
  
  // Array Storage Policies
  // --------------------------------------------------------------------------------------------------------------
  
  template <class T> class Basic
  {
  protected:
    T* m_data;    // Data Array
    SizeType m_size;   // Array Size
    
    typedef T StoredType;
    
    explicit Basic(SizeType size = 0) : m_data(NULL), m_size(0) { ResizeClear(size); }
    Basic(const Basic& rhs) : m_data(NULL), m_size(0) { this->operator=(rhs); }
    ~Basic() { delete [] m_data; }
    
    SizeType GetSize() const { return m_size; }
    
    void ResizeClear(const SizeType in_size)
    {
      if (m_data != NULL) delete [] m_data;
      
      if (in_size > 0) {
        m_data = new T[in_size];  // Allocate block for data
        assert(m_data != NULL);   // Memory allocation error: Out of Memory?
      }
      else m_data = NULL;
      
      m_size = in_size;
    }
    
    void Resize(SizeType new_size)
    {
      // If we're already at the size we want, don't bother doing anything.
      if (new_size == m_size) return;
      
      // If new size is 0, clean up and go!
      if (new_size == 0) {
        if (m_data != NULL) delete [] m_data;
        m_data = NULL;
        m_size = 0;
        return;
      }
      
      T* new_data = new T[new_size];
      assert(new_data != NULL); // Memory Allocation Error: Out of Memory?
      
      // Copy over old data...
      for (SizeType i = 0; i < m_size && i < new_size; i++) new_data[i] = m_data[i];
      if (m_data != NULL) delete [] m_data;  // remove old data if exists
      
      m_data = new_data;
      m_size = new_size;
    }
    
    Basic& operator=(const Basic& rhs)
    {
      if (m_size != rhs.m_size) ResizeClear(rhs.m_size);
      for (SizeType i = 0; i < rhs.m_size; i++) m_data[i] = rhs.m_data[i];
      return *this;
    }
    
    inline T& operator[](const SizeType index) { return m_data[index]; }
    inline const T& operator[](const SizeType index) const { return m_data[index]; }
    
    void Swap(SizeType idx1, SizeType idx2)
    {
      T v = m_data[idx1];
      m_data[idx1] = m_data[idx2];
      m_data[idx2] = v;
    }
  };
  
  
  template <class T> class Smart
  {
  protected:
    T* m_data;    // Data Array
    SizeType m_size;   // Array Size
    SizeType m_active; // Active Size
    SizeType m_reserve;
    
    // "I am so smart..."
    static const SizeType SMRT_INCREASE_MINIMUM = 4;
    static const SizeType SMRT_INCREASE_FACTOR = 2;
    static const SizeType SMRT_SHRINK_TEST_FACTOR = 4;
    
    typedef T StoredType;
    
    explicit Smart(SizeType size = 0) : m_data(NULL), m_size(0), m_active(0), m_reserve(0) { ResizeClear(size); }
    Smart(const Smart& rhs) : m_data(NULL), m_size(0), m_active(0), m_reserve(0) { this->operator=(rhs); }
    ~Smart() { delete [] m_data; }
    
    SizeType GetSize() const { return m_active; }
    
    void ResizeClear(const SizeType in_size)
    {
      m_active = in_size;
      m_size = (in_size >= m_reserve) ? in_size : m_reserve;    
      
      if (m_data != NULL) delete [] m_data;
      
      if (m_size > 0) {
        m_data = new T[m_size];   // Allocate block for data
        assert(m_data != NULL); // Memory allocation error: Out of Memory?
      }
      else m_data = NULL;
    }
    
    void Resize(SizeType new_size)
    {
      // If we're already at the size we want, don't bother doing anything.
      if (new_size == m_active) return;
      
      // If new size is 0, clean up and go!
      if (new_size == 0) {
        if (m_size > m_reserve && m_size > SMRT_INCREASE_MINIMUM) {
          // clean up if more than reserve or the minimum, otherwise keep the memory
          if (m_data != NULL) delete [] m_data;
          m_data = NULL;
          m_size = 0;
        }
        m_active = 0;
        return;
      }
      
      // Determine if we need to adjust the allocated array sizes...
      SizeType shrink_test = new_size * SMRT_SHRINK_TEST_FACTOR;
      if (new_size > m_size || (shrink_test < m_size && shrink_test >= m_reserve)) {
        SizeType new_array_size = new_size * SMRT_INCREASE_FACTOR;
        SizeType new_array_min = (new_size + SMRT_INCREASE_MINIMUM) < m_reserve ? m_reserve : (new_size + SMRT_INCREASE_MINIMUM);
        if (new_array_min > new_array_size) new_array_size = new_array_min;
        
        T* new_data = new T[new_array_size];
        assert(new_data != NULL); // Memory Allocation Error: Out of Memory?
        
        // Copy over old data...
        for (SizeType i = 0; i < m_active && i < new_size; i++) {
          new_data[i] = m_data[i];
        }
        if (m_data != NULL) delete [] m_data;  // remove old data if exists
        m_data = new_data;
        
        m_size = new_array_size;
      }
      
      m_active = new_size;
    }

    Smart& operator=(const Smart& rhs)
    {
      if (GetSize() != rhs.GetSize()) ResizeClear(rhs.GetSize());
      for (SizeType i = 0; i < rhs.GetSize(); i++) m_data[i] = rhs.m_data[i];
      return *this;
    }
    

    inline T& operator[](const SizeType index) { return m_data[index]; }
    inline const T& operator[](const SizeType index) const { return m_data[index]; }
    
    void Swap(SizeType idx1, SizeType idx2)
    {
      T v = m_data[idx1];
      m_data[idx1] = m_data[idx2];
      m_data[idx2] = v;
    }
    
    
  public:
    SizeType GetReserve() const { return m_reserve; }
    void SetReserve(SizeType reserve) { m_reserve = reserve; }
    SizeType GetCapacity() const { return m_size; }
  };
  
  
  template <class T> class ManagedPointer
  {
  protected:
    T** m_data;    // Data Array
    SizeType m_size;   // Array Size
    
    typedef T StoredType;
    
    explicit ManagedPointer(SizeType size = 0) : m_data(NULL), m_size(0) { ResizeClear(size); }
    ManagedPointer(const ManagedPointer& rhs) : m_data(NULL), m_size(0) { this->operator=(rhs); }
    
    ~ManagedPointer()
    {
      for (SizeType i = 0; i < m_size; i++) delete m_data[i];
      delete [] m_data;
    }
    
    SizeType GetSize() const { return m_size; }
    
    void ResizeClear(const SizeType in_size)
    {
      for (SizeType i = 0; i < m_size; i++) delete m_data[i];
      if (m_data != NULL) delete [] m_data;  // remove old data if exists
      
      m_size = in_size;
      assert(m_size >= 0);  // Invalid size specified for array intialization
      
      if (m_size > 0) {
        m_data = new T*[m_size];   // Allocate block for data
        assert(m_data != NULL); // Memory allocation error: Out of Memory?
        for (SizeType i = 0; i < m_size; i++) m_data[i] = new T;
      }
      else m_data = NULL;
    }
    
    void Resize(SizeType new_size)
    {
      // If we're already at the size we want, don't bother doing anything.
      if (m_size == new_size) return;
      
      // If new size is 0, clean up and go!
      if (new_size == 0) {
        for (SizeType i = 0; i < m_size; i++) delete m_data[i];
        if (m_data != NULL) delete [] m_data;
        m_data = NULL;
        m_size = 0;
        return;
      }
      
      T** new_data = new T*[new_size];
      assert(new_data != NULL); // Memory Allocation Error: Out of Memory?
      
      if (m_size < new_size) {
        // Fill out the new portion of the array, if needed
        for (SizeType i = m_size; i < new_size; i++) new_data[i] = new T;
      } else if (new_size < m_size) {
        // Clean up old portion of the array, if needed
        for (SizeType i = new_size; i < m_size; i++) delete m_data[i];
      }
      
      // Copy over old data...
      for (SizeType i = 0; i < m_size && i < new_size; i++) {
        new_data[i] = m_data[i];
      }
      if (m_data != NULL) delete [] m_data;  // remove old data if exists
      m_data = new_data;
      
      m_size = new_size;
    }

    ManagedPointer& operator=(const ManagedPointer& rhs)
    {
      if (m_size != rhs.GetSize()) Resize(rhs.GetSize());
      for(SizeType i = 0; i < m_size; i++) *m_data[i] = rhs[i];
      return *this;
    }

    inline T& operator[](const SizeType index) { return *m_data[index]; }
    inline const T& operator[](const SizeType index) const { return *m_data[index]; }
    
    void Swap(SizeType idx1, SizeType idx2)
    {
      T* v = m_data[idx1];
      m_data[idx1] = m_data[idx2];
      m_data[idx2] = v;
    }
  };
};

#endif
