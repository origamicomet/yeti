#=== build/platform.mk ========================================================#
#                                                                              #
#  Butane                                                                      #
#                                                                              #
#  This file is distributed under the terms described in LICENSE.              #
#                                                                              #
#  Author(s):                                                                  #
#   Michael Williams <mwilliams@bitbyte.ca>                                    #
#                                                                              #
#===------------------------------------------------------------------------===#

## @file build/platform.mk
## @brief Opaquely handles platform specific prefixes, suffixes, and extensions
##  as well as system libraries.
##

ifndef _BITBYTE_BUTANE_BUILD_PLATFORM_MK_
_BITBYTE_BUTANE_BUILD_PLATFORM_MK_ := 1

include build/toolchain.mk

-include build/config
ifndef _BITBYTE_BUTANE_BUILD_CONFIG_
  $(error Please ./configure first.)
endif

include build/$(TARGET_PLATFORM).mk

endif # _BITBYTE_BUTANE_BUILD_PLATFORM_MK_
