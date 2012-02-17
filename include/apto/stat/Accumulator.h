/*
 *  Accumulator.h
 *  Apto
 *
 *  Created by David on 2/16/12.
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
 *  Authors: David M. Bryson <david@programerror.com>
 *
 */

#ifndef AptoStatAccumulator_h
#define AptoStatAccumulator_h

#include "apto/stat/AccumulatorStats.h"

namespace Apto {
  namespace Stat {
    
    // AccumulatorTypes - helper struct for passing type parameters used by AccumulatorStats
    // --------------------------------------------------------------------------------------------------------------
    
    template <typename T, typename D> struct AccumulatorTypes
    {
      typedef T ValueType;
      typedef D FloatType;
    };
    
    typedef TL::Create<AccumulatorStats::Count, AccumulatorStats::Min, AccumulatorStats::Max, AccumulatorStats::Mean,
                       AccumulatorStats::Variance, AccumulatorStats::StdError>::Type DefaultAccumulatorStats;
    

    // Accumulator
    // --------------------------------------------------------------------------------------------------------------
    
    template <typename T, typename TList = DefaultAccumulatorStats, typename D = double> class Accumulator
      : public AccumulatorStats::DependsOn<AccumulatorTypes<T, D>, typename TL::SortDerived<TList>::Result, AccumulatorStats::Internal::StatImpl>
    {
    protected:
      typedef typename AccumulatorStats::DependsOn<AccumulatorTypes<T, D>,
                                                   TList,
                                                   AccumulatorStats::Internal::StatImpl>::SubClass SubClass;
      
    public:
      inline Accumulator() { Clear(); }
      
      inline void Clear() { this->SubClass::clear(); }
      
      void Add(T value) { this->SubClass::addValue(value); }
    };
    
  };
};

#endif
