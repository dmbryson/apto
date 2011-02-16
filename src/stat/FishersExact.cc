/*
 *  FishersExact.cc
 *  Apto
 *
 *  Created by David on 2/15/11.
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

#include "apto/stat/ContingencyTable.h"
#include "apto/stat/Functions.h"

#include "apto/core/Array.h"
#include "apto/core/Pair.h"

#include <cmath>
#include <limits>


static struct PastPathLength {
  double value;
  int observed;
  
  PastPathLength(double in_value) : value(in_value), observed(1) { ; }
};

static struct FExactNode
{
  double shortest_path;
  double longest_path;
  Apto::Array<PastPathLength, Apto::Smart> past_entries;  
};

static double cummulativeGamma(double q, double alpha, bool& fault);
static double logGamma(double x, bool& fault);

double Apto::Stat::FishersExact(const ContingencyTable& table)
{
  if (table.MarginalTotal() == 0.0) return std::numeric_limits<double>::quiet_NaN();  // All elements are 0
  
  
  return 0.0;
}



double cummulativeGamma(double q, double alpha, bool& fault)
{
  if (q <= 0.0 || alpha <= 0.0) {
    fault = true;
    return 0.0;
  }
  
  double f = exp(alpha * log(q) - logGamma(alpha + 1.0, fault) - q); // no need to test logGamma fail as an error is impossible
  if (f == 0.0) {
    fault = true;
    return 0.0;
  }
  
  fault = false;
  
  double c = 1.0;
  double ret_val = 1.0;
  double a = alpha;

  do {
    a += 1.0;
    c = c * q / a;
    ret_val += c;
  } while (c / ret_val > (1e-6));
  ret_val *= f;
  
  return ret_val;
}

double logGamma(double x, bool& fault)
{
  const double a1 = .918938533204673;
  const double a2 = 5.95238095238e-4;
  const double a3 = 7.93650793651e-4;
  const double a4 = .002777777777778;
  const double a5 = .083333333333333;
  
  if (x < 0.0) {
    fault = true;
    return 0.0;
  }
  
  fault = false;
  
  double f = 0.0;
  
  if (x < 7.0) {
    f = x;
    
    x += 1.0;
    while (x < 7.0) {
      f *= x;
      x += 1.0;
    }

    f = -log(f);
  }
  
  double z = 1 / (x * x);
  return f + (x - .5) * log(x) - x + a1 + (((-a2 * z + a3) * z - a4) * z + a5) / x;
}
