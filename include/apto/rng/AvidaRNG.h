/*
 *  AvidaRNG.h
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

#ifndef AptoRNGAvidaRNG_h
#define AptoRNGAvidaRNG_h

#include "apto/core/Random.h"

namespace Apto {
  namespace RNG {
    
    // AvidaRNG - an implementation of the random number generator used by the Avida
    // --------------------------------------------------------------------------------------------------------------
    
    class AvidaRNG : public Random
    {
    private:
      LIB_EXPORT static const unsigned int UPPER_BOUND;
      LIB_EXPORT static const int MAX_SEED;
      
      int m_inext;
      int m_inextp;
      int m_ma[56];

    public:
      LIB_EXPORT inline AvidaRNG(int seed = -1) : Random(UPPER_BOUND, MAX_SEED), m_inext(0), m_inextp(0) { ResetSeed(seed); }
      LIB_EXPORT ~AvidaRNG();
      
    protected:
      LIB_EXPORT void reset();
      LIB_EXPORT unsigned int getNext();
    };
    
  };
};

#endif
