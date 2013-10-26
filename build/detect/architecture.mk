################################################################################
##                                                                            ##
## This file is part of Foundation.                                           ##
##                                                                            ##
## Author(s):                                                                 ##
##                                                                            ##
##   Michael Williams <devbug@bitbyte.ca>                                     ##
##                                                                            ##
## This is free and unencumbered software released into the public domain.    ##
##                                                                            ##
## Anyone is free to copy, modify, publish, use, compile, sell, or distribute ##
## this software, either in source code form or as a compiled binary, for any ##
## purpose, commercial or non-commercial, and by any means.                   ##
##                                                                            ##
## In jurisdictions that recognize copyright laws, the author or authors of   ##
## this software dedicate any and all copyright interest in the software to   ##
## the public domain. We make this dedication for the benefit of the public   ##
## large and to the detriment of our heirs and successors. We intend this     ##
## dedication to be an overt act of relinquishment in perpetuity of all       ##
## present and future rights to this software under copyright law.            ##
##                                                                            ##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR ##
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   ##
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    ##
## THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER   ##
## OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,      ##
## ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR      ##
## OTHER DEALINGS IN THE SOFTWARE.                                            ##
##                                                                            ##
## For more information, please refer to <http://unlicense.org/>              ##
##                                                                            ##
################################################################################
## build/detect/architecture.mk                                               ##
##  Detects the current architecture based on the output of `uname -m`.       ##
################################################################################

ifndef _BUTANE_BUILD_DETECT_ARCHITECTURE_MK_
_BUTANE_BUILD_DETECT_ARCHITECTURE_MK_ := 1

# http://en.wikipedia.org/wiki/Uname is immensely helpful here.

UNAME_M := $(shell uname -m)
WAS_ABLE_TO_DETECT_HOST_ARCHITECTURE := no

ifeq ($(findstring i386,$(UNAME_M)),i386)
  HOST_ARCHITECTURE := x86
  WAS_ABLE_TO_DETECT_HOST_ARCHITECTURE := yes
endif
ifeq ($(findstring i686,$(UNAME_M)),i686)
  HOST_ARCHITECTURE := x86
  WAS_ABLE_TO_DETECT_HOST_ARCHITECTURE := yes
endif
ifeq ($(findstring i686-64,$(UNAME_M)),i686-64)
  HOST_ARCHITECTURE := x86-64
  WAS_ABLE_TO_DETECT_HOST_ARCHITECTURE := yes
endif
ifeq ($(findstring amd64,$(UNAME_M)),amd64)
  HOST_ARCHITECTURE := x86-64
  WAS_ABLE_TO_DETECT_HOST_ARCHITECTURE := yes
endif
ifeq ($(findstring x86_64,$(UNAME_M)),x86_64)
  HOST_ARCHITECTURE := x86-64
  WAS_ABLE_TO_DETECT_HOST_ARCHITECTURE := yes
endif
ifeq ($(findstring arm,$(UNAME_M)),arm)
  HOST_ARCHITECTURE := arm
  WAS_ABLE_TO_DETECT_HOST_ARCHITECTURE := yes
endif

ifeq ($(WAS_ABLE_TO_DETECT_HOST_ARCHITECTURE),yes)
  $(message Host architecture is '$(HOST_ARCHITECTURE)'...)
else
  $(error Unable to detect host architecture!)
endif

endif # _BUTANE_BUILD_DETECT_ARCHITECTURE_MK_
