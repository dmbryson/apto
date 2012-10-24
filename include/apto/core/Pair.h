/*
 *  Pair.h
 *  Apto
 *
 *  Created by David on 1/11/09.
 *  Copyright 2009-2011 David Michael Bryson. All rights reserved.
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

#ifndef AptoCorePair_h
#define AptoCorePair_h

namespace Apto {
  
  // Pair - a simple combination of two items
  // --------------------------------------------------------------------------------------------------------------
  
  template<class V1, class V2> class Pair
  {
  protected:
    V1 m_v1;
    V2 m_v2;
    
  public:
    inline Pair() { ; }
    inline Pair(const V1& v1) : m_v1(v1) { ; }
    inline Pair(const V1& v1, const V2& v2) : m_v1(v1), m_v2(v2) { ; }
    inline Pair(const Pair& p) : m_v1(p.m_v1), m_v2(p.m_v2) { ; }
    
    inline Pair& operator=(const Pair& rhs) { m_v1 = rhs.m_v1; m_v2 = rhs.m_v2; return *this; }
    
    inline V1& Value1() { return m_v1; }
    inline const V1& Value1() const { return m_v1; }
    
    inline V2& Value2() { return m_v2; }
    inline const V2& Value2() const { return m_v2; }
    
    inline void Set(const V1& v1, const V2& v2) { m_v1 = v1; m_v2 = v2; } 
  };
  
  
  // Coord - extended Pair object that adds helpful methods for x,y coordinate manipulation
  // --------------------------------------------------------------------------------------------------------------
  
  template<class T> class Coord : public Pair<T, T>
  {
  private:
    using Pair<T, T>::m_v1;
    using Pair<T, T>::m_v2;
    
  public:
    inline Coord() : Pair<T, T>(0, 0) { ; }
    inline Coord(const T& x, const T& y) : Pair<T, T>(x, y) { ; }
    
    inline bool operator==(const Coord& rhs) const { return m_v1 == rhs.m_v1 && m_v2 == rhs.m_v2; }
    inline bool operator!=(const Coord& rhs) const { return !operator==(rhs); }
    
    inline Coord& operator+=(const Coord& rhs) { m_v1 += rhs.m_v1; m_v2 += rhs.m_v2; return *this; }
    inline Coord& operator-=(const Coord& rhs) { m_v1 -= rhs.m_v1; m_v2 -= rhs.m_v2; return *this; }
    
    inline Coord operator+(const Coord& rhs) const { return Coord(m_v1 + rhs.m_v1, m_v2 + rhs.m_v2); }
    inline Coord operator-(const Coord& rhs) const { return Coord(m_v1 - rhs.m_v1, m_v2 - rhs.m_v2); }
    inline Coord operator*(const int n) const { return Coord(m_v1 * n, m_v2 * n); }
    
    T& X() { return m_v1; }
    const T& X() const { return m_v1; }

    T& Y() { return m_v2; }
    const T& Y() const { return m_v2; }
  };
};


#endif
