/*
 *  Random.cc
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

#include "apto/core/Random.h"

#include "apto/platform.h"

#include <ctime>
#include <limits>
#if APTO_PLATFORM(WINDOWS)
# include <process.h>
#else
# include <unistd.h>
#endif


// Statistical Approximation
static const unsigned int BINOMIAL_TO_NORMAL = 50;    //if < n*p*(1-p)
static const unsigned int BINOMIAL_TO_POISSON = 1000; //if < n

Apto::Random::~Random() { ; }


double Apto::Random::GetRandNormal()
{
  // Draw from a Unit Normal Dist, using rejection method and saving initial exponential random variable
  double expRV2;
  while (1) {
    expRV2 = -log(GetDouble());
    m_rand_norm_exprv -= (expRV2 - 1) * (expRV2 - 1) / 2;
    if (m_rand_norm_exprv > 0) break;
    m_rand_norm_exprv = -log(GetDouble());
  }
  if (P(.5))
    return expRV2;
  else
    return -expRV2;
}


unsigned int Apto::Random::GetRandPoisson(const double mean)
{
  // Draw from a Poisson Dist with mean, if cannot calculate, returns max
  // Uses Rejection Method
  unsigned int k = 0;
  double a = exp(-mean);
  double u = GetDouble();
  if (a <= 0) return std::numeric_limits<unsigned int>::max(); // cannot calculate, so return UINT_MAX
  while (u >= a) {
    u *= GetDouble();
    ++k;
  }
  return k;
}

unsigned int Apto::Random::GetRandBinomial(const double n, const double p)
{
  // Approximate Binomial if appropriate
  if (n * p * (1 - p) >= BINOMIAL_TO_NORMAL) {
    // if np(1-p) is large, use a Normal approx
    return static_cast<unsigned int>(GetRandNormal(n * p, n * p * (1 - p)) + 0.5);
  }
  
  if (n >= BINOMIAL_TO_POISSON) {
    // elseif n is large, use a Poisson approx
    unsigned int k = GetRandPoisson(n, p);
    if (k < std::numeric_limits<unsigned int>::max()) // if approx worked
      return k;
  }

  // otherwise, actually generate the randBinomial
  return GetFullRandBinomial(n, p);
}


int Apto::Random::getRandomSeed()
{
  int seed_time = static_cast<int>(time(NULL));
#if APTO_PLATFORM(WINDOWS)
  int seed_pid = static_cast<int>(_getpid());
#else
  int seed_pid = static_cast<int>(getpid());
#endif
  return seed_time ^ (seed_pid << 8);
}
