/*
 *  Platform.h
 *  Apto
 *
 *  Created by David on 3/2/07.
 *  Copyright 2007-2011 David Michael Bryson. All rights reserved.
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
 *  Platform detection macro structure inspired by platform detection in Webkit <http://www.webkit.org>
 *  Initial implementation was in Avida <http://avida.devosoft.org>
 */

#ifndef AptoPlatformPlatform_h
#define AptoPlatformPlatform_h

// spaces between defined's parentheses and contained value are required by Visual Studio's preprocessor
#define APTO_PLATFORM(PROP) (defined( APTO_PLATFORM_ ## PROP ) && APTO_PLATFORM_##PROP)

#if defined(WIN32) || defined(_WIN32)
# define APTO_PLATFORM_WINDOWS 1
# ifdef _MSC_VER
#  define APTO_PLATFORM_MSVC 1
#  define NOMINMAX
# else
#  define APTO_PLATFORM_MSVC 0
# endif
# ifdef DISABLE_THREADS
#  define APTO_PLATFORM_THREADS 0
# else
#  define APTO_PLATFORM_THREADS 1
# endif
// Disable warning C4355: 'this' : used in base member initializer list
# pragma warning( disable : 4355 )
#endif

#if defined(__APPLE__) || defined(unix) || defined(__unix) || defined(__unix__) || defined (__NetBSD__) || defined(_AIX) || defined(__FreeBSD__)
# define APTO_PLATFORM_UNIX 1
# define APTO_PLATFORM_THREADS 1
#endif

#if defined(__FreeBSD__)
# define APTO_PLATFORM_FREEBSD 1
#endif

#if defined(__APPLE__)
# define APTO_PLATFORM_APPLE 1
#endif

#if defined(__GNUC__) && __GNUC__ >= 4
# define APTO_PLATFORM_GNUC 1
#endif

#if defined(__hppa__) || defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
(defined(__MIPS__) && defined(__MISPEB__)) || defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
defined(__sparc__)
# define APTO_PLATFORM_BIG_ENDIAN 1
# define APTO_PLATFORM_LITTLE_ENDIAN 0
#else
# define APTO_PLATFORM_BIG_ENDIAN 0
# define APTO_PLATFORM_LITTLE_ENDIAN 1
#endif


#ifdef DEBUG
# include <cstdlib>
# include <iostream>
# define ASSERT_MSG(VALUE, MSG) if (!(VALUE)) { std::cerr << "Error: " << MSG << std::endl; abort(); }
#else
# define ASSERT_MSG(VALUE, MSG)
#endif


#ifndef NULL
#define NULL 0
#endif

// Visibility
#if APTO_PLATFORM(WINDOWS)
# ifdef BUILDING_DLL
#  if APTO_PLATFORM(MSVC)
#   define LIB_EXPORT __declspec(dllexport)
#  elif APTO_PLATFORM(GNUC)
#   define LIB_EXPORT __attribute__((dllexport))
#  else
#   define LIB_EXPORT
#  endif
# else
#  if APTO_PLATFORM(MSVC)
#   define LIB_EXPORT __declspec(dllexport)
#  elif APTO_PLATFORM(GNUC)
#   define LIB_EXPORT __attribute__((dllimport))
#  else
#   define LIB_EXPORT
#  endif
# endif
# define LIB_LOCAL
#else
# if APTO_PLATFORM(GNUC)
#  define LIB_EXPORT __attribute__ ((visibility("default")))
#  define LIB_LOCAL  __attribute__ ((visibility("hidden")))
# else
#  define LIB_EXPORT
#  define LIB_LOCAL
# endif
#endif


namespace Apto {
  namespace Platform {
    LIB_EXPORT void Initialize();
    
    LIB_EXPORT int AvailableCPUs();
  };
};



#endif
