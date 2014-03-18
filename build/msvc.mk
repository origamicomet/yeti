#=== build/msvc.mk ============================================================#
#                                                                              #
#  Butane                                                                      #
#                                                                              #
#  This file is distributed under the terms described in LICENSE.              #
#                                                                              #
#  Author(s):                                                                  #
#   Michael Williams <mwilliams@bitbyte.ca>                                    #
#                                                                              #
#===------------------------------------------------------------------------===#

## @file build/msvc.mk
## @brief Provides the standardized toolchain interface for Visual Studio.
##

ifndef _BITBYTE_BUTANE_BUILD_MSVC_MK_
_BITBYTE_BUTANE_BUILD_MSVC_MK_ := 1

include build/detect/platform.mk
include build/detect/architecture.mk

#==============================================================================#
# Check for support
#

# Check that the host platform supports the toolchain.
ifeq ($(HOST_PLATFORM),windows-cygwin)
  $(error Compilation on 'windows-cygwin' with 'msvc' is not supported.)
endif
ifeq ($(HOST_PLATFORM),windows-mingw)
  # Supported.
endif
ifeq ($(HOST_PLATFORM),macosx)
  $(error Compilation on 'macosx' with 'msvc' is not supported.)
endif
ifeq ($(HOST_PLATFORM),linux)
  $(error Compilation on 'linux' with 'msvc' is not supported.)
endif

# And check that the toolchain can compile for the target platform.
ifneq ($(TARGET_PLATFORM),$(HOST_PLATFORM))
  $(error Cross-compilation support on 'windows-mingw' with 'msvc' to '$(TARGET_PLATFORM)' is not supported.)
endif

# And check that we can target the specified architecture.
ifeq ($(TARGET_ARCHITECTURE),x86)
  # Supported.
endif
ifeq ($(TARGET_ARCHITECTURE),x86-64)
  # Supported.
endif
ifeq ($(TARGET_ARCHITECTURE),arm)
  $(error Compilation on 'windows-mingw' with 'msvc' targeting 'arm' is not supported.)
endif

#==============================================================================#
# Define CLFAGS, LDFLAGS, and ARFLAGS
#

CFLAGS  := -nologo -c -favor:blend -GF -GR- -W4

LDFLAGS := -nologo -manifest:embed

ARFLAGS := -nologo

ifeq ($(TARGET_ARCHITECTURE),x86)
  CFLAGS  += -arch:SSE2
  LDFLAGS += -machine:X86
  ARFLAGS += -machine:X86
endif
ifeq ($(TARGET_ARCHITECTURE),x86-64)
  LDFLAGS += -machine:X64
  ARFLAGS += -machine:X64
endif

#==============================================================================#
# Implement the standardized interface
#

cc                           = build/msvc -TC $(CFLAGS)
c++                          = build/msvc -TP $(CFLAGS)
cc-input                     = "$(1)"
cc-output                    = -Fo"$(1)" -Fd"$(patsubst %.o,%.pdb,$(1))"
cc-includes                  = -I"$(1)"
cc-define                    = -D$(1)
cc-position-independent-code =
cc-generate-dependencies     = -MM -MT"$(1)"
cc-debug                     = -MDd -Od -Zi -RTCsu -fp:precise -fp:except -D_DEBUG
cc-development               = -MD -Zi -fp:fast -fp:except- -D_NDEBUG
cc-release                   = -MD -GL -Ox -fp:fast -fp:except- -D_NDEBUG

ar                           = lib $(ARFLAGS)
ar++                         = lib $(ARFLAGS)
ar-input                     = "$(1)"
ar-output                    = -OUT:"$(1)"
ar-debug                     =
ar-development               =
ar-release                   = -LTCG

ld                           = link $(LDFLAGS)
ld++                         = link $(LDFLAGS)
ld-input                     = "$(1)"
ld-output                    = -OUT:"$(1)"
ld-libraries                 = -LIBPATH:"$(1)"
ld-link                      = $(1).lib
ld-shared                    = -DLL
ld-debug                     = -DEBUG
ld-development               = -DEBUG
ld-release                   = -LTCG

#==============================================================================#
# Platform-specific hacks
#

mingw-path = /$(subst //,/,$(subst :,/,$(1)))

WINDOWS_SDK_ := $(call mingw-path,$(WINDOWS_SDK))
VS_PATH_     := $(call mingw-path,$(VS_PATH))

ifeq ($(TARGET_ARCHITECTURE),x86)
  export PATH := $(WINDOWS_SDK_)/Bin:$(VS_PATH_)/Common7/IDE:$(VS_PATH_)/VC/Bin:$(PATH)
  CFLAGS      += -I"$(WINDOWS_SDK)/Include" -I"$(VS_PATH)/VC/include"
  LDFLAGS     += -LIBPATH:"$(WINDOWS_SDK)/Lib" -LIBPATH:"$(VS_PATH)/VC/Lib"
endif
ifeq ($(TARGET_ARCHITECTURE),x86-64)
  export PATH := $(WINDOWS_SDK_)/Bin/x64:$(VS_PATH_)/Common7/IDE:$(VS_PATH_)/VC/Bin/x86_amd64:$(VS_PATH_)/VC/Bin:$(PATH)
  CFLAGS      += -I"$(WINDOWS_SDK)/Include" -I"$(VS_PATH)/VC/include"
  LDFLAGS     += -LIBPATH:"$(WINDOWS_SDK)/Lib/x64" -LIBPATH:"$(VS_PATH)/VC/Lib/amd64"
endif

endif # _BITBYTE_BUTANE_BUILD_MSVC_MK_
