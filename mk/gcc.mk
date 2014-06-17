#===-- mk/gcc.mk -----------------------------------------------------------===#
#
#  Butane
#
#  This file is distributed under the terms described in LICENSE.
#
#  Author(s):
#
#    * Michael Williams <mwilliams@bitbyte.ca>
#
#===------------------------------------------------------------------------===#

## \file mk/gcc.mk
## \brief Provides the standardized toolchain interface for Visual Studio.
##

ifndef _BITBYTE_BUTANE_BUILD_GCC_MK_
_BITBYTE_BUTANE_BUILD_GCC_MK_ := 1

include mk/detect/platform.mk
include mk/detect/architecture.mk

#===------------------------------------------------------------------------===#
# Check for support
#

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

#===------------------------------------------------------------------------===#
# Define CFLAGS, LDFLAGS, and ARFLAGS
#

CFLAGS  := -Wall -Wextra -Wfloat-equal -Wshadow -Wunsafe-loop-optimizations \
           -Wpointer-arith -Wcast-qual -Wcast-align \
           -Wmissing-field-initializers -Wpacked -Wpadded -Wredundant-decls \
           -Wunreachable-code -Winline

LDFLAGS :=

ARFLAGS :=

ifeq ($(TARGET_ARCHITECTURE),x86)
  CFLAGS += -march=i686
endif
ifeq ($(TARGET_ARCHITECTURE),x86-64)
  CFLAGS += -march=x86-64
endif

#===------------------------------------------------------------------------===#
# Implement the standardized interface
#

cc                           = gcc -std=c99 -pedantic $(CFLAGS)
c++                          = g++ -std=c++11 -pedantic $(CFLAGS)
cc-input                     = -c "$(1)"
cc-output                    = -o "$(1)"
cc-includes                  = -I"$(1)"
cc-define                    = -D$(1)
cc-define-str                = -D$(1)="\"$(2)\""
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

endif # _BITBYTE_BUTANE_BUILD_GCC_MK_
