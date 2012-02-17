/*
 *  TypeUtil.h
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
 */

#ifndef AptoCoreTypeUtil_h
#define AptoCoreTypeUtil_h

namespace Apto {
  
  // TypeSelect
  // --------------------------------------------------------------------------------------------------------------
  
  template <bool flag, typename T, typename U> struct TypeSelect { typedef T Result; };
  template <typename T, typename U> struct TypeSelect<false, T, U> { typedef U Result; };
  
  
  // TypeAsType
  // --------------------------------------------------------------------------------------------------------------
  
  template <typename T> struct TypeAsType
  {
    typedef T BaseType;
  };

  
  // Type : Integral Constants as Types
  // --------------------------------------------------------------------------------------------------------------
  
  namespace Type {  
    template <int N> struct Int { enum { Value = N }; };
  };


  // Placeholder Types
  // --------------------------------------------------------------------------------------------------------------
  class NullType { ; };
  class EmptyType { ; };
  

  // IsSameType
  // --------------------------------------------------------------------------------------------------------------
  
  template <typename T, typename U> struct IsSameType { enum { Result = false }; };
  template <typename T> struct IsSameType<T, T> { enum { Result = true }; };
  
    
  // TypeConversion
  // --------------------------------------------------------------------------------------------------------------
  
  // Inspired by The Loki Library Conversion - Copyright 2001 by Andrei Alexandrescu

  namespace Internal {
    template <typename T, typename U> struct TypeConversionHelper
    {
      typedef char Small;
      struct Big { char foo[2]; };
      static Big Test(...);
      static Small Test(U);
      static T MakeT();
    };
  };
  
  template <typename T, typename U> struct TypeConversion
  {
    typedef Internal::TypeConversionHelper<T, U> Helper;
    enum { Exists = sizeof(typename Helper::Small) == sizeof((Helper::Test(Helper::MakeT()))) };
    enum { ExistsTwoWay = Exists && TypeConversion<U, T>::Exists };
    enum { SameType = false };
  };
  
  template <typename T> struct TypeConversion<T, T> { enum { Exists = true, ExistsTwoWay = true, SameType = true }; };
  template <typename T> struct TypeConversion<void, T> { enum { Exists = false, ExistsTwoWay = false, SameType = false }; };
  template <typename T> struct TypeConversion<T, void> { enum { Exists = false, ExistsTwoWay = false, SameType = false }; };
  template <> struct TypeConversion<void, void> { enum { Exists = true, ExistsTwoWay = true, SameType = true }; };
  

  // IsSubclassOf
  // --------------------------------------------------------------------------------------------------------------
  
  template <typename T, typename U> struct IsSubclassOf
  {
    enum { Result = (TypeConversion<const volatile U*, const volatile T*>::Exists &&
                     !TypeConversion<const volatile T*, const volatile void*>::SameType) };
    enum { disallowIncompleteTypes = (sizeof(T) == sizeof(U)) };
  };
  
  template <> struct IsSubclassOf<void, void> { enum { Result = false }; };
  
  template <typename U> struct IsSubclassOf<void, U>
  {
    enum { Result = (TypeConversion<const volatile U*, const volatile void*>::Exists &&
                     !TypeConversion<const volatile void*, const volatile void*>::SameType) };
    enum { disallowIncompleteTypes = (0 == sizeof(U)) };
  };
  
  template <typename T> struct IsSubclassOf<T, void>
  {
    enum { Result = (TypeConversion<const volatile void*, const volatile T*>::Exists &&
                     !TypeConversion<const volatile T*, const volatile void*>::SameType) };
    enum { disallowIncompleteTypes = (sizeof(T) == 0) };
  };
  
  // IsStrictSubclassOf
  // --------------------------------------------------------------------------------------------------------------
  
  template <typename T, typename U> struct IsStrictSubclassOf
  {
    enum { Result = (TypeConversion<const volatile U*, const volatile T*>::Exists &&
                     !TypeConversion<const volatile T*, const volatile void*>::SameType &&
                     !TypeConversion<const volatile T*, const volatile U*>::SameType) };
    enum { disallowIncompleteTypes = (sizeof(T) == sizeof(U)) };
  };
  
  template <> struct IsStrictSubclassOf<void, void> { enum { Result = false }; };
  
  template <typename U> struct IsStrictSubclassOf<void, U>
  {
    enum { Result = (TypeConversion<const volatile U*, const volatile void*>::Exists &&
                     !TypeConversion<const volatile void*, const volatile void*>::SameType &&
                     !TypeConversion<const volatile void*, const volatile U*>::SameType) };
    enum { disallowIncompleteTypes = (0 == sizeof(U)) };
  };
  
  template <typename T> struct IsStrictSubclassOf<T, void>
  {
    enum { Result = (TypeConversion<const volatile void*, const volatile T*>::Exists &&
                     !TypeConversion<const volatile T*, const volatile void*>::SameType) };
    enum { disallowIncompleteTypes = (sizeof(T) == 0) };
  };
};

#endif
