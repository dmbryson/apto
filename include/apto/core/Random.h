/*
 *  Random.h
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

#ifndef AptoCoreRandom_h
#define AptoCoreRandom_h

#include "apto/platform/Visibility.h"

#include <cassert>
#include <limits>
#include <cmath>


namespace Apto {
  
  // Random - base class for random number generation
  // --------------------------------------------------------------------------------------------------------------
  
  class Random
  {
  public:
    // STL Conformance
    typedef unsigned int argument_type;
    typedef unsigned int result_type;
    
  private:
    unsigned int m_ubound;
    int m_max_seed;
    
    int m_orig_seed;
    int m_seed;
    double m_factor;
    
    double m_rand_norm_exprv;
    
  public:
    LIB_EXPORT virtual ~Random() = 0;
    
    inline unsigned int UpperBound() const { return m_ubound; }
    inline int MaxSeed() const { return m_max_seed; }

    
    inline int Seed() const { return m_seed; }
    inline int OriginalSeed() const { return m_orig_seed; }

    inline void ResetSeed(int new_seed);
    
    inline double GetDouble() { return getNext() * m_factor; }
    inline double GetDouble(double max) { return GetDouble() * max; }
    inline double GetDouble(double min, double max) { return GetDouble() * (max - min) + min; }
    
    inline unsigned int GetUInt() { return static_cast<unsigned int>(std::numeric_limits<int>::max()); }
    inline unsigned int GetUInt(unsigned int max) { return static_cast<unsigned int>(GetDouble(max)); }
    inline unsigned int GetUInt(unsigned int min, unsigned int max) { return static_cast<unsigned int>(GetDouble(min, max)); }
    
    inline unsigned int operator()(unsigned int max) { return GetUInt(max); }
    
    inline int GetInt() { return static_cast<int>(GetDouble(std::numeric_limits<int>::min(), std::numeric_limits<int>::max())); }
    inline int GetInt(int max) { return static_cast<int>(GetDouble(max)); }
    inline int GetInt(int min, int max) { return static_cast<int>(floor(GetDouble(min, max))); }
    
    inline bool P(double p) { return (getNext() < (p * m_ubound)); }
    
    LIB_EXPORT double GetRandNormal();
    inline double GetRandNormal(double mean, double variance) { return mean + GetRandNormal() * sqrt(variance); }
    
    LIB_EXPORT unsigned int GetRandPoisson(double mean);
    inline unsigned int GetRandPoisson(double n, double p);
    
    inline unsigned int GetFullRandBinomial(double n, double p);
    LIB_EXPORT unsigned int GetRandBinomial(double n, double p);
    
    template <class A> void Choose(int num_in, A& out_array);
  
    
  protected:
    inline Random(unsigned int ubound, int max_seed) : m_ubound(ubound), m_max_seed(max_seed), m_factor(1.0 / ubound) { ; }
  
  protected:
    // Internal implementation should reset based on current random seed
    LIB_EXPORT virtual void reset() = 0;
    
    // Basic random number generation, should return a number [0, UpperBound)
    LIB_EXPORT virtual unsigned int getNext() = 0;
    
    LIB_EXPORT virtual int getRandomSeed();
  };
  
  
  
  inline void Random::ResetSeed(int new_seed)
  {
    m_orig_seed = new_seed;
    if (new_seed < 0) new_seed = getRandomSeed();
    m_seed = new_seed;
    if (m_seed > m_max_seed) m_seed %= m_max_seed;
    
    this->reset();
    
    m_rand_norm_exprv = -log(GetDouble());
  }
  
  
  inline unsigned int Random::GetRandPoisson(double n, double p)
  {
    if (p > 0.5) return static_cast<unsigned int>(n) - GetRandPoisson(n * (1 - p));
    
    return GetRandPoisson(n * p);
  }

  
  inline unsigned int Random::GetFullRandBinomial(double n, double p)
  {
    unsigned int k = 0;
    for (unsigned int i = 0; i < n; ++i) if (P(p)) k++;
    return k;
  }
  
  
  template <class A>
  void Random::Choose(int num_in, A& out_array)
  {
    assert(num_in > out_array.GetSize());
    
    if (num_in == out_array.GetSize()) {
      for (int i = 0; i < out_array.GetSize(); i++) out_array[i] = i;
      return;
    }
    
    int choice_count = 0;
    while (choice_count < out_array.GetSize()) {
      int next = GetInt(num_in);
      
      bool ok = true;
      for (int i = 0; i < choice_count; i++) {
        if (out_array[i] == next) {
          ok = false;
          break;
        }
      }
      
      if (ok) {
        out_array[choice_count] = next;
        choice_count++;
      }
    }
  }

};

#endif
