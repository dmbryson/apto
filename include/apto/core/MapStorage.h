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
#include "apto/core/Definitions.h"
#include "apto/core/Pair.h"

#include <cstdlib>


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

    
  protected:
    class Iterator;
    class ConstIterator;
    class KeyIterator;
    class ValueIterator;
    
  public:
    static const bool Sorted = false;
    
  protected:
    struct Entry {
      K key;
      V value;
      int left;
      int right;
      int parent;
    };
    
    Array<Entry, ManagedPointer> m_table[HashFactor];
    int m_size;
    
    HashBTree() : m_size(0) { ; }
    ~HashBTree() { ; }
    
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
      Array<Entry, ManagedPointer>& tbl = m_table[hash];
      if (tbl.GetSize() == 0) return false;
      
//      std::cout << "\nRemove Start " << key << " -- " << hash << std::endl;
//      for (int i = 0; i < tbl.GetSize(); i++) {
//        printf("%d -- l: %d  r: %d  p: %d  ", i, tbl[i].left, tbl[i].right, tbl[i].parent);
//        std::cout << tbl[i].key << std::endl;
//      }

      int cur_idx = 0;
      int past_idx = 0;
      while (true) {
        if (key < tbl[cur_idx].key) {
          past_idx = cur_idx;
          cur_idx = tbl[cur_idx].left;
          if (cur_idx >= 0) continue;
        } else if (key > tbl[cur_idx].key) {
          past_idx = cur_idx;
          cur_idx = tbl[cur_idx].right;
          if (cur_idx >= 0) continue;
        } else {
          int last_idx = tbl.GetSize() - 1;
          
          if (cur_idx != last_idx) {
            if (tbl[cur_idx].left < 0 && tbl[cur_idx].right < 0) {
              // handle removing leaf node
              
              // Update parent node subtree index to be blank
              if (tbl[past_idx].left == cur_idx) tbl[past_idx].left = -1;
              else tbl[past_idx].right = -1;
              
              // Move last node to this position in the array
              tbl[cur_idx] = tbl[last_idx];
              int parent_idx = tbl[last_idx].parent;
              if (tbl[parent_idx].left == last_idx) tbl[parent_idx].left = cur_idx;
              else tbl[parent_idx].right = cur_idx;
              
              if (tbl[cur_idx].left >= 0) tbl[tbl[cur_idx].left].parent = cur_idx;
              if (tbl[cur_idx].right >= 0) tbl[tbl[cur_idx].right].parent = cur_idx;
              
            } else if (tbl[cur_idx].left < 0) {
              // all child nodes greater than current
              
              // Find next right node
              int sub_idx = tbl[cur_idx].right;
              while (tbl[sub_idx].left > 0) sub_idx = tbl[sub_idx].left;
              
              // Move next right node to current position
              int right_idx = tbl[cur_idx].right;             // Save right subtree index
              tbl[cur_idx].key = tbl[sub_idx].key;
              tbl[cur_idx].value = tbl[sub_idx].value;
              tbl[tbl[sub_idx].parent].left = tbl[sub_idx].right;
              if (tbl[sub_idx].right >= 0)
                tbl[tbl[sub_idx].right].parent = tbl[sub_idx].parent;
              if (sub_idx != right_idx) tbl[cur_idx].right = right_idx;
              else tbl[cur_idx].right = -1;
              
              // Move last node to the position of the old subtree leaf node
              if (sub_idx != last_idx) {
                tbl[sub_idx] = tbl[last_idx];
                int parent_idx = tbl[last_idx].parent;
                if (tbl[parent_idx].left == last_idx) tbl[parent_idx].left = sub_idx;
                else tbl[parent_idx].right = sub_idx;
                if (tbl[sub_idx].left >= 0) tbl[tbl[sub_idx].left].parent = sub_idx;
                if (tbl[sub_idx].right >= 0) tbl[tbl[sub_idx].right].parent = sub_idx;
              }
            } else if (tbl[cur_idx].right < 0) {
              // all child node less than current
              
              // Find next left node
              int sub_idx = tbl[cur_idx].left;
              while (tbl[sub_idx].right > 0) sub_idx = tbl[sub_idx].right;
              
              // Move extreme right node to current position
              int left_idx = tbl[cur_idx].left;               // Save right subtree index
              tbl[cur_idx].key = tbl[sub_idx].key;
              tbl[cur_idx].value = tbl[sub_idx].value;
              tbl[tbl[sub_idx].parent].right = tbl[sub_idx].left;
              if (tbl[sub_idx].left >= 0)
                tbl[tbl[sub_idx].left].parent = tbl[sub_idx].parent;
              if (sub_idx != left_idx) tbl[cur_idx].left = left_idx;
              else tbl[cur_idx].left = -1;
              
              // Move last node to the position of the old subtree leaf node
              if (sub_idx != last_idx) {
                tbl[sub_idx] = tbl[last_idx];
                int parent_idx = tbl[last_idx].parent;
                if (tbl[parent_idx].left == last_idx) tbl[parent_idx].left = sub_idx;
                else tbl[parent_idx].right = sub_idx;
                if (tbl[sub_idx].left >= 0) tbl[tbl[sub_idx].left].parent = sub_idx;
                if (tbl[sub_idx].right >= 0) tbl[tbl[sub_idx].right].parent = sub_idx;
              }
              
            } else {
              // tossup
              
              int sub_idx = tbl[cur_idx].right;
              while (tbl[sub_idx].left > 0) sub_idx = tbl[sub_idx].left;
              
              // Move extreme right node to current position
              tbl[cur_idx].key = tbl[sub_idx].key;
              tbl[cur_idx].value = tbl[sub_idx].value;
              tbl[tbl[sub_idx].parent].right = tbl[sub_idx].right;
              if (tbl[sub_idx].right >= 0)
                tbl[tbl[sub_idx].right].parent = tbl[sub_idx].parent;
              if (tbl[cur_idx].right == sub_idx) tbl[cur_idx].right = -1;
              if (tbl[cur_idx].left == sub_idx) tbl[cur_idx].left = -1;
              
              // Move last node to the position of the old subtree leaf node
              if (sub_idx != last_idx) {
                tbl[sub_idx] = tbl[last_idx];
                int parent_idx = tbl[last_idx].parent;
                if (tbl[parent_idx].left == last_idx) tbl[parent_idx].left = sub_idx;
                else tbl[parent_idx].right = sub_idx;
                if (tbl[sub_idx].left >= 0) tbl[tbl[sub_idx].left].parent = sub_idx;
                if (tbl[sub_idx].right >= 0) tbl[tbl[sub_idx].right].parent = sub_idx;
              }
              
            }
          } else {
            int parent_idx = tbl[last_idx].parent;
            if (parent_idx >= 0) {
              
              
              if (tbl[last_idx].left < 0 && tbl[last_idx].right < 0) {
                // handle removing leaf node
                if (tbl[parent_idx].left == last_idx) tbl[parent_idx].left = -1;
                else tbl[parent_idx].right = -1;
              } else if (tbl[last_idx].left < 0) {
                // all child nodes greater than current
                
                // Find next right node
                int sub_idx = tbl[last_idx].right;
                while (tbl[sub_idx].left > 0) sub_idx = tbl[sub_idx].left;
                

                // update parent subtree
                if (tbl[parent_idx].left == last_idx) tbl[parent_idx].left = sub_idx;
                else tbl[parent_idx].right = sub_idx;
                
                // Move next right node to current tree position
                if (sub_idx != tbl[last_idx].right && tbl[sub_idx].right >= 0) {
                  tbl[tbl[sub_idx].right].parent = tbl[sub_idx].parent;
                  tbl[tbl[sub_idx].parent].left = tbl[sub_idx].right;
                }
                tbl[sub_idx].parent = parent_idx;
                if (tbl[last_idx].right != sub_idx) {
                  tbl[sub_idx].right = tbl[last_idx].right;
                  if (tbl[sub_idx].right >= 0) tbl[tbl[sub_idx].right].parent = sub_idx;
                }
                tbl[sub_idx].left = tbl[last_idx].left;
                if (tbl[sub_idx].left >= 0) tbl[tbl[sub_idx].left].parent = sub_idx;
                
              } else if (tbl[last_idx].right < 0) {
                // all child node less than current
                
                // Find next left node
                int sub_idx = tbl[last_idx].left;
                while (tbl[sub_idx].right > 0) sub_idx = tbl[sub_idx].right;
                
                
                // update parent subtree
                if (tbl[parent_idx].left == last_idx) tbl[parent_idx].left = sub_idx;
                else tbl[parent_idx].right = sub_idx;
                
                // Move next right node to current tree position
                if (sub_idx != tbl[last_idx].left && tbl[sub_idx].left >= 0) {
                  tbl[tbl[sub_idx].left].parent = tbl[sub_idx].parent;
                  tbl[tbl[sub_idx].parent].right = tbl[sub_idx].left;
                }
                tbl[sub_idx].parent = parent_idx;
                if (tbl[last_idx].left != sub_idx) {
                  tbl[sub_idx].left = tbl[last_idx].left;
                  if (tbl[sub_idx].left >= 0) tbl[tbl[sub_idx].left].parent = sub_idx;
                }
                tbl[sub_idx].right = tbl[last_idx].right;
                if (tbl[sub_idx].right >= 0) tbl[tbl[sub_idx].right].parent = sub_idx;
               
              } else {
                // tossup
                
                // Find next left node
                int sub_idx = tbl[last_idx].left;
                while (tbl[sub_idx].right > 0) sub_idx = tbl[sub_idx].right;
                
                
                // update parent subtree
                if (tbl[parent_idx].left == last_idx) tbl[parent_idx].left = sub_idx;
                else tbl[parent_idx].right = sub_idx;
                
                // Move next right node to current tree position
                if (sub_idx != tbl[last_idx].left && tbl[sub_idx].left >= 0) {
                  tbl[tbl[sub_idx].left].parent = tbl[sub_idx].parent;
                  tbl[tbl[sub_idx].parent].right = tbl[sub_idx].left;
                }
                tbl[sub_idx].parent = parent_idx;
                if (tbl[last_idx].left != sub_idx) {
                  tbl[sub_idx].left = tbl[last_idx].left;
                  if (tbl[sub_idx].left >= 0) tbl[tbl[sub_idx].left].parent = sub_idx;
                }
                tbl[sub_idx].right = tbl[last_idx].right;
                if (tbl[sub_idx].right >= 0) tbl[tbl[sub_idx].right].parent = sub_idx;
              }
              
            }
          }
          
          tbl.Resize(last_idx);
          m_size--;
          
//          std::cout << "\nRemove " << key << " -- " << hash << std::endl;
//          for (int i = 0; i < tbl.GetSize(); i++) {
//            printf("%d -- l: %d  r: %d  p: %d  ", i, tbl[i].left, tbl[i].right, tbl[i].parent);
//            std::cout << tbl[i].key << std::endl;
//          }          
          
          return true;
        }
        break;
      }
      return false;
    }
    
    
    Iterator Begin() { return Iterator(this); }
    ConstIterator Begin() const { return ConstIterator(this); }
    
    KeyIterator Keys() const { return KeyIterator(this); }
    ValueIterator Values() { return ValueIterator(this); }
    
    
  protected:
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
      Pair<K, V*>* Next()
      {
        if (m_table_idx == HashFactor) return NULL;
        if (m_table_idx == -1 || ++m_entry_idx >= m_map->m_table[m_table_idx].GetSize()) {
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx].GetSize() == 0) ;
          if (m_table_idx == HashFactor) return NULL;
          m_entry_idx = 0;
        }
        m_pair.Value1() = m_map->m_table[m_table_idx][m_entry_idx].key;
        m_pair.Value2() = &m_map->m_table[m_table_idx][m_entry_idx].value;
        return &m_pair;
      }
      Pair<K, V*>* Get()
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
      Pair<K, const V*> m_pair;
      
      ConstIterator(); // @not_implemented
      
      ConstIterator(const HashBTree<K, V, HashFactor, HashFunctor>* map) : m_map(map), m_table_idx(-1) { ; }
      
    public:
      const Pair<K, const V*>* Next()
      {
        if (m_table_idx == HashFactor) return NULL;
        if (m_table_idx == -1 || ++m_entry_idx >= m_map->m_table[m_table_idx].GetSize()) {
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx].GetSize() == 0) ;
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
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx].GetSize() == 0) ;
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
      HashBTree<K, V, HashFactor, HashFunctor>* m_map;
      int m_table_idx;
      int m_entry_idx;
      
      ValueIterator(); // @not_implemented
      
      ValueIterator(HashBTree<K, V, HashFactor, HashFunctor>* map) : m_map(map), m_table_idx(-1) { ; }
      
    public:
      V* Next()
      {
        if (m_table_idx == HashFactor) return NULL;
        if (m_table_idx == -1 || ++m_entry_idx >= m_map->m_table[m_table_idx].GetSize()) {
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx].GetSize() == 0) ;
          if (m_table_idx == HashFactor) return NULL;
          m_entry_idx = 0;
        }
        return &m_map->m_table[m_table_idx][m_entry_idx].value;
      }
      V* Get()
      {
        if (m_table_idx < HashFactor && m_entry_idx < m_map->m_table[m_table_idx].GetSize())
          return &m_map->m_table[m_table_idx][m_entry_idx].value;
        return NULL;
      }
    };
  };
  
  template <class K, class V> class DefaultHashBTree : public HashBTree<K, V, 23> { ; };
      
      
      
  template <class K, class V, int HashFactor, template <class, int> class HashFunctor = HashKey, class Allocator = BasicMalloc>
  class HashStaticTableLinkedList
  {
  protected:
    typedef HashFunctor<K, HashFactor> HF;

    
  protected:
    class Iterator;
    class ConstIterator;
    class KeyIterator;
    class ValueIterator;
    
  public:
    static const bool Sorted = false;
    
  protected:
    struct Entry : public ClassAllocator<Allocator> {
      K key;
      V value;
      Entry* prev;
      Entry* next;
      
      inline ~Entry() { ; }
    };
    
    Entry* m_table[HashFactor];
    int m_size;
    
    inline HashStaticTableLinkedList() : m_size(0)
    {
      for (int i = 0; i < HashFactor; i++) m_table[i] = NULL;
    }
    inline ~HashStaticTableLinkedList() { Clear(); }
    
    
    inline int GetSize() const { return m_size; }
    
    void Clear()
    {
      if (m_size) {
        for (int i = 0; i < HashFactor; i++) {
          if (m_table[i]) {
            Entry* cur_entry = m_table[i];
            Entry* next_entry;
            while (cur_entry) {
              next_entry = cur_entry->next;
              delete cur_entry;
              cur_entry = next_entry;
            }
            m_table[i] = NULL;
          }
        }
      }
      m_size = 0;
    }
    
    const V* Find(const K& key) const
    {
      int hash = HF::Hash(key);
      Entry* cur_entry = m_table[hash];
      while (cur_entry) {
        if (key == cur_entry->key) return &cur_entry->value;
        cur_entry = cur_entry->next;
      }
      return NULL;
    }

    V* Find(const K& key)
    {
      int hash = HF::Hash(key);
      Entry* cur_entry = m_table[hash];
      while (cur_entry) {
        if (key == cur_entry->key) return &cur_entry->value;
        cur_entry = cur_entry->next;
      }
      return NULL;
    }
    
    
    V& Get(const K& key)
    {
      int hash = HF::Hash(key);
      Entry* cur_entry = m_table[hash];
      if (cur_entry == NULL) {
        // Create root node
        cur_entry = new Entry;
        m_table[hash] = cur_entry;
        cur_entry->key = key;
        cur_entry->next = NULL;
        cur_entry->prev = NULL;
        m_size++;
        
        return cur_entry->value;
      }
      
      // Search the list for the key
      Entry* last_entry = NULL;
      while (cur_entry) {
        if (key == cur_entry->key) return cur_entry->value;
        last_entry = cur_entry;
        cur_entry = cur_entry->next;
      }
      
      // Add new entry into the list
      cur_entry = new Entry;
      last_entry->next = cur_entry;
      cur_entry->key = key;
      cur_entry->next = NULL;
      cur_entry->prev = last_entry;
      m_size++;
      
      return cur_entry->value;
    }
    
    
    bool Remove(const K& key)
    {
      int hash = HF::Hash(key);
      Entry* cur_entry = m_table[hash];
      if (cur_entry == NULL) return false;
      
      // Search the tree for the key
      while (cur_entry) {
        if (key == cur_entry->key) {
          if (cur_entry->prev) {
            cur_entry->prev->next = cur_entry->next;
            cur_entry->next->prev = cur_entry->prev;
          } else {
            m_table[hash] = cur_entry->next;
            cur_entry->next->prev = NULL;
          }
          m_size--;
      
          return true;
        }
        cur_entry = cur_entry->next;
      }

      return false;
    }
    
    
    Iterator Begin() { return Iterator(this); }
    ConstIterator Begin() const { return ConstIterator(this); }
    
    KeyIterator Keys() const { return KeyIterator(this); }
    ValueIterator Values() { return ValueIterator(this); }
    
    
  protected:
    class Iterator
    {
      friend class HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>;
    private:
      HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>* m_map;
      int m_table_idx;
      Entry* m_cur_entry;
      Pair<K, V*> m_pair;
      
      Iterator(); // @not_implemented

      Iterator(HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>* map) : m_map(map), m_table_idx(-1), m_cur_entry(NULL) { ; }
      
    public:
      Pair<K, V*>* Next()
      {
        if (m_table_idx == HashFactor) return NULL;
        if (m_table_idx == -1 || (m_cur_entry && m_cur_entry->next == NULL)) {
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx] == NULL) ;
          if (m_table_idx == HashFactor) return NULL;
          m_cur_entry = m_map->m_table[m_table_idx];
        }
        m_pair.Value1() = m_cur_entry->key;
        m_pair.Value2() = &m_cur_entry->value;
        return &m_pair;
      }
      Pair<K, V*>* Get()
      {
        if (m_table_idx < HashFactor && m_cur_entry) return &m_pair;
        return NULL;
      }
    };
    
    
    class ConstIterator
    {
      friend class HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>;
    private:
      const HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>* m_map;
      int m_table_idx;
      Entry* m_cur_entry;
      Pair<K, const V*> m_pair;
      
      ConstIterator(); // @not_implemented
      
      ConstIterator(const HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>* map) : m_map(map), m_table_idx(-1), m_cur_entry(NULL) { ; }
      
    public:
      const Pair<K, const V*>* Next()
      {
        if (m_table_idx == HashFactor) return NULL;
        if (m_table_idx == -1 || (m_cur_entry && m_cur_entry->next == NULL)) {
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx] == NULL) ;
          if (m_table_idx == HashFactor) return NULL;
          m_cur_entry = m_map->m_table[m_table_idx];
        }
        m_pair.Value1() = m_cur_entry->key;
        m_pair.Value2() = &m_cur_entry->value;
        return &m_pair;
      }
      const Pair<K, const V*>* Get()
      {
        if (m_table_idx < HashFactor && m_cur_entry) return &m_pair;
        return NULL;
      }
    };
    
    
    class KeyIterator
    {
      friend class HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>;
    private:
      const HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>* m_map;
      int m_table_idx;
      Entry* m_cur_entry;
      
      KeyIterator(); // @not_implemented
      
      KeyIterator(const HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>* map) : m_map(map), m_table_idx(-1), m_cur_entry(NULL) { ; }
      
    public:
      const K* Next()
      {
        if (m_table_idx == HashFactor) return NULL;
        if (m_table_idx == -1 || (m_cur_entry && m_cur_entry->next == NULL)) {
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx] == NULL) ;
          if (m_table_idx == HashFactor) return NULL;
          m_cur_entry = m_map->m_table[m_table_idx];
        }
        return &m_cur_entry->key;
      }
      const K* Get()
      {
        if (m_table_idx < HashFactor && m_cur_entry) return &m_cur_entry->key;
        return NULL;
      }
    };
    
    
    class ValueIterator
    {
      friend class HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>;
    private:
      HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>* m_map;
      int m_table_idx;
      Entry* m_cur_entry;
      
      ValueIterator(); // @not_implemented
      
      ValueIterator(HashStaticTableLinkedList<K, V, HashFactor, HashFunctor, Allocator>* map) : m_map(map), m_table_idx(-1), m_cur_entry(NULL) { ; }
      
    public:
      V* Next()
      {
        if (m_table_idx == HashFactor) return NULL;
        if (m_table_idx == -1 || (m_cur_entry && m_cur_entry->next == NULL)) {
          while (++m_table_idx < HashFactor && m_map->m_table[m_table_idx] == NULL) ;
          if (m_table_idx == HashFactor) return NULL;
          m_cur_entry = m_map->m_table[m_table_idx];
        }
        return &m_cur_entry->value;
      }
      V* Get()
      {
        if (m_table_idx < HashFactor && m_cur_entry) return &m_cur_entry->value;
        return NULL;
      }
    };
  };
  
  

  // Map ConstAccess Policies
  // --------------------------------------------------------------------------------------------------------------
  
  template <class ValueType> class NoDefault
  {
  };
  

  template <class ValueType> class ExplicitDefault
  {
  protected:
    inline void AllowNonUpdatingExplicitDefault() const { ; }
  };
  
  
  template <class ValueType> class ImplicitDefault
  {
  protected:
    ValueType m_default;
    
    inline void AllowNonUpdatingExplicitDefault() const { ; }
    
  public:
    // Accessors for configuring the default value
    void SetDefaultValue(const ValueType& new_default) { m_default = new_default; }
    ValueType& DefaultValue() { return m_default; }
    const ValueType& DefaultValue() const { return m_default; }
  };
  
};

#endif
