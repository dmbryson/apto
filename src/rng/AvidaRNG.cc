/*
 *  AvidaRNG.cc
 *  Apto
 *
 *  Created by David on 10/24/12.
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
 *  Authors: David M. Bryson <david@programerror.com>, Charles Ofria <ofria@msu.edu>
 *
 */

#include "apto/rng/AvidaRNG.h"


const unsigned int Apto::RNG::AvidaRNG::UPPER_BOUND = 1000000000;
const int Apto::RNG::AvidaRNG::MAX_SEED = 161803398;

Apto::RNG::AvidaRNG::~AvidaRNG() { ; }


void Apto::RNG::AvidaRNG::reset()
{
  int mj, mk;
  
  // Clear variables
  m_inext = 0;
  m_inextp = 0;
  for (int i = 0; i < 56; i++) m_ma[i] = 0;
  
  mj = MAX_SEED - Seed();
  mj %= UPPER_BOUND;
  m_ma[55] = mj;
  mk = 1;
  
  for (int i = 1; i < 55; i++) {
    int j = (21 * i) % 55;
    m_ma[j] = mk;
    mk = mj - mk;
    if (mk < 0) mk += UPPER_BOUND;
    mj = m_ma[j];
  }
  
  for (int k = 0; k < 4; ++k) {
    for (int j = 1; j < 55; ++j) {
      m_ma[j] -= m_ma[1 + (j + 30) % 55];
      if (m_ma[j] < 0) m_ma[j] += UPPER_BOUND;
    }
  }
  
  m_inext = 0;
  m_inextp = 31;
}


unsigned int Apto::RNG::AvidaRNG::getNext()
{
  if (++m_inext == 56) m_inext = 0;
  if (++m_inextp == 56) m_inextp = 0;
  assert(m_inext < 56);
  assert(m_inextp < 56);
  int mj = m_ma[m_inext] - m_ma[m_inextp];
  if (mj < 0) mj += UPPER_BOUND;
  m_ma[m_inext] = mj;
  return mj;
}
