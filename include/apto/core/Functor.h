/*
 *  Functor.h
 *  Apto
 *
 *  Created by David on 5/12/11.
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
 *  Functor based off of ideas presented in Modern C++ Design (2001) by Andrei Alexandrescu
 */

#ifndef AptoCoreFunctor_h
#define AptoCoreFunctor_h

#include "apto/core/SmartPtr.h"
#include "apto/core/TypeList.h"
#include "apto/core/TypeTraits.h"

#include <cassert>


namespace Apto {
  
  // Internal Functor Implemenation
  // --------------------------------------------------------------------------------------------------------------
  
  namespace Internal {
    template <typename R> class FunctorBase
    {
    public:
      typedef R ReturnType;
      typedef EmptyType Param1;
      typedef EmptyType Param2;
      typedef EmptyType Param3;
      typedef EmptyType Param4;
      typedef EmptyType Param5;
      typedef EmptyType Param6;
      typedef EmptyType Param7;
      typedef EmptyType Param8;
      typedef EmptyType Param9;
      typedef EmptyType Param10;
      typedef EmptyType Param11;
      typedef EmptyType Param12;
      typedef EmptyType Param13;
      typedef EmptyType Param14;
      typedef EmptyType Param15;
      typedef EmptyType Param16;
      
      virtual FunctorBase* DoClone() const = 0;
      virtual ~FunctorBase() { ; }
      
      template <class U>
      static U* Clone(U* obj)
      {
        if (!obj) return NULL;
        U* clone = static_cast<U*>(obj->DoClone());
        assert(typeid(*clone) == typeid(*obj));
        return clone;
      }
    };
  };
  

  // Functor Container Interface Definitions (supporting up to 16 arguments)
  // --------------------------------------------------------------------------------------------------------------
  
  template <typename ReturnType, class TList> class FunctorContainer;
  
  template <
    typename ReturnType
  > class FunctorContainer<ReturnType, NullType>
  : public Internal::FunctorBase<ReturnType>
  {
  public:    
    virtual ReturnType operator()() = 0;
  };

  template <
    typename ReturnType,
    typename P1
  > class FunctorContainer<ReturnType, TL::Create<P1> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    virtual ReturnType operator()(P1) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2
  > class FunctorContainer<ReturnType, TL::Create<P1, P2> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    virtual ReturnType operator()(P1, P2) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    virtual ReturnType operator()(P1, P2, P3) = 0;
  };

  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    virtual ReturnType operator()(P1, P2, P3, P4) = 0;
  };

  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    virtual ReturnType operator()(P1, P2, P3, P4, P5) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    virtual ReturnType operator()(P1, P2, P3, P4, P5, P6) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7> > : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    virtual ReturnType operator()(P1, P2, P3, P4, P5, P6, P7) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    typedef typename TypeTraits<P8>::ParameterType Param8;
    virtual ReturnType operator()(P1, P2, P3, P4, P5, P6, P7, P8) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    typedef typename TypeTraits<P8>::ParameterType Param8;
    typedef typename TypeTraits<P9>::ParameterType Param9;
    virtual ReturnType operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    typedef typename TypeTraits<P8>::ParameterType Param8;
    typedef typename TypeTraits<P9>::ParameterType Param9;
    typedef typename TypeTraits<P10>::ParameterType Param10;
    virtual ReturnType operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    typedef typename TypeTraits<P8>::ParameterType Param8;
    typedef typename TypeTraits<P9>::ParameterType Param9;
    typedef typename TypeTraits<P10>::ParameterType Param10;
    typedef typename TypeTraits<P11>::ParameterType Param11;
    virtual ReturnType operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    typedef typename TypeTraits<P8>::ParameterType Param8;
    typedef typename TypeTraits<P9>::ParameterType Param9;
    typedef typename TypeTraits<P10>::ParameterType Param10;
    typedef typename TypeTraits<P11>::ParameterType Param11;
    typedef typename TypeTraits<P12>::ParameterType Param12;
    virtual ReturnType operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    typedef typename TypeTraits<P8>::ParameterType Param8;
    typedef typename TypeTraits<P9>::ParameterType Param9;
    typedef typename TypeTraits<P10>::ParameterType Param10;
    typedef typename TypeTraits<P11>::ParameterType Param11;
    typedef typename TypeTraits<P12>::ParameterType Param12;
    typedef typename TypeTraits<P13>::ParameterType Param13;
    virtual ReturnType operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13, typename P14
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    typedef typename TypeTraits<P8>::ParameterType Param8;
    typedef typename TypeTraits<P9>::ParameterType Param9;
    typedef typename TypeTraits<P10>::ParameterType Param10;
    typedef typename TypeTraits<P11>::ParameterType Param11;
    typedef typename TypeTraits<P12>::ParameterType Param12;
    typedef typename TypeTraits<P13>::ParameterType Param13;
    typedef typename TypeTraits<P14>::ParameterType Param14;
    virtual ReturnType operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    typedef typename TypeTraits<P8>::ParameterType Param8;
    typedef typename TypeTraits<P9>::ParameterType Param9;
    typedef typename TypeTraits<P10>::ParameterType Param10;
    typedef typename TypeTraits<P11>::ParameterType Param11;
    typedef typename TypeTraits<P12>::ParameterType Param12;
    typedef typename TypeTraits<P13>::ParameterType Param13;
    typedef typename TypeTraits<P14>::ParameterType Param14;
    typedef typename TypeTraits<P15>::ParameterType Param15;
    virtual ReturnType operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15, P16> >
  : public Internal::FunctorBase<ReturnType>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    typedef typename TypeTraits<P8>::ParameterType Param8;
    typedef typename TypeTraits<P9>::ParameterType Param9;
    typedef typename TypeTraits<P10>::ParameterType Param10;
    typedef typename TypeTraits<P11>::ParameterType Param11;
    typedef typename TypeTraits<P12>::ParameterType Param12;
    typedef typename TypeTraits<P13>::ParameterType Param13;
    typedef typename TypeTraits<P14>::ParameterType Param14;
    typedef typename TypeTraits<P15>::ParameterType Param15;
    typedef typename TypeTraits<P16>::ParameterType Param16;
    virtual ReturnType operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15, P16) = 0;
  };
  
  
  // BasicFunctorHandler - handle raw function pointers and other functor objects
  // --------------------------------------------------------------------------------------------------------------

  template <class ParentFunctor, typename ActionType> class BasicFunctorHandler : public ParentFunctor::ContainerType
  {
    typedef typename ParentFunctor::ContainerType Base;
    
  public:
    typedef typename Base::ReturnType ReturnType;
    typedef typename Base::Param1 Param1;
    typedef typename Base::Param2 Param2;
    typedef typename Base::Param3 Param3;
    typedef typename Base::Param4 Param4;
    typedef typename Base::Param5 Param5;
    typedef typename Base::Param6 Param6;
    typedef typename Base::Param7 Param7;
    typedef typename Base::Param8 Param8;
    typedef typename Base::Param9 Param9;
    typedef typename Base::Param10 Param10;
    typedef typename Base::Param11 Param11;
    typedef typename Base::Param12 Param12;
    typedef typename Base::Param13 Param13;
    typedef typename Base::Param14 Param14;
    typedef typename Base::Param15 Param15;
    typedef typename Base::Param16 Param16;
    
  private:
    ActionType m_action;
    
  public:
    BasicFunctorHandler(const ActionType& action) : m_action(action) { ; }
    BasicFunctorHandler* DoClone() const { return new BasicFunctorHandler(*this); }
    
    ReturnType operator()()
    {
      return m_action();
    }
    
    ReturnType operator()(Param1 p1)
    {
      return m_action(p1);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2)
    {
      return m_action(p1, p2);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3)
    {
      return m_action(p1, p2, p3);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
    {
      return m_action(p1, p2, p3, p4);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
    {
      return m_action(p1, p2, p3, p4, p5);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6)
    {
      return m_action(p1, p2, p3, p4, p5, p6);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7)
    {
      return m_action(p1, p2, p3, p4, p5, p6, p7);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8)
    {
      return m_action(p1, p2, p3, p4, p5, p6, p7, p8);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9)
    {
      return m_action(p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10)
    {
      return m_action(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11)
    {
      return m_action(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12)
    {
      return m_action(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13)
    {
      return m_action(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13, Param14 p14)
    {
      return m_action(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    }

    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13, Param14 p14, Param15 p15)
    {
      return m_action(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13, Param14 p14, Param15 p15, Param16 p16)
    {
      return m_action(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
    }
  };
  
  
  // Member Function Handler
  // --------------------------------------------------------------------------------------------------------------

  template <class ParentFunctor, typename ObjectPointer, typename MemberFunction>
  class MemberFunctionHandler : public ParentFunctor::ContainerType
  {
    typedef typename ParentFunctor::ContainerType Base;
    
  public:
    typedef typename Base::ReturnType ReturnType;
    typedef typename Base::Param1 Param1;
    typedef typename Base::Param2 Param2;
    typedef typename Base::Param3 Param3;
    typedef typename Base::Param4 Param4;
    typedef typename Base::Param5 Param5;
    typedef typename Base::Param6 Param6;
    typedef typename Base::Param7 Param7;
    typedef typename Base::Param8 Param8;
    typedef typename Base::Param9 Param9;
    typedef typename Base::Param10 Param10;
    typedef typename Base::Param11 Param11;
    typedef typename Base::Param12 Param12;
    typedef typename Base::Param13 Param13;
    typedef typename Base::Param14 Param14;
    typedef typename Base::Param15 Param15;
    typedef typename Base::Param16 Param16;
    
  private:
    ObjectPointer m_obj;
    MemberFunction m_action;
    
  public:
    MemberFunctionHandler(const ObjectPointer& obj, MemberFunction action) : m_obj(obj), m_action(action) { ; }
    MemberFunctionHandler* DoClone() const { return new MemberFunctionHandler(*this); }
    
    ReturnType operator()()
    {
      return ((*m_obj).*m_action)();
    }
    
    ReturnType operator()(Param1 p1)
    {
      return ((*m_obj).*m_action)(p1);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2)
    {
      return ((*m_obj).*m_action)(p1, p2);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3)
    {
      return ((*m_obj).*m_action)(p1, p2, p3);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5, p6);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5, p6, p7);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5, p6, p7, p8);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13, Param14 p14)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13, Param14 p14, Param15 p15)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13, Param14 p14, Param15 p15, Param16 p16)
    {
      return ((*m_obj).*m_action)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
    }
  };
  
  
  // Functor
  // --------------------------------------------------------------------------------------------------------------

  template <typename R = void, class TList = NullType> class Functor
  {
  public:
    typedef FunctorContainer<R, TList> ContainerType;
    typedef R ReturnType;
    typedef TList ParameterList;
    typedef typename ContainerType::Param1 Param1;
    typedef typename ContainerType::Param2 Param2;
    typedef typename ContainerType::Param3 Param3;
    typedef typename ContainerType::Param4 Param4;
    typedef typename ContainerType::Param5 Param5;
    typedef typename ContainerType::Param6 Param6;
    typedef typename ContainerType::Param7 Param7;
    typedef typename ContainerType::Param8 Param8;
    typedef typename ContainerType::Param9 Param9;
    typedef typename ContainerType::Param10 Param10;
    typedef typename ContainerType::Param11 Param11;
    typedef typename ContainerType::Param12 Param12;
    typedef typename ContainerType::Param13 Param13;
    typedef typename ContainerType::Param14 Param14;
    typedef typename ContainerType::Param15 Param15;
    typedef typename ContainerType::Param16 Param16;
    
  private:
    SmartPtr<ContainerType, DestructiveCopy> m_container;
    
  public:
    Functor() : m_container(NULL) { ; }
    Functor(const Functor& rhs)
      : m_container(ContainerType::Clone(SmartPtr<ContainerType, DestructiveCopy>::GetPointer(rhs.m_container))) { ; }
    Functor(SmartPtr<ContainerType, DestructiveCopy> container) : m_container(container) { ; }
    
    template <typename ActionType>
    Functor(ActionType action) : m_container(new BasicFunctorHandler<Functor, ActionType>(action)) { ; }
    
    template <typename ObjectPointer, typename MemberFunction>
    Functor(const ObjectPointer& obj, MemberFunction action)
      : m_container(new MemberFunctionHandler<Functor, ObjectPointer, MemberFunction>(obj, action)) { ; }
    
    Functor& operator=(const Functor& rhs)
    {
      Functor temp(rhs);
      m_container = temp.m_container;
      return *this;
    }
    
    
    ReturnType operator()() const
    {
      return (*m_container)();
    }

    ReturnType operator()(Param1 p1) const
    {
      return (*m_container)(p1);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2) const
    {
      return (*m_container)(p1, p2);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3) const
    {
      return (*m_container)(p1, p2, p3);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const
    {
      return (*m_container)(p1, p2, p3, p4);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const
    {
      return (*m_container)(p1, p2, p3, p4, p5);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const
    {
      return (*m_container)(p1, p2, p3, p4, p5, p6);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const
    {
      return (*m_container)(p1, p2, p3, p4, p5, p6, p7);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8) const
    {
      return (*m_container)(p1, p2, p3, p4, p5, p6, p7, p8);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8,
                          Param9 p9) const
    {
      return (*m_container)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10) const
    {
      return (*m_container)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11) const
    {
      return (*m_container)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12) const
    {
      return (*m_container)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13) const
    {
      return (*m_container)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13, Param14 p14) const
    {
      return (*m_container)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13, Param14 p14, Param15 p15) const
    {
      return (*m_container)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
    }
    
    ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8, Param9 p9,
                          Param10 p10, Param11 p11, Param12 p12, Param13 p13, Param14 p14, Param15 p15, Param16 p16) const
    {
      return (*m_container)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
    }

    
  private:
    struct Tester
    {
      void Placeholder() { ; }
    };
    
    typedef void (Tester::*PlaceholderBooleanType)();
    
  public:
    inline operator PlaceholderBooleanType() const { return (m_container ? &Tester::Placeholder : NULL); }
  };
  
  

};

#endif
