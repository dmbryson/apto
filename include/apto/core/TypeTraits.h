/*
 *  TypeTraits.h
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
 *  TypeTraits based off of ideas presented in Modern C++ Design (2001) by Andrei Alexandrescu
 */

#ifndef AptoCoreTypeTraits_h
#define AptoCoreTypeTraits_h

#include "apto/core/TypeList.h"

namespace Apto {
  
  namespace Types {
    typedef TL::Create<
      unsigned char, unsigned short int, unsigned int, unsigned long int, unsigned long long int
    >::Type StandardUnsignedInts;
    
    typedef TL::Create<
      signed char, signed short int, signed int, signed long int, signed long long int
    >::Type StandardSignedInts;
    
    typedef TL::Create<
      bool, char, wchar_t
    >::Type StandardOtherInts;
    
    typedef TL::Create<
      float, double, long double
    >::Type StandardFloatingPoint;
  }
  
  namespace Internal {
    template <class U> struct MakeParameterType
    {
      typedef const U& Result;
    };
    template <class U> struct MakeParameterType<U&>
    {
      typedef U& Result;
    };
    template <> struct MakeParameterType<void>
    {
      typedef NullType Result;
    };
  }
  
  template <typename T> class TypeTraits
  {
  private:
    template <class U> struct ReferenceTraits
    {
      enum { Result = false };
      typedef U ReferredType;
    };
    template <class U> struct ReferenceTraits<U&>
    {
      enum { Result = true };
      typedef U ReferredType;
    };

    
    template <class U> struct PointerTraits
    {
      enum { Result = false };
      typedef NullType PointeeType;
    };
    template <class U> struct PointerTraits<U*>
    {
      enum { Result = true };
      typedef U PointeeType;
    };
    template <class U> struct PointerTraits<U*&>
    {
      enum { Result = true };
      typedef U PointeeType;
    };
    
    
    template <class U> struct PointerToMemberTraits
    {
      enum { Result = false };
    };
    template <class U, class V> struct PointerToMemberTraits<U V::*>
    {
      enum { Result = true };
    };
    template <class U, class V> struct PointerToMemberTraits<U V::*&>
    {
      enum { Result = true };
    };
    
    
    template <class U> struct UnConst
    {
      typedef U Result;
      enum { IsConst = false };
    };
    template <class U> struct UnConst<const U>
    {
      typedef U Result;
      enum { IsConst = true };
    };
    template <class U> struct UnConst<const U&>
    {
      typedef U Result;
      enum { IsConst = true };
    };
    

    template <class U> struct UnVolatile
    {
      typedef U Result;
      enum { IsVolatile = false };
    };
    template <class U> struct UnVolatile<const U>
    {
      typedef U Result;
      enum { IsVolatile = true };
    };
    template <class U> struct UnVolatile<const U&>
    {
      typedef U Result;
      enum { IsVolatile = true };
    };
    
    
  public:
    typedef typename UnConst<T>::Result NonConstType;
    typedef typename UnVolatile<T>::Result NonVolatileType;
    typedef typename UnVolatile<typename UnConst<T>::Result >::Result UnqualifiedType;
    typedef typename PointerTraits<UnqualifiedType>::PointeeType PointeeType;
    typedef typename ReferenceTraits<T>::ReferredType ReferredType;
    
    enum { IsConst = UnConst<T>::IsConst };
    enum { IsVolatile = UnVolatile<T>::IsVolatile };
    enum { IsReference = ReferenceTraits<UnqualifiedType>::Result };
    enum { IsPointer = PointerTraits<typename ReferenceTraits<UnqualifiedType>::ReferredType>::Result };
    enum { IsMemberPointer = PointerToMemberTraits<typename ReferenceTraits<UnqualifiedType>::ReferredType>::Result };
    enum { IsStandardUnsignedInt = TL::IndexOf<Types::StandardUnsignedInts, UnqualifiedType>::Value >= 0 ||
      TL::IndexOf<Types::StandardUnsignedInts, typename ReferenceTraits<UnqualifiedType>::ReferredType>::Value >= 0 };
    enum { IsStandardSignedInt = TL::IndexOf<Types::StandardSignedInts, UnqualifiedType>::Value >= 0 ||
      TL::IndexOf<Types::StandardSignedInts, typename ReferenceTraits<UnqualifiedType>::ReferredType>::Value >= 0 };
    enum { IsStandardIntegral = IsStandardUnsignedInt || IsStandardSignedInt ||
      TL::IndexOf<Types::StandardOtherInts, UnqualifiedType>::Value >= 0 ||
      TL::IndexOf<Types::StandardOtherInts, typename ReferenceTraits<UnqualifiedType>::ReferredType>::Value >= 0 };
    enum { IsStandardFloatingPoint = TL::IndexOf<Types::StandardFloatingPoint, UnqualifiedType>::Value >= 0 ||
      TL::IndexOf<Types::StandardFloatingPoint, typename ReferenceTraits<UnqualifiedType>::ReferredType>::Value >= 0 };
    enum { IsStandardArithmetic = IsStandardIntegral || IsStandardFloatingPoint };
    enum { IsIntegral = IsStandardIntegral };
    enum { IsFloatingPoint = IsStandardFloatingPoint };
    enum { IsArithmetic = IsIntegral || IsFloatingPoint };
    
    typedef typename TypeSelect<
      IsArithmetic || IsPointer || IsMemberPointer,
      T,
      typename Internal::MakeParameterType<T>::Result
    >::Result ParameterType;
  };
};

#endif
