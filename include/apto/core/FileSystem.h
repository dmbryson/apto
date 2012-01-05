/*
 *  FileSystem.h
 *  Apto
 *
 *  Created by David.
 *  Copyright 2005-2011 David Michael Bryson. All rights reserved.
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
#ifndef AptoCoreFileSystem_h
#define AptoCoreFileSystem_h

#include "apto/core/Array.h"
#include "apto/core/String.h"
#include "apto/platform/Platform.h"
#include "apto/platform/Visibility.h"


namespace Apto {
  namespace FileSystem {

    const int MAXIMUM_DIRECTORY_LENGTH = 2048;
    
    LIB_EXPORT String GetCWD();
    LIB_EXPORT String GetAbsolutePath(const String& path, const String& working_dir);
    LIB_EXPORT inline String PathAppend(const String& path, const String& path_add);
    
    LIB_EXPORT bool IsDir(const String& path);
    LIB_EXPORT bool IsFile(const String& path);
    
    LIB_EXPORT bool CpDir(const String& src, const String& dest);
    LIB_EXPORT bool MkDir(const String& dirname);
    LIB_EXPORT bool RmDir(const String& dirname, bool recursive = false);
    LIB_EXPORT bool ReadDir(const String& path, Array<String, Smart>& entries);
    
    LIB_EXPORT bool CpFile(const String& src, const String& dest);
    LIB_EXPORT bool RmFile(const String& path);
  };
};


inline Apto::String Apto::FileSystem::PathAppend(const String& path, const String& path_add)
{
#if APTO_PLATFORM(WINDOWS)
  return String(path) + "\\" + path_add;  
#else
  return String(path) + "/" + path_add;
#endif
}

#endif
