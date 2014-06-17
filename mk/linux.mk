#===-- mk/linux.mk ---------------------------------------------------------===#
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

## \file mk/linux.mk
## \brief Opaquely handles platform specific prefixes, suffixes, and extensions
## as well as system libraries for Linux.
##

ifndef _BITBYTE_BUTANE_BUILD_LINUX_MK_
_BITBYTE_BUTANE_BUILD_LINUX_MK_ := 1

#===------------------------------------------------------------------------===#
# Prefixes, suffixes, and extensions
#

EXECUTABLE_PREFIX    :=
EXECUTABLE_SUFFIX    :=
EXECUTABLE_EXTENSION :=
STATIC_LIB_PREFIX    := lib
STATIC_LIB_SUFFIX    :=
STATIC_LIB_EXTENSION := .a
SHARED_LIB_PREFIX    := lib
SHARED_LIB_SUFFIX    :=
SHARED_LIB_EXTENSION := .so

#===------------------------------------------------------------------------===#
# System libraries
#

LDFLAGS += $(call ld-link,rt)

endif # _BITBYTE_BUTANE_BUILD_LINUX_MK_
