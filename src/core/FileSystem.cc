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
#include "apto/platform/FileSystem.h"

#include <cerrno>
#include <sys/stat.h>
#include <cstdio>
#include <fstream>


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

    String GetCWD()
    {
      String cwd_str;
      
      char* dirbuf = new char[MAXIMUM_DIRECTORY_LENGTH];
#if APTO_PLATFORM(WINDOWS)
      char* cwd = _getcwd(dirbuf, MAXIMUM_DIRECTORY_LENGTH);
#else
      char* cwd = getcwd(dirbuf, MAXIMUM_DIRECTORY_LENGTH);
#endif
      if (cwd != NULL) cwd_str = cwd;
      delete [] dirbuf;
      
      return cwd_str;
    }

    
    String GetAbsolutePath(const String& path, const String& working_dir)
    {
      if (path.GetSize() == 0) return working_dir;

#if APTO_PLATFORM(WINDOWS)
      if (!(path.IsLetter(0) && path[1] == ':' && path[2] =='\\')) return PathAppend(working_dir, path);
#else
      if (path[0] != '/' && path[0] != '\\') return PathAppend(working_dir, path);
#endif
      
      return path;
    }
    
    bool IsDir(const String& path)
    {
      struct stat st;
      if (stat(path, &st) == 0 && st.st_mode & S_IFDIR) return true;
      return false;
    }
    
    bool IsFile(const String& path)
    {
      struct stat st;
      if (stat(path, &st) == 0 && st.st_mode & S_IFREG) return true;
      return false;
    }
    
    
    bool CpDir(const String& src, const String& dest)
    {
      if (!IsDir(src)) return false;
      
      MkDir(dest);

      // Collect entries in directory
      Array<String, Smart> direntries;
      ReadDir(src, direntries);
      
      // Process all entries, removing them as appropriate
      for (int i = 0; i < direntries.GetSize(); i++) {
        // Do not copy current or parent directory entries
        if (direntries[i] == "." || direntries[i] == "..") continue;
        
        Apto::String src_path = PathAppend(src, direntries[i]);
        Apto::String dest_path = PathAppend(dest, direntries[i]);
        
        struct stat st;
        if (stat(src_path, &st) != 0) continue;
        
        if (st.st_mode & S_IFDIR) {
          // Recursively copy subdirectory
          if (!CpDir(src_path, dest_path)) return false;
        } else if (st.st_mode & S_IFREG) {
          // Cop regular file
          if (!CpFile(src_path, dest_path)) return false;
        }
      }
      
      return true;
    }
    
    
    bool MkDir(const String& dirname)
    {
      FILE* fp = 0;

#if APTO_PLATFORM(WINDOWS)
      fopen_s(&fp, dirname, "r");
      if (fp == 0) {
#else
      fp = fopen(dirname, "r");
      if (fp == 0) {
#endif
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
    
    
    bool RmDir(const String& dirname, bool recursive)
    {
      if (!IsDir(dirname)) return false;
      
      if (recursive) {
        // Collect entries in directory
        Array<String, Smart> direntries;
        ReadDir(dirname, direntries);
        
        // Process all entries, removing them as appropriate
        for (int i = 0; i < direntries.GetSize(); i++) {
          // Do not remove current or parent directory entries
          if (direntries[i] == "." || direntries[i] == "..") continue;
          
          Apto::String path = PathAppend(dirname, direntries[i]);
          
          struct stat st;
          if (stat(path, &st) != 0) continue;
          
          if (st.st_mode & S_IFDIR) {
            // Recursively remove subdirectory
            if (!RmDir(path)) return false;
          } else {
            // Remove regular (or other) file
            if (remove(path) != 0) return false;
          }
        }
      }
      
      // Attempt to remove the directory itself
#if APTO_PLATFORM(WINDOWS)
      return (_rmdir(dirname) == 0);
#else
      return (rmdir(dirname) == 0);
#endif
    }
    
    
    bool ReadDir(const String& path, Array<String, Smart>& entries)
    {
      DIR* dp;
      struct dirent* dirp;
      if ((dp = opendir(path)) == NULL) return false;
      
      while ((dirp = readdir(dp)) != NULL) entries.Push(dirp->d_name);
      
      closedir(dp);      
      return true;
    }
    
    
    bool CpFile(const String& src, const String& dest)
    {
      std::ifstream ifs(src, std::ios::binary);
      std::ofstream ofs(dest, std::ios::binary);
      
      if (!ifs.is_open() || !ofs.is_open()) return false;
      
      ofs << ifs.rdbuf();
      
      ifs.close();
      ofs.close();
      
      return true;
    }
    

    bool RmFile(const String& path)
    {
      return (remove(path) == 0);
    }
    
  };
};
