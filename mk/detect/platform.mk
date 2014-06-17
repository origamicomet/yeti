#===-- mk/detect/platform.mk -----------------------------------------------===#
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

## \file mk/detect/platform.mk
## \brief Detects the current platform based on the output of `uname -s`.
##

ifndef _BITBYTE_BUTANE_BUILD_DETECT_PLATFORM_MK_
_BITBYTE_BUTANE_BUILD_DETECT_PLATFORM_MK_ := 1

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

endif # _BITBYTE_BUTANE_BUILD_DETECT_PLATFORM_MK_
