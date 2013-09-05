/*
 *  Algorithms.h
 *  Apto
 *
 *  Created by David on 9/28/12.
 *  Copyright 2012 David Michael Bryson. All rights reserved.
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
 *  Authors: David M. Bryson <david@programerror.com>, Aaron P. Wagner <apwagner@msu.edu>
 *
 */

#ifndef AptoCoreAlgorithms_h
#define AptoCoreAlgorithms_h

#include <limits>

namespace Apto {
  
  // Basic Comparison Algorithms
  // --------------------------------------------------------------------------------------------------------------
  
  template <class T> inline const T& Min(const T& v1, const T& v2) { return !(v2 < v1) ? v1 : v2; }
  template <class T> inline const T& Max(const T& v1, const T& v2) { return (v1 < v2) ? v2 : v1; }
  template <class T> inline const T Abs(const T& v1) { return (v1 < 0) ? -v1 : v1; }

  inline unsigned int UAbs(int v)
  {
    if (v == std::numeric_limits<int>::min()) return static_cast<unsigned int>(std::numeric_limits<int>::max()) + 1u;
    return static_cast<unsigned int>((v < 0) ? -v : v);
  }

  template <typename T> inline void Swap(T& op1, T& op2) { const T tmp = op1; op1 = op2; op2 = tmp; }
};


#endif
