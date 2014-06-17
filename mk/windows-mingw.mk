#===-- mk/windows-mingw.mk -------------------------------------------------===#
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

## \file mk/windows-mingw.mk
## \brief Opaquely handles platform specific prefixes, suffixes, and extensions
## as well as system libraries for MinGW/MSYS.
##

ifndef _BITBYTE_BUTANE_WINDOWS_MINGW_MK_
_BITBYTE_BUTANE_WINDOWS_MINGW_MK_ := 1

#===------------------------------------------------------------------------===#
# Prefixes, suffixes, and extensions
#

EXECUTABLE_PREFIX    :=
EXECUTABLE_SUFFIX    :=
EXECUTABLE_EXTENSION := .exe
STATIC_LIB_PREFIX    :=
STATIC_LIB_SUFFIX    :=
STATIC_LIB_EXTENSION := .lib
SHARED_LIB_PREFIX    :=
SHARED_LIB_SUFFIX    :=
SHARED_LIB_EXTENSION := .dll

#===------------------------------------------------------------------------===#
# System defines
#

CFLAGS += $(call cc-def,WINVER=0x0501)
CFLAGS += $(call cc-define,_WIN32_WINNT=0x0501)

#===------------------------------------------------------------------------===#
# System libraries
#

LDFLAGS += $(call ld-link,kernel32) $(call ld-link,user32)

endif # _BITBYTE_BUTANE_WINDOWS_MINGW_MK_
