#=== build/toolchain.mk =======================================================#
#                                                                              #
#  Butane                                                                      #
#                                                                              #
#  This file is distributed under the terms described in LICENSE.              #
#                                                                              #
#  Author(s):                                                                  #
#   Michael Williams <mwilliams@bitbyte.ca>                                    #
#                                                                              #
#===------------------------------------------------------------------------===#

## @file build/toolchain.mk
## @brief Abstracts toolchain usage to provide a standard and opaque interface
##  for compiling and linking C, C++, and Assembly code. Ideally it reconciles
##  all differences, but unfortunately it can't. So be aware.
##

ifndef _BITBYTE_BUTANE_BUILD_TOOLCHAIN_MK_
_BITBYTE_BUTANE_BUILD_TOOLCHAIN_MK_ := 1

-include build/config
ifndef _BITBYTE_BUTANE_BUILD_CONFIG_
  $(error Please ./configure first.)
endif

include build/$(TOOLCHAIN).mk

endif # _BITBYTE_BUTANE_BUILD_TOOLCHAIN_MK_
