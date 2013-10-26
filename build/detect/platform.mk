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
## build/detect/platform.mk                                                   ##
##  Detects the current platform based on the output of `uname -s`.           ##
################################################################################

ifndef _BUTANE_BUILD_DETECT_PLATFORM_MK_
_BUTANE_BUILD_DETECT_PLATFORM_MK_ := 1

# http://en.wikipedia.org/wiki/Uname is immensely helpful here.

UNAME_S := $(shell uname -s)
WAS_ABLE_TO_DETECT_HOST_PLATFORM := no

ifeq ($(findstring CYGWIN_NT,$(UNAME_S)),CYGWIN_NT)
  HOST_PLATFORM := windows-cygwin
  WAS_ABLE_TO_DETECT_HOST_PLATFORM := yes
endif
ifeq ($(findstring MINGW32_NT,$(UNAME_S)),MINGW32_NT)
  HOST_PLATFORM := windows-mingw
  WAS_ABLE_TO_DETECT_HOST_PLATFORM := yes
endif
ifeq ($(findstring Darwin,$(UNAME_S)),Darwin)
  HOST_PLATFORM := macosx
  WAS_ABLE_TO_DETECT_HOST_PLATFORM := yes
endif
ifeq ($(findstring Linux,$(UNAME_S)),Linux)
  HOST_PLATFORM := linux
  WAS_ABLE_TO_DETECT_HOST_PLATFORM := yes
endif

ifeq ($(WAS_ABLE_TO_DETECT_HOST_PLATFORM),yes)
  $(message Host platform is '$(HOST_PLATFORM)'...)
else
  $(error Unable to detect host platform!)
endif

endif # _BUTANE_BUILD_DETECT_PLATFORM_MK_
