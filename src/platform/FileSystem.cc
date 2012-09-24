/*
 *  FileSystem.cc
 *  Apto
 *
 *  Created by David on 1/13/12.
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

#include "apto/platform/FileSystem.h"

#if APTO_PLATFORM(WINDOWS)

/*****************************************************************************
 * dirent.h - dirent API for Microsoft Visual Studio
 *
 * Copyright (C) 2006 Toni Ronkko
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * ``Software''), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL TONI RONKKO BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Aug 11, 2010, Toni Ronkko
 * Added d_type and d_namlen fields to dirent structure.  The former is
 * especially useful for determining whether directory entry represents a
 * file or a directory.  For more information, see
 * http://www.delorie.com/gnu/docs/glibc/libc_270.html
 *
 * Aug 11, 2010, Toni Ronkko
 * Improved conformance to the standards.  For example, errno is now set
 * properly on failure and assert() is never used.  Thanks to Peter Brockam
 * for suggestions.
 *
 * Aug 11, 2010, Toni Ronkko
 * Fixed a bug in rewinddir(): when using relative directory names, change
 * of working directory no longer causes rewinddir() to fail.
 *
 * Dec 15, 2009, John Cunningham
 * Added rewinddir member function
 *
 * Jan 18, 2008, Toni Ronkko
 * Using FindFirstFileA and WIN32_FIND_DATAA to avoid converting string
 * between multi-byte and unicode representations.  This makes the
 * code simpler and also allows the code to be compiled under MingW.  Thanks
 * to Azriel Fasten for the suggestion.
 *
 * Mar 4, 2007, Toni Ronkko
 * Bug fix: due to the strncpy_s() function this file only compiled in
 * Visual Studio 2005.  Using the new string functions only when the
 * compiler version allows.
 *
 * Nov  2, 2006, Toni Ronkko
 * Major update: removed support for Watcom C, MS-DOS and Turbo C to
 * simplify the file, updated the code to compile cleanly on Visual
 * Studio 2005 with both unicode and multi-byte character strings,
 * removed rewinddir() as it had a bug.
 *
 * Aug 20, 2006, Toni Ronkko
 * Removed all remarks about MSVC 1.0, which is antiqued now.  Simplified
 * comments by removing SGML tags.
 *
 * May 14 2002, Toni Ronkko
 * Embedded the function definitions directly to the header so that no
 * source modules need to be included in the Visual Studio project.  Removed
 * all the dependencies to other projects so that this very header can be
 * used independently.
 *
 * May 28 1998, Toni Ronkko
 * First version.
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

  /*****************************************************************************
   * Open directory stream DIRNAME for read and return a pointer to the
   * internal working area that is used to retrieve individual directory
   * entries.
   */
  DIR *opendir(const char *dirname)
  {
    DIR *dirp;
    
    /* ensure that the resulting search pattern will be a valid file name */
    if (dirname == NULL) {
      DIRENT_SET_ERRNO (ENOENT);
      return NULL;
    }
    if (strlen (dirname) + 3 >= MAX_PATH) {
      DIRENT_SET_ERRNO (ENAMETOOLONG);
      return NULL;
    }
    
    /* construct new DIR structure */
    dirp = (DIR*) malloc (sizeof (struct DIR));
    if (dirp != NULL) {
      int error;
      
      /*
       * Convert relative directory name to an absolute directory one.  This
       * allows rewinddir() to function correctly when the current working
       * directory is changed between opendir() and rewinddir().
       */
      if (GetFullPathNameA (dirname, MAX_PATH, dirp->patt, NULL)) {
        char *p;
        
        /* append the search pattern "\\*\0" to the directory name */
        p = strchr (dirp->patt, '\0');
        if (dirp->patt < p  &&  *(p-1) != '\\'  &&  *(p-1) != ':') {
          *p++ = '\\';
        }
        *p++ = '*';
        *p = '\0';
        
        /* open directory stream and retrieve the first entry */
        dirp->search_handle = FindFirstFileA (dirp->patt, &dirp->find_data);
        if (dirp->search_handle != INVALID_HANDLE_VALUE) {
          /* a directory entry is now waiting in memory */
          dirp->cached = 1;
          error = 0;
        } else {
          /* search pattern is not a directory name? */
          DIRENT_SET_ERRNO (ENOENT);
          error = 1;
        }
      } else {
        /* buffer too small */
        DIRENT_SET_ERRNO (ENOMEM);
        error = 1;
      }
      
      if (error) {
        free (dirp);
        dirp = NULL;
      }
    }
    
    return dirp;
  }
  
  
  /*****************************************************************************
   * Read a directory entry, and return a pointer to a dirent structure
   * containing the name of the entry in d_name field.  Individual directory
   * entries returned by this very function include regular files,
   * sub-directories, pseudo-directories "." and "..", but also volume labels,
   * hidden files and system files may be returned.
   */
  struct dirent *readdir(DIR *dirp)
  {
    DWORD attr;
    if (dirp == NULL) {
      /* directory stream did not open */
      DIRENT_SET_ERRNO (EBADF);
      return NULL;
    }
    
    /* get next directory entry */
    if (dirp->cached != 0) {
      /* a valid directory entry already in memory */
      dirp->cached = 0;
    } else {
      /* get the next directory entry from stream */
      if (dirp->search_handle == INVALID_HANDLE_VALUE) {
        return NULL;
      }
      if (FindNextFileA (dirp->search_handle, &dirp->find_data) == FALSE) {
        /* the very last entry has been processed or an error occured */
        FindClose (dirp->search_handle);
        dirp->search_handle = INVALID_HANDLE_VALUE;
        return NULL;
      }
    }
    
    /* copy as a multibyte character string */
    DIRENT_STRNCPY ( dirp->curentry.d_name,
                    dirp->find_data.cFileName,
                    sizeof(dirp->curentry.d_name) );
    dirp->curentry.d_name[MAX_PATH] = '\0';
    
    /* compute the length of name */
    dirp->curentry.d_namlen = strlen (dirp->curentry.d_name);
    
    /* determine file type */
    attr = dirp->find_data.dwFileAttributes;
    if ((attr & FILE_ATTRIBUTE_DEVICE) != 0) {
      dirp->curentry.d_type = DT_CHR;
    } else if ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0) {
      dirp->curentry.d_type = DT_DIR;
    } else {
      dirp->curentry.d_type = DT_REG;
    }
    return &dirp->curentry;
  }
  
  
  /*****************************************************************************
   * Close directory stream opened by opendir() function.  Close of the
   * directory stream invalidates the DIR structure as well as any previously
   * read directory entry.
   */
  int closedir(DIR *dirp)
  {
    if (dirp == NULL) {
      /* invalid directory stream */
      DIRENT_SET_ERRNO (EBADF);
      return -1;
    }
    
    /* release search handle */
    if (dirp->search_handle != INVALID_HANDLE_VALUE) {
      FindClose (dirp->search_handle);
      dirp->search_handle = INVALID_HANDLE_VALUE;
    }
    
    /* release directory structure */
    free (dirp);
    return 0;
  }
  
  
  /*****************************************************************************
   * Resets the position of the directory stream to which dirp refers to the
   * beginning of the directory.  It also causes the directory stream to refer
   * to the current state of the corresponding directory, as a call to opendir()
   * would have done.  If dirp does not refer to a directory stream, the effect
   * is undefined.
   */
  void rewinddir(DIR* dirp)
  {
    if (dirp != NULL) {
      /* release search handle */
      if (dirp->search_handle != INVALID_HANDLE_VALUE) {
        FindClose (dirp->search_handle);
      }
      
      /* open new search handle and retrieve the first entry */
      dirp->search_handle = FindFirstFileA (dirp->patt, &dirp->find_data);
      if (dirp->search_handle != INVALID_HANDLE_VALUE) {
        /* a directory entry is now waiting in memory */
        dirp->cached = 1;
      } else {
        /* failed to re-open directory: no directory entry in memory */
        dirp->cached = 0;
      }
    }
  }
  
  
#ifdef __cplusplus
}
#endif


#endif

