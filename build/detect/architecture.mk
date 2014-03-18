#=== build/detect/architecture.mk =============================================#
#                                                                              #
#  Butane                                                                      #
#                                                                              #
#  This file is distributed under the terms described in LICENSE.              #
#                                                                              #
#  Author(s):                                                                  #
#   Michael Williams <mwilliams@bitbyte.ca>                                    #
#                                                                              #
#===------------------------------------------------------------------------===#

## @file build/detect/architecture.mk
## @brief Detects the current architecture based on the output of `uname -m`.
##

ifndef _BITBYTE_BUTANE_BUILD_DETECT_ARCHITECTURE_MK_
_BITBYTE_BUTANE_BUILD_DETECT_ARCHITECTURE_MK_ := 1

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

endif # _BITBYTE_BUTANE_BUILD_DETECT_ARCHITECTURE_MK_
