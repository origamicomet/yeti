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
## build/msvc.mk                                                              ##
##  Provides the standardized toolchain interface for Visual Studio.          ##
################################################################################

ifndef _BUTANE_BUILD_GCC_MK_
_BUTANE_BUILD_GCC_MK_ := 1

include build/detect/platform.mk
include build/detect/architecture.mk

################################################################################
# Check for support:                                                           #
################################################################################

# Check that the host platform supports the toolchain.

ifeq ($(HOST_PLATFORM),windows-cygwin)
  $(error Compilation on 'windows-cygwin' with 'gcc' is not supported.)
endif
ifeq ($(HOST_PLATFORM),windows-mingw)
  # Supported.
endif
ifeq ($(HOST_PLATFORM),macosx)
  $(error Compilation on 'macosx' with 'gcc' is not supported.)
endif
ifeq ($(HOST_PLATFORM),linux)
  # Supported.
endif

# And check that the toolchain can compile for the target platform.

ifneq ($(TARGET_PLATFORM),$(HOST_PLATFORM))
  $(error Cross-compilation support on '$(HOST_PLATFORM)' with 'gcc' to '$(TARGET_PLATFORM)' is not supported.)
endif

# And check that we can target the specified architecture.

ifeq ($(TARGET_ARCHITECTURE),x86)
  # Supported.
endif
ifeq ($(TARGET_ARCHITECTURE),x86-64)
  # Supported.
endif
ifeq ($(TARGET_ARCHITECTURE),arm)
  $(error Compilation on '$(HOST_PLATFORM)' with 'gcc' targeting 'arm' is not supported.)
endif

################################################################################
# Define CFLAGS, LDFLAGS and ARFLAGS:                                          #
################################################################################

CFLAGS  := -Wall -Wextra -Wfloat-equal -Wshadow -Wunsafe-loop-optimizations \
           -Wpointer-arith -Wcast-qual -Wcast-align \
           -Wmissing-field-initializers -Wpacked -Wpadded -Wredundant-decls \
           -Wunreachable-code -Winline

LDFLAGS :=

ARFLAGS :=

ifeq ($(TARGET_ARCHITECTURE),x86)
  CFLAGS += -m32
endif
ifeq ($(TARGET_ARCHITECTURE),x86-64)
  CFLAGS += -m64
endif

################################################################################
# Implement the standardized interface:                                        #
################################################################################

cc                           = gcc -std=c99 -pedantic $(CFLAGS)
c++                          = g++ -std=c++98 -pedantic $(CFLAGS)
cc-input                     = -c "$(1)"
cc-output                    = -o "$(1)"
cc-includes                  = -I"$(1)"
cc-define                    = -D$(1)
cc-position-independent-code = -fPIC
cc-generate-dependencies     = -MM -MT"$(1)"
cc-debug                     = -g
cc-development               = -g
cc-release                   = -O3

ar                           = ar -rcs $(ARFLAGS)
ar++                         = ar -rcs $(ARFLAGS)
ar-input                     = "$(1)"
ar-output                    = -o "$(1)"
ar-debug                     =
ar-development               =
ar-release                   =

ld                           = gcc $(LDFLAGS)
ld++                         = g++ $(LDFLAGS)
ld-input                     = "$(1)"
ld-output                    = -o "$(1)"
ld-libraries                 = -L"$(1)"
ld-link                      = -l$(1)
ld-shared                    = -shared
ld-debug                     =
ld-development               =
ld-release                   =

endif # _BUTANE_BUILD_GCC_MK_
