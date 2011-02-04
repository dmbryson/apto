/*
 *  FileSystem.cc
 *  Apto
 *
 *  Created by David on 12/7/05.
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

#include "apto/core/FileSystem.h"

#include "apto/platform/Platform.h"

#include <cerrno>
#include <sys/stat.h>
#include <cstdio>


// mkdir undefined in ms windows
#if APTO_PLATFORM(WINDOWS)
# include <direct.h>
# ifndef ACCESSPERMS
#  define ACCESSPERMS 0
# endif
# ifndef mkdir
#  define mkdir(path, ignored_mode) _mkdir(path)
# endif
# ifndef mode_t
#  define mode_t unsigned int
# endif
#endif

#if APTO_PLATFORM(WINDOWS)
# include <direct.h>
#else
# include <unistd.h>
#endif


namespace Apto {
  namespace FileSystem {

    bool MkDir(const cString& dirname)
    {
      FILE* fp = fopen(dirname, "r");
      if (fp == 0) {
        if (errno == ENOENT) {
          // not found, creating...
          if (mkdir(dirname, (S_IRWXU | S_IRWXG | S_IRWXO))) return false;
          
          return true;
        }
        
        return false;
      }
      fclose(fp);
      
      // found
      return true;
    }

    
    cString GetCWD()
    {
      cString cwd_str;
      
      char* dirbuf = new char[MAXIMUM_DIRECTORY_LENGTH];    
      char* cwd = getcwd(dirbuf, MAXIMUM_DIRECTORY_LENGTH);
      if (cwd != NULL) cwd_str = cwd;
      delete [] dirbuf;
      
      return cwd_str;
    }

    
    cString GetAbsolutePath(const cString& path, const cString& working_dir)
    {
      if (path.GetSize() == 0 || (path[0] != '/' && path[0] != '\\')) {
        return (cString(working_dir) + "/" + path);
      }
      
      return path;
    }
    
  };
};