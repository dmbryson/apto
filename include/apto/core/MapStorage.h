/*
 *  MapStorage.h
 *  Apto
 *
 *  Created by David on 3/10/11.
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

#ifndef AptoCoreMapStorage_h
#define AptoCoreMapStorage_h

#include "apto/core/Array.h"
#include "apto/core/Pair.h"


namespace Apto {
  
  // Map - Hashing Functions
  // --------------------------------------------------------------------------------------------------------------
  
  // HASH_TYPE = basic object
  // Casts the pointer to an int, shift right last two bit positions, mod by
  // the size of the hash table and hope for the best.  The shift is to account
  // for typical 4-byte alignment of pointer values.  Depending on architecture
  // this may not be true and could result in suboptimal hashing at higher
  // order alignments.
  template <class T, int HashFactor> class HashKey
  {
  public:
    static int Hash(const T& key)
    {
      // Cast/Dereference of key as an int* tells the compiler that we really want
      // to truncate the value to an integer, even if a pointer is larger.
      return abs((*((int*)&key) >> 2) % HashFactor);         
    }
  };
  
  // HASH_TYPE = int
  // Simply mod the into by the size of the hash table and hope for the best
  template<int HashFactor> class HashKey<int, HashFactor>
  {
  public:
    static int Hash(const int key)
    {
      return abs(key % HashFactor);
    }
  };
  
  // HASH_TYPE = double
  // Simply mod the into by the size of the hash table and hope for the best
  template<int HashFactor> class HashKey<double, HashFactor>
  {
  public:
    static int Hash(const double key)
    {
      return abs((int)key % HashFactor);
    }
  };
  

  // Map Storage Policies
  // --------------------------------------------------------------------------------------------------------------
  
  template <class K, class V, int HashFactor, template <class, int> class HashFunctor = HashKey> class HashBTree
  {
  protected:
    typedef HashFunctor<K, HashFactor> HF;
  public:
    class Iterator;
    class ConstIterator;
    class KeyIterator;
    class ValueIterator;
    
  protected:
    struct Entry {
      K key;
      V value;
      int left;
      int right;
      int parent;
    };
    
    Array<Entry, Smart> m_table[HashFactor];
    int m_size;
    
    HashBTree() : m_size(0) { ; }
    
    
    inline int GetSize() const { return m_size; }
    
    void Clear()
    {
      for (int i = 0; i < HashFactor; i++) m_table[i].Resize(0);
      m_size = 0;
    }
    
    const V* Find(const K& key) const
    {
      int hash = HF::Hash(key);
      if (m_table[hash].GetSize() == 0) return NULL;
      int cur_idx = 0;
      while (true) {
        if (key < m_table[hash][cur_idx].key) {
          cur_idx = m_table[hash][cur_idx].left;
          if (cur_idx >= 0) continue;
        } else if (key > m_table[hash][cur_idx].key) {
          cur_idx = m_table[hash][cur_idx].right;
          if (cur_idx >= 0) continue;
        } else {
          return &m_table[hash][cur_idx].value;
        }
        break;
      }
      return NULL;
    }

    V* Find(const K& key)
    {
      int hash = HF::Hash(key);
      if (m_table[hash].GetSize() == 0) return NULL;
      int cur_idx = 0;
      while (true) {
        if (key < m_table[hash][cur_idx].key) {
          cur_idx = m_table[hash][cur_idx].left;
          if (cur_idx >= 0) continue;
        } else if (key > m_table[hash][cur_idx].key) {
          cur_idx = m_table[hash][cur_idx].right;
          if (cur_idx >= 0) continue;
        } else {
          return &m_table[hash][cur_idx].value;
        }
        break;
      }
      return NULL;
    }
    
    
    V& Get(const K& key)
    {
      int hash = HF::Hash(key);
      if (m_table[hash].GetSize() == 0) {
        // Create root node
        m_table[hash].Resize(1);
        m_table[hash][0].key = key;
        m_table[hash][0].left = -1;
        m_table[hash][0].right = -1;
        m_table[hash][0].parent = -1;
        m_size++;
        
        return m_table[hash][0].value;
      }
      
      // Search the tree for the key
      int cur_idx = 0;
      int past_idx = 0;
      while (true) {
        if (key < m_table[hash][cur_idx].key) {
          past_idx = cur_idx;
          cur_idx = m_table[hash][cur_idx].left;
          if (cur_idx >= 0) continue;
        } else if (key > m_table[hash][cur_idx].key) {
          past_idx = cur_idx;
          cur_idx = m_table[hash][cur_idx].right;
          if (cur_idx >= 0) continue;
        } else {
          return m_table[hash][cur_idx].value;
        }
        break;
      }
      
      // Add new entry into the tree
      cur_idx = m_table[hash].GetSize();
      m_table[hash].Resize(cur_idx + 1);
      m_table[hash][cur_idx].key = key;
      m_table[hash][cur_idx].left = -1;
      m_table[hash][cur_idx].right = -1;
      m_table[hash][cur_idx].parent = past_idx;
      m_size++;
      
      // Update b-tree
      if (key < m_table[hash][past_idx].key) {
        m_table[hash][past_idx].left = cur_idx;
      } else {
        m_table[hash][past_idx].right = cur_idx;
      }
      
      return m_table[hash][cur_idx].value;
    }
    
    
    bool Remove(const K& key)
    {
      int hash = HF::Hash(key);
      if (m_table[hash].GetSize() == 0) return false;
      
      int cur_idx = 0;
      int past_idx = 0;
      while (true) {
        if (key < m_table[hash][cur_idx].key) {
          past_idx = cur_idx;
          cur_idx = m_table[hash][cur_idx].left;
          if (cur_idx >= 0) continue;
        } else if (key > m_table[hash][cur_idx].key) {
          past_idx = cur_idx;
          cur_idx = m_table[hash][cur_idx].right;
          if (cur_idx >= 0) continue;
        } else {
          int last_idx = m_table[hash].GetSize() - 1;
          
          if (cur_idx != last_idx) {
            if (m_table[hash][cur_idx].left < 0 && m_table[hash][cur_idx].right < 0) {
              // handle removing leaf node
              
              // Update parent node subtree index to be blank
              if (m_table[hash][past_idx].left == cur_idx) m_table[hash][past_idx].left = -1;
              else m_table[hash][past_idx].right = -1;
              
              // Move last node to this position in the array
              m_table[hash][cur_idx] = m_table[hash][last_idx];
              int parent_idx = m_table[hash][last_idx].parent;
              if (m_table[hash][parent_idx].left == last_idx) m_table[hash][parent_idx].left = cur_idx;
              else m_table[hash][parent_idx].right = cur_idx;
              
            } else if (m_table[hash][cur_idx].left < 0) {
              // all child nodes greater than current
              
              // Find most extreme right node
              int sub_idx = m_table[hash][cur_idx].right;
              while (m_table[hash][sub_idx].right > 0) sub_idx = m_table[hash][sub_idx].right;
              
              // Move extreme right node to current position
              int right_idx = m_table[hash][cur_idx].right;             // Save right subtree index
              m_table[hash][cur_idx].key = m_table[hash][sub_idx].key;
              m_table[hash][cur_idx].value = m_table[hash][sub_idx].value;
              m_table[hash][m_table[hash][sub_idx].parent].right = m_table[hash][sub_idx].left;
              if (m_table[hash][sub_idx].left >= 0)
                m_table[hash][m_table[hash][sub_idx].left].parent = m_table[hash][sub_idx].parent;
              if (sub_idx != right_idx) m_table[hash][cur_idx].left = right_idx;
              else m_table[hash][cur_idx].left = -1;
              m_table[hash][cur_idx].right = -1;
              
              // Move last node to the position of the old subtree leaf node
              if (sub_idx != last_idx) {
                m_table[hash][sub_idx] = m_table[hash][last_idx];
                int parent_idx = m_table[hash][last_idx].parent;
                if (m_table[hash][parent_idx].left == last_idx) m_table[hash][parent_idx].left = sub_idx;
                else m_table[hash][parent_idx].right = sub_idx;
              }
            } else if (m_table[hash][cur_idx].right < 0) {
              // all child node less than current
              
              // Find most extreme left node
              int sub_idx = m_table[hash][cur_idx].left;
              while (m_table[hash][sub_idx].left > 0) sub_idx = m_table[hash][sub_idx].left;
              
              // Move extreme right node to current position
              int left_idx = m_table[hash][cur_idx].left;               // Save right subtree index
              m_table[hash][cur_idx].key = m_table[hash][sub_idx].key;
              m_table[hash][cur_idx].value = m_table[hash][sub_idx].value;
              m_table[hash][m_table[hash][sub_idx].parent].left = m_table[hash][sub_idx].right;
              if (m_table[hash][sub_idx].right >= 0) 
                m_table[hash][m_table[hash][sub_idx].right].parent = m_table[hash][sub_idx].parent;
              if (sub_idx != left_idx) m_table[hash][cur_idx].right = left_idx;
              else m_table[hash][cur_idx].right = -1;
              m_table[hash][cur_idx].left = -1;
              
              // Move last node to the position of the old subtree leaf node
              if (sub_idx != last_idx) {
                m_table[hash][sub_idx] = m_table[hash][last_idx];
                int parent_idx = m_table[hash][last_idx].parent;
                if (m_table[hash][parent_idx].left == last_idx) m_table[hash][parent_idx].left = sub_idx;
                else m_table[hash][parent_idx].right = sub_idx;
              }
              
            } else {
              // tossup
              
              int sub_idx = m_table[hash][cur_idx].right;
              while (m_table[hash][sub_idx].left > 0) sub_idx = m_table[hash][sub_idx].left;
              
              // Move extreme right node to current position
              m_table[hash][cur_idx].key = m_table[hash][sub_idx].key;
              m_table[hash][cur_idx].value = m_table[hash][sub_idx].value;
              m_table[hash][m_table[hash][sub_idx].parent].right = m_table[hash][sub_idx].right;
              if (m_table[hash][sub_idx].right >= 0)
                m_table[hash][m_table[hash][sub_idx].right].parent = m_table[hash][sub_idx].parent;
              if (m_table[hash][cur_idx].right == sub_idx) m_table[hash][cur_idx].right = -1;
              if (m_table[hash][cur_idx].left == sub_idx) m_table[hash][cur_idx].left = -1;
              
              // Move last node to the position of the old subtree leaf node
              if (sub_idx != last_idx) {
                m_table[hash][sub_idx] = m_table[hash][last_idx];
                int parent_idx = m_table[hash][last_idx].parent;
                if (m_table[hash][parent_idx].left == last_idx) m_table[hash][parent_idx].left = sub_idx;
                else m_table[hash][parent_idx].right = sub_idx;
              }
              
            }
          }
          m_table[hash].Resize(last_idx);
          m_size--;
          return true;
        }
        break;
      }
      return false;
    }
    
    
    Iterator Begin() { return Iterator(this); }
    ConstIterator Begin() const { return ConstIterator(this); }
    
    KeyIterator Keys() const { return KeyIterator(this); }
    ValueIterator Values() const { return ValueIterator(this); }
    
    
    
    
  public:
    class Iterator
    {
      friend class HashBTree<K, V, HashFactor, HashFunctor>;
    private:
      HashBTree<K, V, HashFactor, HashFunctor>* m_map;
      int m_table_idx;
      int m_entry_idx;
      Pair<K, V*> m_pair;
      
      Iterator(); // @not_implemented

      Iterator(HashBTree<K, V, HashFactor, HashFunctor>* map) : m_map(map), m_table_idx(-1) { ; }
      
    public:
      const Pair<K, V*>* Next()
      {
        if (m_table_idx == HashFactor) return NULL;
        if (m_table_idx == -1 || ++m_entry_idx >= m_map->m_table[m_table_idx].GetSize()) {
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx].GetSize() == 0);
          if (m_table_idx == HashFactor) return NULL;
          m_entry_idx = 0;
        }
        m_pair.Value1() = m_map->m_table[m_table_idx][m_entry_idx].key;
        m_pair.Value2() = &m_map->m_table[m_table_idx][m_entry_idx].value;
        return &m_pair;
      }
      const Pair<K, V*>* Get()
      {
        if (m_table_idx < HashFactor && m_entry_idx < m_map->m_table[m_table_idx].GetSize()) return &m_pair;
        return NULL;
      }
    };
    
    
    class ConstIterator
    {
      friend class HashBTree<K, V, HashFactor, HashFunctor>;
    private:
      const HashBTree<K, V, HashFactor, HashFunctor>* m_map;
      int m_table_idx;
      int m_entry_idx;
      Pair<K, V*> m_pair;
      
      ConstIterator(); // @not_implemented
      
      ConstIterator(const HashBTree<K, V, HashFactor, HashFunctor>* map) : m_map(map), m_table_idx(-1) { ; }
      
    public:
      const Pair<K, const V*>* Next()
      {
        if (m_table_idx == HashFactor) return NULL;
        if (m_table_idx == -1 || ++m_entry_idx >= m_map->m_table[m_table_idx].GetSize()) {
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx].GetSize() == 0);
          if (m_table_idx == HashFactor) return NULL;
          m_entry_idx = 0;
        }
        m_pair.Value1() = m_map->m_table[m_table_idx][m_entry_idx].key;
        m_pair.Value2() = &m_map->m_table[m_table_idx][m_entry_idx].value;
        return &m_pair;
      }
      const Pair<K, const V*>* Get()
      {
        if (m_table_idx < HashFactor && m_entry_idx < m_map->m_table[m_table_idx].GetSize()) return &m_pair;
        return NULL;
      }
    };
    
    
    class KeyIterator
    {
      friend class HashBTree<K, V, HashFactor, HashFunctor>;
    private:
      const HashBTree<K, V, HashFactor, HashFunctor>* m_map;
      int m_table_idx;
      int m_entry_idx;
      
      KeyIterator(); // @not_implemented
      
      KeyIterator(const HashBTree<K, V, HashFactor, HashFunctor>* map) : m_map(map), m_table_idx(-1) { ; }
      
    public:
      const K* Next()
      {
        if (m_table_idx == HashFactor) return NULL;
        if (m_table_idx == -1 || ++m_entry_idx >= m_map->m_table[m_table_idx].GetSize()) {
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx].GetSize() == 0);
          if (m_table_idx == HashFactor) return NULL;
          m_entry_idx = 0;
        }
        return &m_map->m_table[m_table_idx][m_entry_idx].key;
      }
      const K* Get()
      {
        if (m_table_idx < HashFactor && m_entry_idx < m_map->m_table[m_table_idx].GetSize())
          return &m_map->m_table[m_table_idx][m_entry_idx].key;
        return NULL;
      }
    };
    
    
    class ValueIterator
    {
      friend class HashBTree<K, V, HashFactor, HashFunctor>;
    private:
      const HashBTree<K, V, HashFactor, HashFunctor>* m_map;
      int m_table_idx;
      int m_entry_idx;
      
      ValueIterator(); // @not_implemented
      
      ValueIterator(const HashBTree<K, V, HashFactor, HashFunctor>* map) : m_map(map), m_table_idx(-1) { ; }
      
    public:
      const V* Next()
      {
        if (m_table_idx == HashFactor) return NULL;
        if (m_table_idx == -1 || ++m_entry_idx >= m_map->m_table[m_table_idx].GetSize()) {
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx].GetSize() == 0);
          if (m_table_idx == HashFactor) return NULL;
          m_entry_idx = 0;
        }
        return &m_map->m_table[m_table_idx][m_entry_idx].value;
      }
      const V* Get()
      {
        if (m_table_idx < HashFactor && m_entry_idx < m_map->m_table[m_table_idx].GetSize())
          return &m_map->m_table[m_table_idx][m_entry_idx].value;
        return NULL;
      }
    };
  };
  
  template <class K, class V> class DefaultHashBTree : public HashBTree<K, V, 23> { ; };
  
};

#endif
