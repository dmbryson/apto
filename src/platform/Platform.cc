/*
 *  Platform.cc
 *  Apto
 *
 *  Created by David on 6/29/07.
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
 */

#include "apto/platform/Platform.h"

#if APTO_PLATFORM(APPLE)
# include <mach/mach.h>
#elif APTO_PLATFORM(UNIX)
# include <unistd.h>
#endif

#include "apto/platform/FloatingPoint.h"


// Initialize various platform settings and system handlers
void Apto::Platform::Initialize()
{
  SetupFloatingPointEnvironment();
}


// Autodetect the number of CPUs on a box, if available.  Otherwise, return 1.
int Apto::Platform::AvailableCPUs()
{
  int ncpus = 1;
  
#if APTO_PLATFORM(APPLE)
  kern_return_t kr;
  host_basic_info_data_t p_host_info;     
  mach_msg_type_number_t count = HOST_BASIC_INFO_COUNT;
  kr = host_info(mach_host_self(), HOST_BASIC_INFO, (host_info_t)&p_host_info, &count);
  if (kr == KERN_SUCCESS) ncpus = (int)(p_host_info.avail_cpus);
#elif APTO_PLATFORM(UNIX) && defined(_SC_NPROCESSORS_ONLN)
  ncpus = (int)sysconf(_SC_NPROCESSORS_ONLN);
#endif
  
  return ncpus;
}
