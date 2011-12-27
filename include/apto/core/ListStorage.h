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

#ifndef AptoCoreListStorage_h
#define AptoCoreListStorage_h

#include "apto/core/Array.h"
#include "apto/core/Definitions.h"
#include "apto/platform/Platform.h"

#include <cassert>


namespace Apto {
  
  // List Storage Policies
  // --------------------------------------------------------------------------------------------------------------
  
  template <class T> class DL
  {
  public:
    class EntryHandle;
    class Iterator;
    class ConstIterator;
    
  private:
    class Node
    {
    public:
      T data;
      EntryHandle* handle;
      Node* next;
      Node* prev;
    };
    
    Node m_root;
    int m_size;
    
    DL(const DL&) { ; }
    
  protected:
    DL() : m_size(0)
    {
      m_root.next = &m_root;
      m_root.prev = &m_root;
    }
        
    ~DL()
    {
      // Clear handles and delete nodes
      Node* cur = &m_root;
      while (cur->next != &m_root) {
        Node* next = cur->next;
        if (cur != &m_root) delete cur;
        if (next->handle) next->handle->m_node = NULL;
        cur = next;
      }
    }
    
    DL& operator=(const DL& rhs)
    {
      if (this == &rhs) return *this;
      Clear();
      ConstIterator it = rhs.Begin();
      while (it.Next()) PushRear(*it.Get());
      return *this;
    }
    
    
    inline int GetSize() const { return m_size; }
    
    inline void Clear()
    {
      // Clear handles and delete nodes
      Node* cur = &m_root;
      while (cur->next != &m_root) {
        Node* next = cur->next;
        if (cur != &m_root) delete cur;
        if (next->handle) next->handle->m_node = NULL;
        cur = next;
      }
      
      m_root.next = &m_root;
      m_root.prev = &m_root;
      m_size = 0;
    }
    
    inline T& GetFirst() { return m_root.next->data; }
    inline const T& GetFirst() const { return m_root.next->data; }
    inline T& GetLast() { return m_root.prev->data; }
    inline const T& GetLast()  const { return m_root.prev->data; }
    
    inline T Pop() { return removeNode(m_root.next); }
    inline T PopRear() { return removeNode(m_root.prev); }
    inline T PopPos(int pos)
    {
      if (pos >= m_size) return NULL;
      Node* test_node = m_root.next;
      for (int i = 0; i < pos; i++) test_node = test_node->next;
      return removeNode(test_node);
    }
    
    
    void Push(const T& val, EntryHandle** handle = NULL)
    {
      if (handle) delete *handle;
      Node* node = new Node;
      node->data = val;
      if (handle) {
        *handle = node->handle = new EntryHandle(this, node);
      } else {
        node->handle = NULL;
      }
      node->next = m_root.next;
      node->prev = &m_root;
      m_root.next->prev = node;
      m_root.next = node;
      m_size++;
    }
    
    void PushRear(const T& val, EntryHandle** handle = NULL)
    {
      if (handle) delete *handle;
      Node* node = new Node;
      node->data = val;
      if (handle) {
        *handle = node->handle = new EntryHandle(this, node);
      } else {
        node->handle = NULL;
      }
      node->next = &m_root;
      node->prev = m_root.prev;
      m_root.prev->next = node;
      m_root.prev = node;
      m_size++;
    }
    
    bool Remove(const T& value)
    {
      for (Node* cur = &m_root; cur->next != &m_root; cur = cur->next) {
        if (cur->data == value) {
          removeNode(cur);
          return true;
        }
      }
      return false;
    }
    
    Iterator Begin() { return Iterator(this); }
    ConstIterator Begin() const { return ConstIterator(this); }
    
    
  private:
    T removeNode(Node* out_node)
    {
      // Make sure we're not trying to delete the root node!
      assert(out_node != &m_root);
      
      // Save the data and patch up the linked list.
      T out_data = out_node->data;
      if (out_node->handle) out_node->handle->m_node = NULL;
      out_node->prev->next = out_node->next;
      out_node->next->prev = out_node->prev;      
      delete out_node;
      
      m_size--;
      
      return out_data;
    }    
    
  public:
    class Iterator
    {
      friend class DL<T>;
    private:
      DL<T>* m_list;
      Node* m_cur;
      
      Iterator(); // @not_implemented
      
      Iterator(DL<T>* list) : m_list(list), m_cur(&list->m_root) { ; }
      
    public:
      inline T* Get() {
        if (m_cur && m_cur != &m_list->m_root) return &m_cur->data;
        return NULL;
      }
      
      inline T* Next() {
        if (m_cur) {
          m_cur = m_cur->next;
          if (m_cur == &m_list->m_root) {
            m_cur = NULL;
            return NULL;
          } else {
            return &m_cur->data;
          }
        }
        return NULL;
      }
    };
    
    class ConstIterator
    {
      friend class DL<T>;
    private:
      const DL<T>* m_list;
      const Node* m_cur;
      
      ConstIterator(); // @not_implemented
      
      ConstIterator(const DL<T>* list) : m_list(list), m_cur(&list->m_root) { ; }
      
    public:
      inline const T* Get() {
        if (m_cur && m_cur != &m_list->m_root) return &m_cur->data;
        return NULL;
      }
      
      inline const T* Next() {
        if (m_cur) {
          m_cur = m_cur->next;
          if (m_cur == &m_list->m_root) {
            m_cur = NULL;
            return NULL;
          } else {
            return &m_cur->data;
          }
        }
        return NULL;
      }
    };
    
    
    class EntryHandle
    {
      friend class DL<T>;
    private:
      DL<T>* m_list;
      Node* m_node;
      
      EntryHandle(); // @not_implemented
      EntryHandle(const EntryHandle&); // @not_implemented
      EntryHandle& operator=(const EntryHandle&); // @not_implemented
      
      EntryHandle(DL<T>* list, Node* node) : m_list(list), m_node(node) { ; }
      
    public:
      ~EntryHandle() { if (m_node) m_node->handle = NULL; }
      bool IsValid() const { return (m_node); }
      void Remove()
      {
        if (!m_node) return;
        m_list->removeNode(m_node);
        m_node = NULL;
      }
    };
    
  };
  
  
  template <class T> class BufferedDL
  {
  public:
    class EntryHandle;
    class Iterator;
    class ConstIterator;
    
  private:
    static const int BUFFER_SIZE = 64;

    typedef union {
      unsigned int index;
      struct {
#if APTO_PLATFORM(LITTLE_ENDIAN)
        unsigned int offset:6;
        unsigned int num:26;
#else
        unsigned int num:26;
        unsigned int offset:6;
#endif
      } buffer;
    } ListIndex;
    
    class Node
    {
    public:
      T data;
      EntryHandle* handle;
      Node* next;
      Node* prev;
    };
    
    class Buffer
    {
    private:
      Node* m_nodes;
      
    public:
      Buffer() : m_nodes(new Node[BUFFER_SIZE]) { ; }
      ~Buffer() { delete [] m_nodes; }
      
      Node& operator[](int idx) { return m_nodes[idx]; }
      const Node& operator[](int idx) const { return m_nodes[idx]; }
    };
    
    
    Array<Buffer, ManagedPointer> m_bufs;
    ListIndex m_next;
    Node m_root;
    unsigned int m_size;
    
    BufferedDL(const BufferedDL&) { ; }
    
  protected:
    BufferedDL() : m_size(0) { Clear(); }
    ~BufferedDL()
    {
      // Clear handles
      ListIndex i;
      for (i.index = 0; i.index < m_size; i.index++) {
        if (m_bufs[i.buffer.num][i.buffer.offset].handle) {
          m_bufs[i.buffer.num][i.buffer.offset].handle->m_node = NULL;
        }
      }
    }
    
    BufferedDL& operator=(const BufferedDL& rhs)
    {
      if (this == &rhs) return *this;
      Clear();
      ConstIterator it = rhs.Begin();
      while (it.Next()) PushRear(*it.Get());
      return *this;
    }
    

    inline int GetSize() const { return m_size; }
    
    inline void Clear()
    {
      // Clear handles
      ListIndex i;
      for (i.index = 0; i.index < m_size; i.index++) {
        if (m_bufs[i.buffer.num][i.buffer.offset].handle) {
          m_bufs[i.buffer.num][i.buffer.offset].handle->m_node = NULL;
        }
      }
      
      m_bufs.Resize(1);
      m_next.index = 0;
      m_root.next = &m_root;
      m_root.prev = &m_root;
      m_size = 0;
    }
    
    inline T& GetFirst() { return m_root.next->data; }
    inline const T& GetFirst() const { return m_root.next->data; }
    inline T& GetLast() { return m_root.prev->data; }
    inline const T& GetLast()  const { return m_root.prev->data; }

    inline T Pop() { return removeNode(m_root.next); }
    inline T PopRear() { return removeNode(m_root.prev); }
    inline T PopPos(int pos)
    {
      if (pos >= m_size) return NULL;
      Node* test_node = m_root.next;
      for (int i = 0; i < pos; i++) test_node = test_node->next;
      return removeNode(test_node);
    }
    
    
    void Push(const T& val, EntryHandle** handle = NULL)
    {
      if (handle) delete *handle;
      Node& node = m_bufs[m_next.buffer.num][m_next.buffer.offset];
      node.data = val;
      if (handle) {
        node.handle = new EntryHandle(this, &m_bufs[m_next.buffer.num][m_next.buffer.offset]);
        *handle = node.handle;
      } else {
        node.handle = NULL;
      }
      node.next = m_root.next;
      node.prev = &m_root;
      m_root.next->prev = &node;
      m_root.next = &node;
      incSize();
    }
    
    void PushRear(const T& val, EntryHandle** handle = NULL)
    {
      if (handle) delete *handle;
      Node& node = m_bufs[m_next.buffer.num][m_next.buffer.offset];
      node.data = val;
      if (handle) {
        node.handle = new EntryHandle(this, &m_bufs[m_next.buffer.num][m_next.buffer.offset]);
        *handle = node.handle;
      } else {
        node.handle = NULL;
      }
      node.next = &m_root;
      node.prev = m_root.prev;
      m_root.prev->next = &node;
      m_root.prev = &node;
      incSize();
    }
    
    bool Remove(const T& value)
    {
      ListIndex i;
      for (i.index = 0; i.index < m_size; i.index++) {
        if (m_bufs[i.buffer.num][i.buffer.offset].data == value) {
          removeNode(&m_bufs[i.buffer.num][i.buffer.offset]);
          return true;
        }
      }
      return false;
    }
    
    Iterator Begin() { return Iterator(this); }
    ConstIterator Begin() const { return ConstIterator(this); }
    
    
  private:
    T removeNode(Node* out_node)
    {
      // Make sure we're not trying to delete the root node!
      assert(out_node != &m_root);
      
      // Save the data and patch up the linked list.
      T out_data = out_node->data;
      if (out_node->handle) out_node->handle->m_node = NULL;
      out_node->prev->next = out_node->next;
      out_node->next->prev = out_node->prev;
      
      // Clean up now unused data with default (should, for example, force SmartPtr clean up)
      out_node->data = T();
      
      m_next.index--;
      Node* next_node = &m_bufs[m_next.buffer.num][m_next.buffer.offset];
      if (next_node != out_node) {
        *out_node = *next_node;
        out_node->next->prev = out_node;
        out_node->prev->next = out_node;
        if (out_node->handle) out_node->handle->m_node = out_node;
      }
      m_size--;
      
      if (m_bufs.GetSize() > (m_next.buffer.num + 1) && m_next.buffer.offset < (BUFFER_SIZE - 3))
        m_bufs.Resize(m_bufs.GetSize() - 1);
      
      return out_data;
    }
    
    
    inline void incSize()
    {
      m_next.index++;
      if (m_bufs.GetSize() <= m_next.buffer.num) m_bufs.Resize(m_bufs.GetSize() + 1);
      m_size++;
    }
    
  public:
    class Iterator
    {
      friend class BufferedDL<T>;
    private:
      BufferedDL<T>* m_list;
      Node* m_cur;
      
      Iterator(); // @not_implemented
      
      Iterator(BufferedDL<T>* list) : m_list(list), m_cur(&list->m_root) { ; }
      
    public:
      inline T* Get() {
        if (m_cur && m_cur != &m_list->m_root) return &m_cur->data;
        return NULL;
      }
      
      inline T* Next() {
        if (m_cur) {
          m_cur = m_cur->next;
          if (m_cur == &m_list->m_root) {
            m_cur = NULL;
            return NULL;
          } else {
            return &m_cur->data;
          }
        }
        return NULL;
      }
    };
    
    class ConstIterator
    {
      friend class BufferedDL<T>;
    private:
      const BufferedDL<T>* m_list;
      const Node* m_cur;
      
      ConstIterator(); // @not_implemented
      
      ConstIterator(const BufferedDL<T>* list) : m_list(list), m_cur(&list->m_root) { ; }
      
    public:
      inline const T* Get() {
        if (m_cur && m_cur != &m_list->m_root) return &m_cur->data;
        return NULL;
      }
      
      inline const T* Next() {
        if (m_cur) {
          m_cur = m_cur->next;
          if (m_cur == &m_list->m_root) {
            m_cur = NULL;
            return NULL;
          } else {
            return &m_cur->data;
          }
        }
        return NULL;
      }
    };
    
    
    class EntryHandle
    {
      friend class BufferedDL<T>;
    private:
      BufferedDL<T>* m_list;
      Node* m_node;
      
      EntryHandle(); // @not_implemented
      EntryHandle(const EntryHandle&); // @not_implemented
      EntryHandle& operator=(const EntryHandle&); // @not_implemented
      
      EntryHandle(BufferedDL<T>* list, Node* node) : m_list(list), m_node(node) { ; }
      
    public:
      ~EntryHandle() { if (m_node) m_node->handle = NULL; }
      bool IsValid() const { return (m_node); }
      void Remove()
      {
        if (!m_node) return;
        m_list->removeNode(m_node);
        m_node = NULL;
      }
    };

  };
  
  
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
    
    SparseVector(const SparseVector&) { ; }

    
  protected:
    SparseVector() : m_size(0), m_segs(0), m_head_seg(NULL), m_tail_seg(NULL) { ; }
    
    ~SparseVector()
    {
      ListSegment* cur = m_head_seg;
      while (cur) {
        // Clear handles
        for (int i = 0; i < cur->used; i++) if (cur->handles[i]) cur->handles[i]->m_seg = NULL;
        
        ListSegment* next = cur->next;
        delete cur;
        cur = next;
      }
    }
    
    SparseVector& operator=(const SparseVector& rhs)
    {
      if (this == &rhs) return *this;
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
            
            // Clean up now unused entry with default (should, for example, force SmartPtr clean up)
            cur->entries[cur->used] = T();
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
      ~EntryHandle()
      {
        if (m_seg) {
          for (int i = 0; i < m_seg->used; i++) {
            if (m_seg->handles[i] == this) {
              m_seg->handles[i] = NULL;
              break;
            }
          }
        }
      }
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
