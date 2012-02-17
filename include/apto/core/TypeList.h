/*
 *  TypeList.h
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
 *  TypeList based off of ideas presented in Modern C++ Design (2001) by Andrei Alexandrescu
 */

#ifndef AptoCoreTypeList_h
#define AptoCoreTypeList_h

#include "apto/core/TypeUtil.h"


namespace Apto {
  
  // TypeList
  // --------------------------------------------------------------------------------------------------------------
  
  template <class T, class U> struct TypeList
  {
    typedef T Current;
    typedef U Next;
  };
  
  
  namespace TL {

    // TL::Create
    // --------------------------------------------------------------------------------------------------------------
    
    template <
      typename T1  = NullType, typename T2  = NullType, typename T3  = NullType, typename T4  = NullType,
      typename T5  = NullType, typename T6  = NullType, typename T7  = NullType, typename T8  = NullType,
      typename T9  = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType,
      typename T13 = NullType, typename T14 = NullType, typename T15 = NullType, typename T16 = NullType,
      typename T17 = NullType, typename T18 = NullType, typename T19 = NullType, typename T20 = NullType,
      typename T21 = NullType, typename T22 = NullType, typename T23 = NullType, typename T24 = NullType,
      typename T25 = NullType, typename T26 = NullType, typename T27 = NullType, typename T28 = NullType,
      typename T29 = NullType, typename T30 = NullType, typename T31 = NullType, typename T32 = NullType
    > class Create
    {
    private:
      typedef typename Create<
             T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9,  T10, T11, T12, T13, T14, T15, T16,
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32
      >::Type Next;
      
    public:
      typedef TypeList<T1, Next> Type;
    };
    
    template <> class Create<>
    {
    public:
      typedef NullType Type;
    };
    
    
    // TL::Length
    // --------------------------------------------------------------------------------------------------------------
    
    
    template <class TList> struct Length;
    template <> struct Length<NullType>
    {
      enum { Value = 0 };
    };
    template <class T, class U> struct Length<TypeList<T, U> >
    {
      enum { Value = 1 + Length<U>::Value };
    };
    

    // TL::TypeAt
    // --------------------------------------------------------------------------------------------------------------
    
    template <class TList, unsigned int idx> struct TypeAt;
    template <class Current, class Next>
    struct TypeAt<TypeList<Current, Next>, 0>
    {
      typedef Current Result;
    };
    template <class Current, class Next, unsigned int idx>
    struct TypeAt<TypeList<Current, Next>, idx>
    {
      typedef typename TypeAt<Next, idx - 1>::Result Result;
    };
    
    template <
      typename T1,  typename T2,  typename T3,  typename T4,  typename T5,  typename T6,  typename T7,  typename T8,
      typename T9,  typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16,
      typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
      typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32,
      unsigned int idx
    >
    struct TypeAt<
      Create<
        T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9,  T10, T11, T12, T13, T14, T15, T16,
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32
      >, idx
    >
    {
      typedef typename TypeAt<
        typename Create<
          T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9,  T10, T11, T12, T13, T14, T15, T16,
          T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32
        >::Type
      , idx>::Result Result;
    };
    
    
    // TL::TypeAtNonStrict
    // --------------------------------------------------------------------------------------------------------------
    
    template <class TList, unsigned int idx, typename Default = NullType> struct TypeAtNonStrict
    {
      typedef Default Result;
    };
    template <class Current, class Next, typename Default>
    struct TypeAtNonStrict<TypeList<Current, Next>, 0, Default>
    {
      typedef Current Result;
    };
    template <class Current, class Next, unsigned int idx, typename Default>
    struct TypeAtNonStrict<TypeList<Current, Next>, idx, Default>
    {
      typedef typename TypeAtNonStrict<Next, idx - 1, Default>::Result Result;
    };
    
    template <
      typename T1,  typename T2,  typename T3,  typename T4,  typename T5,  typename T6,  typename T7,  typename T8,
      typename T9,  typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16,
      typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
      typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32,
      unsigned int idx, typename Default
    >
    struct TypeAtNonStrict<
      Create<
        T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9,  T10, T11, T12, T13, T14, T15, T16,
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32
      >, idx, Default
    >
    {
      typedef typename TypeAtNonStrict<
        typename Create<
          T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9,  T10, T11, T12, T13, T14, T15, T16,
          T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32
        >::Type
      , idx, Default>::Result Result;
    };
    
    
    // TL::IndexOf
    // --------------------------------------------------------------------------------------------------------------
    
    template <class TList, class T> struct IndexOf;
    template <class T> struct IndexOf<NullType, T>
    {
      enum { Value = -1 };
    };
    template <class T, class Next> struct IndexOf<TypeList<T, Next>, T>
    {
      enum { Value = 0 };
    };
    template <class Current, class Next, class T> struct IndexOf<TypeList<Current, Next>, T>
    {
      enum { Value = (IndexOf<Next, T>::Value == -1) ? -1 : 1 + IndexOf<Next, T>::Value };
    };
    template <
      typename T1,  typename T2,  typename T3,  typename T4,  typename T5,  typename T6,  typename T7,  typename T8,
      typename T9,  typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16,
      typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
      typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32,
      class T
    >
    struct IndexOf<
      Create<
        T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9,  T10, T11, T12, T13, T14, T15, T16,
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32
      >, T
    >
    {
      enum {
        Value = IndexOf<
          typename Create<
            T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9,  T10, T11, T12, T13, T14, T15, T16,
            T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32
          >::Type
          , T>::Value
      };
    };
    

    // TL::Remove
    // --------------------------------------------------------------------------------------------------------------
    
    template <class TList, class T> struct Remove;
    template <class T> struct Remove<NullType, T>
    {
      typedef NullType Result;
    };
    template <class T, class Next> struct Remove<TypeList<T, Next>, T>
    {
      typedef Next Result;
    };
    template <class Current, class Next, class T> struct Remove<TypeList<Current, Next>, T>
    {
      typedef TypeList<Current, typename Remove<Next, T>::Result> Result;
    };
    template <
      typename T1,  typename T2,  typename T3,  typename T4,  typename T5,  typename T6,  typename T7,  typename T8,
      typename T9,  typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16,
      typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
      typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32,
      class T
    >
    struct Remove<
      Create<
        T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9,  T10, T11, T12, T13, T14, T15, T16,
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32
      >, T
    >
    {
      typedef typename Remove<
        typename Create<
          T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9,  T10, T11, T12, T13, T14, T15, T16,
          T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32
        >::Type
        , T
      >::Result Result;
    };
    
    
    // TL::Unique
    // --------------------------------------------------------------------------------------------------------------
    
    template <class TList> struct Unique;
    template <> struct Unique<NullType> { typedef NullType Result; };
    template <class Head, class Tail> struct Unique<TypeList<Head, Tail> >
    {
    private:
      typedef typename Unique<Tail>::Result L1;
      typedef typename Remove<L1, Head>::Result L2;
    public:
      typedef TypeList<Head, L2> Result;
    };
    
    
    // TL::Replace
    // --------------------------------------------------------------------------------------------------------------
    
    template <class TList, class T, class U> struct Replace;
    template <class T, class U> struct Replace<NullType, T, U> { typedef NullType Result; };
    template <class T, class Tail, class U> struct Replace<TypeList<T, Tail>, T, U> { typedef TypeList<U, Tail> Result; };
    template <class Head, class Tail, class T, class U> struct Replace<TypeList<Head, Tail>, T, U>
    {
      typedef TypeList<Head, typename Replace<Tail, T, U>::Result> Result;
    };

    
    // TL::MostDerived
    // --------------------------------------------------------------------------------------------------------------

    template <class TList, class T> struct MostDerived;
    template <class T> struct MostDerived<NullType, T> { typedef T Result; };
    template <class Head, class Tail, class T> struct MostDerived<TypeList<Head, Tail>, T>
    {
    private:
      typedef typename MostDerived<Tail, T>::Result Candidate;
    public:
      typedef typename TypeSelect<IsSubclassOf<Candidate, Head>::Result, Head, Candidate>::Result Result;
    };
    
    
    // TL::SortDerived
    // --------------------------------------------------------------------------------------------------------------
    
    template <class TList> struct SortDerived;
    template <> struct SortDerived<NullType> { typedef NullType Result; };
    template <class Head, class Tail> struct SortDerived<TypeList<Head, Tail> >
    {
    private:
      typedef typename MostDerived<Tail, Head>::Result TheMostDerived;
      typedef typename Replace<Tail, TheMostDerived, Head>::Result Temp;
      typedef typename SortDerived<Temp>::Result RestOfList;
    public:
      typedef TypeList<TheMostDerived, RestOfList> Result;
    };

  };
};

#endif
