/*
 *  AccumulatorStats.h
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

#ifndef AptoStatAccumulatorStats_h
#define AptoStatAccumulatorStats_h

#include "apto/core/TypeList.h"

#include <cstdio>
#include <cmath>
#include <cstring>
#include <limits>


namespace Apto {
  namespace Stat {
    namespace AccumulatorStats {

      // Stats Available
      // --------------------------------------------------------------------------------------------------------------
      
      class Count { ; };
      class Max { ; };
      class Mean { ; };
      class Min { ; };
      template <int N> class Moment { enum { Value = N }; };
      class StdError { ; };
      class Sum { ; };
      class Variance { ; };

      
      // DependsOn - AccumulatorStats variant of LinearHierarchy. Passes AccumulatorTypes and defines SubClass
      // --------------------------------------------------------------------------------------------------------------
      
      namespace Internal { template <class AccTypes> class StatImplRoot; };
      
      template <class AccTypes, class TList, template <class _AccTypes, class StatType, class Base> class Unit,
                class Root = Internal::StatImplRoot<AccTypes> >
      class DependsOn;
      
      template <class AccTypes, class T1, class T2, template <class, class, class> class Unit, class Root>
      class DependsOn< AccTypes, TypeList<T1, T2>, Unit, Root >
      : public TypeSelect<TL::IndexOf<typename DependsOn<AccTypes, T2, Unit, Root>::SubClass::Features, T1>::Value == -1,
                          Unit<AccTypes, T1, typename DependsOn<AccTypes, T2, Unit, Root>::SubClass>,
                          typename DependsOn<AccTypes, T2, Unit, Root>::SubClass>::Result
      {
      public:
        typedef typename TypeSelect<TL::IndexOf<typename DependsOn<AccTypes, T2, Unit, Root>::SubClass::Features, T1>::Value == -1,
                                    Unit<AccTypes, T1, typename DependsOn<AccTypes, T2, Unit, Root>::SubClass>,
                                    typename DependsOn<AccTypes, T2, Unit, Root>::SubClass>::Result SubClass;
      };

      template <class AccTypes, class T, template <class, class, class> class Unit, class Root>
      class DependsOn<AccTypes, TypeList<T, NullType>, Unit, Root>
      : public TypeSelect<TL::IndexOf<typename Root::Features, T>::Value == -1, Unit<AccTypes, T, Root>, Root >::Result
      {
      public:
        typedef typename TypeSelect<TL::IndexOf<typename Root::Features, T>::Value == -1,
                                                Unit<AccTypes, T, Root>,
                                                Root >::Result SubClass;
      };
      
      template <class AccTypes, template <class, class, class> class Unit, class Root>
      class DependsOn<AccTypes, NullType, Unit, Root> : public Root
      {
      protected:
        typedef NullType SubClass;
      };
      
      
      
      // Internal
      // --------------------------------------------------------------------------------------------------------------
      
      namespace Internal {
      
        template <class AccTypes>
        class StatImplRoot
        {
        public:
          typedef TypeList<NullType, NullType> Features;
          typedef typename AccTypes::ValueType ValueType;
          
        protected:
          inline void clear() { ; }
          inline void addValue(ValueType value) { (void)value; }
          
        public:
          inline void Moment() { ; }
        };


        // StatImpl - base class for AccumulatorStats implementations
        // --------------------------------------------------------------------------------------------------------------
        
        template <class AccTypes, class F, class Base> class StatImpl;
        
        
        // StatImpl<Count>
        // --------------------------------------------------------------------------------------------------------------
        
        template <class AccTypes, class Base> class StatImpl<AccTypes, typename ::Apto::Stat::AccumulatorStats::Count, Base>
          : public Base
        {
        private:
          std::size_t m_n;
          
        public:
          typedef Base SubClass;
          typedef TypeList< ::Apto::Stat::AccumulatorStats::Count, typename SubClass::Features> Features;
          typedef typename AccTypes::ValueType ValueType;
          
        protected:
          inline void clear() { SubClass::clear(); m_n = 0; }
          inline void addValue(ValueType value) { SubClass::addValue(value); m_n++; }
          
        public:
          std::size_t Count() const { return m_n; }
        };
        
        
        // StatImpl<Max>
        // --------------------------------------------------------------------------------------------------------------
        
        template <class AccTypes, class Base> class StatImpl<AccTypes, ::Apto::Stat::AccumulatorStats::Max, Base>
          : public Base
        {
        public:
          typedef Base SubClass;
          typedef TypeList< ::Apto::Stat::AccumulatorStats::Max, typename SubClass::Features> Features;
          typedef typename AccTypes::ValueType ValueType;
          
        private:
          ValueType m_max;
          
        protected:
          inline void clear() { SubClass::clear(); m_max = std::numeric_limits<ValueType>::min(); }
          inline void addValue(ValueType value) { SubClass::addValue(value); if (value > m_max) m_max = value; }
          
        public:
          inline const ValueType& Max() const { return m_max; }
        };
        
        
        // StatImpl<Min>
        // --------------------------------------------------------------------------------------------------------------
        
        template <class AccTypes, class Base> class StatImpl<AccTypes, ::Apto::Stat::AccumulatorStats::Min, Base>
          : public Base
        {
        public:
          typedef Base SubClass;
          typedef TypeList< ::Apto::Stat::AccumulatorStats::Min, typename SubClass::Features> Features;
          typedef typename AccTypes::ValueType ValueType;
          
        private:
          ValueType m_min;
          
        protected:
          inline void clear() { SubClass::clear(); m_min = std::numeric_limits<ValueType>::max(); }
          inline void addValue(ValueType value) { SubClass::addValue(value); if (value < m_min) m_min = value; }
          
        public:
          inline const ValueType& Min() const { return m_min; }
        };
        
        
        // StatImpl<Moment<N> >
        // --------------------------------------------------------------------------------------------------------------
        
        template <class AccTypes, typename Base, int N> class StatImpl<AccTypes, ::Apto::Stat::AccumulatorStats::Moment<N>, Base>
          : public DependsOn<AccTypes,
                             typename TL::SortDerived<TL::Create< ::Apto::Stat::AccumulatorStats::Count>::Type>::Result,
                             ::Apto::Stat::AccumulatorStats::Internal::StatImpl, Base>::SubClass
        {
        public:
          typedef typename DependsOn<AccTypes,
                                     typename TL::SortDerived<TL::Create< ::Apto::Stat::AccumulatorStats::Count>::Type>::Result,
                                     ::Apto::Stat::AccumulatorStats::Internal::StatImpl, Base>::SubClass SubClass;
          typedef TypeList< ::Apto::Stat::AccumulatorStats::Moment<N>, typename SubClass::Features> Features;
          typedef typename AccTypes::ValueType ValueType;
          typedef typename AccTypes::FloatType FloatType;
          
        private:
          ValueType m_s;
          
          template <typename T> const T& pow(const T& x, Type::Int<1>) { return x; }
          template <typename T, int E> T pow(const T& x, Type::Int<E>)
          {
            T y = pow(x, Type::Int<E / 2>());
            T z = y * y;
            return (E % 2) ? (z * x) : z;
          }
          
        protected:
          inline void clear() { SubClass::clear(); m_s = 0; }
          inline void addValue(ValueType value) { SubClass::addValue(value); m_s += pow(value, Type::Int<N>()); }
          
        public:
          using SubClass::Moment;
          inline FloatType Moment(Type::Int<N>) const { return static_cast<FloatType>(m_s) / this->Count(); }
        };
        
        
        // StatImpl<Sum>
        // --------------------------------------------------------------------------------------------------------------
        
        template <class AccTypes, typename Base> class StatImpl<AccTypes, ::Apto::Stat::AccumulatorStats::Sum, Base>
          : public Base
        {
        public:
          typedef Base SubClass;
          typedef TypeList< ::Apto::Stat::AccumulatorStats::Sum, typename SubClass::Features> Features;
          typedef typename AccTypes::ValueType ValueType;
          
        private:
          ValueType m_s;
          
        protected:
          inline void clear() { SubClass::clear(); m_s = 0; }
          inline void addValue(ValueType value) { SubClass::addValue(value); m_s += value; }
          
        public:
          inline const ValueType& Sum() const { return m_s; }
        };
        
        
       
        // StatImpl<Mean>
        // --------------------------------------------------------------------------------------------------------------
        
        template <class AccTypes, typename Base> class StatImpl<AccTypes, Mean, Base>
          : public DependsOn<AccTypes,
                             TL::Create< ::Apto::Stat::AccumulatorStats::Count, ::Apto::Stat::AccumulatorStats::Sum>::Type,
                             ::Apto::Stat::AccumulatorStats::Internal::StatImpl, Base>::SubClass
        {
        public:
          typedef typename DependsOn<AccTypes,
                                     typename TL::SortDerived<TL::Create< ::Apto::Stat::AccumulatorStats::Count, ::Apto::Stat::AccumulatorStats::Sum>::Type>::Result,
                                     ::Apto::Stat::AccumulatorStats::Internal::StatImpl, Base>::SubClass SubClass;
          typedef TypeList< ::Apto::Stat::AccumulatorStats::Mean, typename SubClass::Features> Features;
          typedef typename AccTypes::ValueType ValueType;
          typedef typename AccTypes::FloatType FloatType;
          
        protected:
          inline void clear() { SubClass::clear(); }
          inline void addValue(ValueType value) { SubClass::addValue(value); }
          
        public:
          inline FloatType Mean() const
          {
            return (this->Count() > 0) ? (static_cast<FloatType>(this->Sum()) / this->Count()) :
                                                                std::numeric_limits<FloatType>::quiet_NaN();
          }
        };

      
        // StatImpl<Variance>
        // --------------------------------------------------------------------------------------------------------------
        // 
        //  Lazy calculation of sample variance based on the second moment, mean
        //
        
        template <class AccTypes, typename Base> class StatImpl<AccTypes, Variance, Base>
          : public DependsOn<AccTypes,
                             TL::Create< ::Apto::Stat::AccumulatorStats::Moment<2>, ::Apto::Stat::AccumulatorStats::Mean>::Type,
                             ::Apto::Stat::AccumulatorStats::Internal::StatImpl, Base>::SubClass
        {
        public:
          typedef typename DependsOn<AccTypes,
                                     typename TL::SortDerived<TL::Create< ::Apto::Stat::AccumulatorStats::Moment<2>, ::Apto::Stat::AccumulatorStats::Mean>::Type>::Result,
                                     ::Apto::Stat::AccumulatorStats::Internal::StatImpl, Base>::SubClass SubClass;
          typedef TypeList< ::Apto::Stat::AccumulatorStats::Variance, typename SubClass::Features> Features;
          typedef typename AccTypes::ValueType ValueType;
          typedef typename AccTypes::FloatType FloatType;
          
        protected:
          inline void clear() { SubClass::clear(); }
          inline void addValue(ValueType value) { SubClass::addValue(value); }
          
        public:
          inline FloatType Variance() const
          {
            FloatType mean = this->Mean();
            return this->Moment(Type::Int<2>()) - (mean * mean);
          }
        };
        
        
        // StatImpl<StdError>
        // --------------------------------------------------------------------------------------------------------------
        
        template <class AccTypes, typename Base> class StatImpl<AccTypes, ::Apto::Stat::AccumulatorStats::StdError, Base>
          : public DependsOn<AccTypes,
                             typename TL::Create< ::Apto::Stat::AccumulatorStats::Variance>::Type,
                             ::Apto::Stat::AccumulatorStats::Internal::StatImpl, Base>::SubClass
        {
        public:
          typedef typename DependsOn<AccTypes,
                                     typename TL::Create< ::Apto::Stat::AccumulatorStats::Variance>::Type,
                                     ::Apto::Stat::AccumulatorStats::Internal::StatImpl, Base>::SubClass SubClass;
          typedef TypeList< ::Apto::Stat::AccumulatorStats::StdError, typename SubClass::Features> Features;
          typedef typename AccTypes::ValueType ValueType;
          typedef typename AccTypes::FloatType FloatType;
          
        protected:
          inline void clear() { SubClass::clear(); }
          inline void addValue(ValueType value) { SubClass::addValue(value); }
          
        public:
          inline FloatType StdError() const { return sqrt(this->Variance() / (this->Count() - 1)); }
        };
        
      };
    };
    
    
  };
};

#endif
