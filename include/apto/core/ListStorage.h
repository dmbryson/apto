/*
 *  ListStorage.h
 *  Apto
 *
 *  Created by David on 3/8/11.
 *  Copyright 2010-2011 David Michael Bryson. All rights reserved.
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

#ifndef ListStorage_h
#define ListStorage_h

#include <cassert>


namespace Apto {
  
  // Storage Policies
  // --------------------------------------------------------------------------------------------------------------  
  
  template <class T> class SparseVector
  {
  public:
    class EntryHandle;
    class Iterator;
    class ConstIterator;
    
    
  private:
    static const int SEGMENT_SIZE = 16;
    
    int m_size;
    int m_segs;
        
    struct ListSegment
    {
      SparseVector<T>* list;
      int used;
      T entries[SEGMENT_SIZE];
      ListSegment* prev;
      ListSegment* next;
      EntryHandle* handles[SEGMENT_SIZE];
      
      ListSegment() : used(0), next(NULL) { ; }
      ListSegment(SparseVector<T>* in_list, const T& value, ListSegment* in_next, ListSegment* in_prev = NULL)
      : list(in_list), used(1), prev(in_prev), next(in_next)
      {
        entries[0] = value; handles[0] = NULL;
      }
    };
    
    ListSegment* m_head_seg;
    ListSegment* m_tail_seg;
    
    
  protected:
    SparseVector() : m_size(0), m_segs(0), m_head_seg(NULL), m_tail_seg(NULL) { ; }
    
    ~SparseVector()
    {
      ListSegment* next = m_head_seg;
      while (next) {
        ListSegment* cur = next;
        next = cur->next;
        delete cur;
      }
    }
    
    SparseVector& operator=(const SparseVector& rhs)
    {
      // TODO - SparseVector assignment operator is quick-and-dirty - should more efficiently copy over elements
      Clear();
      ConstIterator it = rhs.Begin();
      while (it.Next()) PushRear(*it.Get());
      return *this;
    }
    
    
    inline int GetSize() const { return m_size; }
    
    void Clear()
    {
      ListSegment* cur = m_head_seg;
      while (cur) {
        // Clear handles
        for (int i = 0; i < cur->used; i++) if (cur->handles[i]) cur->handles[i]->m_seg = NULL;
        
        ListSegment* next = cur->next;
        delete cur;
        cur = next;
      }
      m_head_seg = NULL;
      m_tail_seg = NULL;
      m_size = 0;
      m_segs = 0;
    }
    
    
    inline T& GetFirst() { return m_head_seg->entries[m_head_seg->used - 1]; }
    inline const T& GetFirst() const { return m_head_seg->entries[m_head_seg->used - 1]; }
    inline T& GetLast() { return m_tail_seg->entries[0]; }
    inline const T& GetLast() const { return m_tail_seg->entries[0]; }
    
    
    void Push(const T& value, EntryHandle** handle = NULL)
    {
      if (handle) delete *handle;
      if (m_head_seg && m_head_seg->used < SEGMENT_SIZE) {
        int idx = m_head_seg->used;
        m_head_seg->used++;
        m_head_seg->entries[idx] = value;
        if (handle) {
          *handle = m_head_seg->handles[idx] = new EntryHandle(m_head_seg, value);
        } else {
          m_head_seg->handles[idx] = NULL;
        }
      } else {
        m_head_seg = new ListSegment(this, value, m_head_seg);
        m_segs++;
        if (m_head_seg->next) {
          m_head_seg->next->prev = m_head_seg;
        } else {
          m_tail_seg = m_head_seg;
        }
        if (handle) *handle = m_head_seg->handles[0] = new EntryHandle(m_head_seg, value);
      }
      
      m_size++;
    }
    
    void PushRear(const T& value, EntryHandle** handle = NULL)
    {
      if (handle) delete *handle;
      if (m_tail_seg && m_tail_seg->used < SEGMENT_SIZE) {
        for (int i = m_tail_seg->used; i > 0; i--) m_tail_seg->entries[i] = m_tail_seg->entries[i - 1];
        for (int i = m_tail_seg->used; i > 0; i--) m_tail_seg->handles[i] = m_tail_seg->handles[i - 1];
        m_tail_seg->entries[0] = value;
        m_tail_seg->used++;
        if (handle) {
          *handle = m_tail_seg->handles[0] = new EntryHandle(m_tail_seg, value);
        } else {
          m_tail_seg->handles[0] = NULL;
        }      
      } else if (m_tail_seg && m_tail_seg->used == SEGMENT_SIZE) {
        m_tail_seg->next = new ListSegment(this, value, NULL, m_tail_seg);
        m_segs++;
        m_tail_seg = m_tail_seg->next;
        if (handle) *handle = m_tail_seg->handles[0] = new EntryHandle(m_tail_seg, value);
      } else {
        m_tail_seg = new ListSegment(this, value, NULL, NULL);
        m_segs++;
        m_head_seg = m_tail_seg;
        if (handle) *handle = m_tail_seg->handles[0] = new EntryHandle(m_tail_seg, value);
      }
      
      m_size++;
    }
    
    inline T Pop()
    {
      T rval = GetFirst();
      removeFromSeg(rval, m_head_seg, m_head_seg->used - 1);
      return rval;
    }
    
    T PopRear()
    {
      T rval = GetLast();
      removeFromSeg(rval, m_tail_seg, 0);
      return rval;
    }
    
    bool Remove(const T& value)
    {
      ListSegment* cur = m_head_seg;
      while (cur) {
        if (removeFromSeg(value, cur)) return true;
        cur = cur->next;
      }
      return false;
    }
    
    Iterator Begin() { return Iterator(this); }
    ConstIterator Begin() const { return ConstIterator(this); }
    
    
  public:
    int GetDataSize() const { return sizeof(T) * m_size; }
    int GetMemSize() const { return sizeof(ListSegment) * m_segs; }
    
  private:
    bool removeFromSeg(const T& value, ListSegment* cur, int entry_idx = 0)
    {
      for (; entry_idx < cur->used; entry_idx++) {
        if (cur->entries[entry_idx] == value) {
          cur->used--;
          if (cur->used == 0) {
            // Last entry in this segment, remove segment
            if (cur->prev) cur->prev->next = cur->next;
            if (cur->next) cur->next->prev = cur->prev;
            if (cur == m_head_seg) m_head_seg = cur->next;
            if (cur == m_tail_seg) m_tail_seg = cur->prev;
            
            m_segs--;
            if (cur->handles[0]) cur->handles[0]->m_seg = NULL;
            delete cur;
          } else if (cur->next && (cur->used + cur->next->used) <= SEGMENT_SIZE) {            
            // Pack the remaining entries in this segment onto the next
            for (int j = 0; j < entry_idx && j < cur->used; j++) cur->next->entries[cur->next->used + j] = cur->entries[j];
            for (int j = entry_idx; j < cur->used; j++) cur->next->entries[cur->next->used + j] = cur->entries[j + 1];
            
            // Move and update segment handles associated with moved entries
            for (int j = 0; j < entry_idx && j < cur->used; j++) {
              EntryHandle* handle = cur->handles[j];
              cur->next->handles[cur->next->used + j] = handle;
              if (handle) handle->m_seg = cur->next;
            }
            if (cur->handles[entry_idx]) cur->handles[entry_idx]->m_seg = NULL;
            for (int j = entry_idx; j < cur->used; j++) {
              EntryHandle* handle = cur->handles[j + 1];
              cur->next->handles[cur->next->used + j] = handle;
              if (handle) handle->m_seg = cur->next;
            }
            
            // Update used count on packed segment
            cur->next->used += cur->used;
            
            // Remove now empty segment
            if (cur->prev) cur->prev->next = cur->next;
            if (cur->next) cur->next->prev = cur->prev;
            if (cur == m_head_seg) m_head_seg = cur->next;
            m_segs--;
            delete cur;
          } else {
            // Shift remaining entries in this segment
            for (int j = entry_idx; j < cur->used; j++) cur->entries[j] = cur->entries[j + 1];
            if (cur->handles[entry_idx]) cur->handles[entry_idx]->m_seg = NULL;
            for (int j = entry_idx; j < cur->used; j++) cur->handles[j] = cur->handles[j + 1];
            assert((cur->entries[cur->used] = NULL) == NULL);
          }
          
          m_size--;
          return true;
        }
      }
      return false;
    }
    
    
  public:
    class Iterator
    {
      friend class SparseVector<T>;
    private:
      SparseVector<T>* m_list;
      ListSegment* m_cur;
      int m_pos;
      
      Iterator(); // @not_implemented
      
      Iterator(SparseVector<T>* list)
      : m_list(list), m_cur(list->m_head_seg), m_pos((list->m_head_seg) ? list->m_head_seg->used : -1) { ; }
      
      
    public:
      T* Get() {
        if (m_cur && m_pos >= 0 && m_pos < m_cur->used) {
          return &m_cur->entries[m_pos];
        }
        return NULL;
      }
      
      T* Next() {
        if (m_cur && m_pos > 0) {
          m_pos--;
          return &m_cur->entries[m_pos];
        }
        if (m_cur && m_pos <= 0) {
          m_cur = m_cur->next;
          if (m_cur) {
            m_pos = m_cur->used - 1;
            return &m_cur->entries[m_pos];
          }
        }
        return NULL;
      }
    };
    
    class ConstIterator
    {
      friend class SparseVector<T>;
    private:
      const SparseVector<T>* m_list;
      ListSegment* m_cur;
      int m_pos;
      
      ConstIterator(); // @not_implemented
      
      ConstIterator(const SparseVector<T>* list)
      : m_list(list), m_cur(list->m_head_seg), m_pos((list->m_head_seg) ? list->m_head_seg->used : -1) { ; }
      
    public:
      const T* Get() {
        if (m_cur && m_pos >= 0 && m_pos < m_cur->used) {
          return &m_cur->entries[m_pos];
        }
        return NULL;
      }
      
      const T* Next() {
        if (m_cur && m_pos > 0) {
          m_pos--;
          return &m_cur->entries[m_pos];
        }
        if (m_cur && m_pos <= 0) {
          m_cur = m_cur->next;
          if (m_cur) {
            m_pos = m_cur->used - 1;
            return &m_cur->entries[m_pos];
          }
        }
        return NULL;
      }
    };
    
    
    class EntryHandle
    {
      friend class SparseVector<T>;
    private:
      ListSegment* m_seg;
      T m_entry;
      
      EntryHandle(); // @not_implemented
      EntryHandle(const EntryHandle&); // @not_implemented
      EntryHandle& operator=(const EntryHandle&); // @not_implemented
      
      EntryHandle(ListSegment* seg, const T& entry) : m_seg(seg), m_entry(entry) { ; }
      
    public:
      bool IsValid() const { return (m_seg); }
      void Remove()
      {
        if (!m_seg) return;
        m_seg->list->removeFromSeg(m_entry, m_seg);
        m_seg = NULL;
      }
    };
  };
};

#endif
