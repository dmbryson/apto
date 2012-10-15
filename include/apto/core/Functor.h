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

#include "apto/core/Malloc.h"
#include "apto/core/SmartPtr.h"
#include "apto/core/TypeList.h"
#include "apto/core/TypeTraits.h"

#include "apto/platform/Visibility.h"

#include <cassert>


namespace Apto {
  
  // Internal Functor Implemenation
  // --------------------------------------------------------------------------------------------------------------
  
  namespace Internal {
    template <typename R, class Allocator>
    class FunctorBase : public ClassAllocator<Allocator>
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
        //assert(typeid(*clone) == typeid(*obj));
        return clone;
      }
    };
  };
  

  // Functor Container Interface Definitions (supporting up to 16 arguments)
  // --------------------------------------------------------------------------------------------------------------
  
  template <typename ReturnType, class TList, class Allocator> class FunctorContainer;
  
  template <
    typename ReturnType,
    class Allocator
  > class FunctorContainer<ReturnType, NullType, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:    
    virtual ReturnType operator()() = 0;
  };

  template <
    typename ReturnType,
    typename P1,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    virtual ReturnType operator()(Param1) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    virtual ReturnType operator()(Param1, Param2) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    virtual ReturnType operator()(Param1, Param2, Param3) = 0;
  };

  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    virtual ReturnType operator()(Param1, Param2, Param3, Param4) = 0;
  };

  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7>, Allocator >
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11, Param12) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11, Param12, Param13) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13, typename P14,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11, Param12, Param13, Param14) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11, Param12, Param13, Param14, Param15) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16,
    class Allocator
  > class FunctorContainer<ReturnType, TL::Create<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15, P16>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11, Param12, Param13, Param14, Param15, Param16) = 0;
  };

  
  template <
    typename ReturnType,
    typename P1,
    class Allocator
  > class FunctorContainer<ReturnType, TypeList<P1, NullType>, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    virtual ReturnType operator()(Param1) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2,
    class Allocator
  > class FunctorContainer<ReturnType, TypeList<P2, TypeList<P1, NullType> >, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    virtual ReturnType operator()(Param1, Param2) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3,
    class Allocator
  > class FunctorContainer<ReturnType, TypeList<P3, TypeList<P2, TypeList<P1, NullType> > >, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    virtual ReturnType operator()(Param1, Param2, Param3) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4,
    class Allocator
  > class FunctorContainer<ReturnType, TypeList<P4, TypeList<P3, TypeList<P2, TypeList<P1, NullType> > > >, Allocator>
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    virtual ReturnType operator()(Param1, Param2, Param3, Param4) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P5, TypeList<P4, TypeList<P3, TypeList<P2, TypeList<P1, NullType> > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P6, TypeList<P5, TypeList<P4, TypeList<P3, TypeList<P2, TypeList<P1, NullType> > > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P7, TypeList<P6, TypeList<P5, TypeList<P4, TypeList<P3, TypeList<P2, TypeList<P1, NullType> > > > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
  {
  public:
    typedef typename TypeTraits<P1>::ParameterType Param1;
    typedef typename TypeTraits<P2>::ParameterType Param2;
    typedef typename TypeTraits<P3>::ParameterType Param3;
    typedef typename TypeTraits<P4>::ParameterType Param4;
    typedef typename TypeTraits<P5>::ParameterType Param5;
    typedef typename TypeTraits<P6>::ParameterType Param6;
    typedef typename TypeTraits<P7>::ParameterType Param7;
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P8, TypeList<P7, TypeList<P6, TypeList<P5, TypeList<P4, TypeList<P3, TypeList<P2,
      TypeList<P1, NullType> > > > > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P9, TypeList<P8, TypeList<P7, TypeList<P6, TypeList<P5, TypeList<P4, TypeList<P3, TypeList<P2,
      TypeList<P1, NullType> > > > > > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P10, TypeList<P9, TypeList<P8, TypeList<P7, TypeList<P6, TypeList<P5, TypeList<P4, TypeList<P3, TypeList<P2,
      TypeList<P1, NullType> > > > > > > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P11, TypeList<P10, TypeList<P9, TypeList<P8, TypeList<P7, TypeList<P6, TypeList<P5, TypeList<P4, TypeList<P3,
      TypeList<P2, TypeList<P1, NullType> > > > > > > > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P12, TypeList<P11, TypeList<P10, TypeList<P9, TypeList<P8, TypeList<P7, TypeList<P6, TypeList<P5, TypeList<P4,
      TypeList<P3, TypeList<P2, TypeList<P1, NullType> > > > > > > > > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11, Param12) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P13, TypeList<P12, TypeList<P11, TypeList<P10, TypeList<P9, TypeList<P8, TypeList<P7, TypeList<P6, TypeList<P5,
      TypeList<P4, TypeList<P3, TypeList<P2, TypeList<P1, NullType> > > > > > > > > > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11, Param12, Param13) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13, typename P14,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P14, TypeList<P13, TypeList<P12, TypeList<P11, TypeList<P10, TypeList<P9, TypeList<P8, TypeList<P7, TypeList<P6,
      TypeList<P5, TypeList<P4, TypeList<P3, TypeList<P2, TypeList<P1, NullType> > > > > > > > > > > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11, Param12, Param13, Param14) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P15, TypeList<P14, TypeList<P13, TypeList<P12, TypeList<P11, TypeList<P10, TypeList<P9, TypeList<P8,
      TypeList<P7, TypeList<P6, TypeList<P5, TypeList<P4, TypeList<P3, TypeList<P2, TypeList<P1, NullType>
    > > > > > > > > > > > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11, Param12, Param13, Param14, Param15) = 0;
  };
  
  template <
    typename ReturnType,
    typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16,
    class Allocator
  > class FunctorContainer<
    ReturnType,
    TypeList<P16, TypeList<P15, TypeList<P14, TypeList<P13, TypeList<P12, TypeList<P11, TypeList<P10, TypeList<P9,
      TypeList<P8, TypeList<P7, TypeList<P6, TypeList<P5, TypeList<P4, TypeList<P3, TypeList<P2, TypeList<P1, NullType>
    > > > > > > > > > > > > > > >,
    Allocator
  >
  : public Internal::FunctorBase<ReturnType, Allocator>
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
    virtual ReturnType operator()(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,
                                  Param11, Param12, Param13, Param14, Param15, Param16) = 0;
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

  template <typename R = void, class TList = NullType, class Allocator = BasicMalloc> class Functor
  {
  public:
    typedef FunctorContainer<R, TList, Allocator> ContainerType;
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
    mutable SmartPtr<ContainerType, DestructiveCopy> m_container;
    
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
      LIB_EXPORT void Placeholder() { ; }
    };
    
    typedef void (Tester::*PlaceholderBooleanType)();
    
  public:
    inline operator PlaceholderBooleanType() const { return (m_container ? &Tester::Placeholder : NULL); }
  };
  

  // Parameter Binding Internal Support
  // --------------------------------------------------------------------------------------------------------------
  
  namespace Internal {
    
    template <class Fctor> struct BindFirstTraits;
    template <typename R, class TList, class A> struct BindFirstTraits<Functor<R, TList, A> >
    {
      typedef Functor<R, TList, A> OriginalFunctor;
      typedef typename TL::Remove<TList, typename TL::TypeAt<TList, 0>::Result>::Result ParameterList;
      typedef typename TL::TypeAt<TList, 0>::Result OriginalParam1;
      typedef Functor<R, ParameterList, A> BoundFunctorType;
      typedef typename BoundFunctorType::ContainerType ContainerType;
    };
    
    
    template <class T> struct BindTypeStorage;
    template <class T> struct BindTypeStorage
    {
      typedef typename TypeTraits<T>::ParameterType RefOrValue;
    };
    template <typename R, class TList, class A> struct BindTypeStorage<Functor<R, TList, A> >
    {
      typedef Functor<R, TList, A> OriginalFunctor;
      typedef const typename TypeTraits<OriginalFunctor>::ReferredType RefOrValue;
    };
    
    
    template <class OriginalFunctor> class BindFirst : public BindFirstTraits<OriginalFunctor>::ContainerType
    {
    public:
      typedef typename BindFirstTraits<OriginalFunctor>::ContainerType Base;
      typedef typename OriginalFunctor::ReturnType ReturnType;
      typedef typename OriginalFunctor::Param1 BoundType;
      typedef typename BindTypeStorage<typename BindFirstTraits<OriginalFunctor>::OriginalParam1>::RefOrValue
        BoundTypeStorage;
      
      typedef typename OriginalFunctor::Param2 Param1;
      typedef typename OriginalFunctor::Param3 Param2;
      typedef typename OriginalFunctor::Param4 Param3;
      typedef typename OriginalFunctor::Param5 Param4;
      typedef typename OriginalFunctor::Param6 Param5;
      typedef typename OriginalFunctor::Param7 Param6;
      typedef typename OriginalFunctor::Param8 Param7;
      typedef typename OriginalFunctor::Param9 Param8;
      typedef typename OriginalFunctor::Param10 Param9;
      typedef typename OriginalFunctor::Param11 Param10;
      typedef typename OriginalFunctor::Param12 Param11;
      typedef typename OriginalFunctor::Param13 Param12;
      typedef typename OriginalFunctor::Param14 Param13;
      typedef typename OriginalFunctor::Param15 Param14;
      typedef typename OriginalFunctor::Param16 Param15;
      typedef EmptyType Param16;
      
    private:
      OriginalFunctor m_functor;
      BoundTypeStorage m_param;
      
    public:
      BindFirst(const OriginalFunctor& functor, BoundType param) : m_functor(functor), m_param(param) { ; }
      BindFirst* DoClone() const { return new BindFirst(*this); }
      
      ReturnType operator()()
      {
        return m_functor(m_param);
      }
      
      ReturnType operator()(Param1 p1)
      {
        return m_functor(m_param, p1);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2)
      {
        return m_functor(m_param, p1, p2);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3)
      {
        return m_functor(m_param, p1, p2, p3);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
      {
        return m_functor(m_param, p1, p2, p3, p4);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
      {
        return m_functor(m_param, p1, p2, p3, p4, p5);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6)
      {
        return m_functor(m_param, p1, p2, p3, p4, p5, p6);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7)
      {
        return m_functor(m_param, p1, p2, p3, p4, p5, p6, p7);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8)
      {
        return m_functor(m_param, p1, p2, p3, p4, p5, p6, p7, p8);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8,
                            Param9 p9)
      {
        return m_functor(m_param, p1, p2, p3, p4, p5, p6, p7, p8, p9);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8,
                            Param9 p9, Param10 p10)
      {
        return m_functor(m_param, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8,
                            Param9 p9, Param10 p10, Param11 p11)
      {
        return m_functor(m_param, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8,
                            Param9 p9, Param10 p10, Param11 p11, Param12 p12)
      {
        return m_functor(m_param, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8,
                            Param9 p9, Param10 p10, Param11 p11, Param12 p12, Param13 p13)
      {
        return m_functor(m_param, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8,
                            Param9 p9, Param10 p10, Param11 p11, Param12 p12, Param13 p13, Param14 p14)
      {
        return m_functor(m_param, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
      }
      
      ReturnType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param2 p8,
                            Param9 p9, Param10 p10, Param11 p11, Param12 p12, Param13 p13, Param14 p14, Param15 p15)
      {
        return m_functor(m_param, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
      }
    };
  };
  
  template <class Fctor>
  typename Internal::BindFirstTraits<Fctor>::BoundFunctorType BindFirst(const Fctor& f, typename Fctor::Param1 p)
  {
    typedef typename Internal::BindFirstTraits<Fctor>::BoundFunctorType Outgoing;
    return Outgoing(SmartPtr<typename Outgoing::ContainerType, DestructiveCopy>(new Internal::BindFirst<Fctor>(f, p)));
  }
  

};

#endif
