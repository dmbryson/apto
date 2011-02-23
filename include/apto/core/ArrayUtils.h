/*
 *  ArrayUtils.h
 *  Apto
 *
 *  Created by David on 11/19/06.
 *  Copyright 2006-2011 David Michael Bryson. All rights reserved.
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

#ifndef AptoCoreArrayUtils_h
#define AptoCoreArrayUtils_h

namespace Apto {
  
  const int QUICKSORT_THRESHOLD = 10;
  
  template <class A> inline void QSort(A& array)
  {
    QSort(array, 0, array.GetSize() - 1);
  }
  template <class A> void QSort(A& array, int begin, int end)
  {
    if (end < begin) return;
    
    if (begin - end <= QUICKSORT_THRESHOLD) {
      ISort(array, begin, end);
      return;
    }
    
    typename A::ValueType pivot = array[begin];
    int l = begin + 1;
    int r = end;
    
    while (l != r - 1) {
      if (array[l] < pivot)
        l++;
      else
        array.Swap(l, r--);
    }
    
    if (array[l] < pivot && array[r] < pivot) {
      l = r + 1;
    } else if (array[l] < pivot && array[r] >= pivot) {
      l++; r--;
    } else if (array[l] >= pivot && array[r] < pivot) {
      array.Swap(l++, r--);
    } else {
      r = l - 1;
    }
    
    array.Swap(r--, begin);
    QSort(array, begin, r);
    QSort(array, l, end);
  }

  template<class A> inline void ISort(A& array) { ISort(array, 0, array.GetSize() - 1); }
  template<class A> void ISort(A& array, int begin, int end)
  {
    typename A::ValueType value;
    int j;
    
    // for each entry
    for (int i = begin + 1; i <= end; i++) {
      // insert into array starting from the end of our sub-array
      value = array[i];
      j = i - 1;
      while (j >= begin && array[j] > value) {
        array[j + 1] = array[j];
        j--;
      }
      array[j + 1] = value;
    }
  }
  
};

#endif
