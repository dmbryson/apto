/*
 *  HierarchyGeneration.h
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

#ifndef AptoCoreHierarchyGeneration_h
#define AptoCoreHierarchyGeneration_h

#include "apto/core/TypeList.h"


namespace Apto {
  
  // LinearHierarchy
  // --------------------------------------------------------------------------------------------------------------
  
  template <class TList, template <class AtomicType, class Base> class Unit, class Root = EmptyType>
  class LinearHierarchy;
  
  template <class T1, class T2, template <class, class> class Unit, class Root>
  class LinearHierarchy<TypeList<T1, T2>, Unit, Root>
  : public Unit<T1, LinearHierarchy<T2, Unit, Root> >
  {
  protected:
    typedef T1 SubClass;
  };
  
  template <class T, template <class, class> class Unit, class Root>
  class LinearHierarchy<TypeList<T, NullType>, Unit, Root> : public Unit<T, Root>
  {
  protected:
    typedef T SubClass;
  };
  
  template <template <class, class> class Unit, class Root>
  class LinearHierarchy<NullType, Unit, Root> : public Root
  {
  protected:
    typedef NullType SubClass;
  };

  
  // Inherit
  // --------------------------------------------------------------------------------------------------------------  
  
  template <class T, class U> struct Inherit : public T, public U
  {
  };
  
};

#endif
