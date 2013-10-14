################################################################################
# This file is part of Butane.
#
# Copyright (c) Michael Williams <devbug@bitbyte.ca>
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer. Redistributions in binary
# form must reproduce the above copyright notice, this list of conditions and
# the following disclaimer in the documentation and/or other materials provided
# with the distribution. Neither the name of the nor the names of its
# contributors may be used to endorse or promote products derived from this
# software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
################################################################################

################################################################################
# Toolchain:                                                                   #
#  * CC & CXX & LD & LDXX                                                      #
#  * Flags                                                                     #
#  * Macros                                                                    #
#  * Prefixes and suffixes                                                     #
################################################################################

################################################################################
# CC & CXX & LD & LDXX                                                         #
################################################################################

CC   := gcc
CXX  := g++
LD   := gcc
LDXX := g++

################################################################################
# Flags:                                                                       #
################################################################################

CFLAGS    := -std=c99 -pedantic -Wall -Wextra -Wfloat-equal -Wshadow \
             -Wunsafe-loop-optimizations -Wpointer-arith -Wcast-qual \
             -Wcast-align -Wmissing-field-initializers -Wpacked \
             -Wpadded -Wredundant-decls -Wunreachable-code -Winline

CXXFLAGS  := -std=c++98 -pedantic -Wall -Wextra -Wfloat-equal -Wshadow \
             -Wunsafe-loop-optimizations -Wpointer-arith -Wcast-qual \
             -Wcast-align -Wmissing-field-initializers -Wpacked \
             -Wpadded -Wredundant-decls -Wunreachable-code -Winline

LDFLAGS   :=

LDXXFLAGS :=

################################################################################
# Macros:                                                                      #
################################################################################

cc = $(call __cc_$(1),$(subst $(1),,$(subst $(strip $(1),),,$(args))))

  __cc_                           = $(CC) $(CFLAGS)
  __cc_deps                       = $(CC) $(CFLAGS) -MM -MT '$(1)'
  __cc_position_independent_code  = -fPIC
  __cc_input                      = -c $(1)
  __cc_output                     = -o $(1)
  __cc_dir                        = -I$(1)
  __cc_define                     = -D$(1)
  __cc_debug                      = -g
  __cc_development                = -g
  __cc_release                    = -03

cxx = $(call __cxx_$(1),$(subst $(1),,$(subst $(strip $(1),),,$(args))))

  __cxx_                          = $(CXX) $(CXXFLAGS)
  __cxx_deps                      = $(CXX) $(CXXFLAGS) -MM -MT '$(1)'
  __cxx_position_independent_code = $(__cc_position_independent_code)
  __cxx_input                     = $(__cc_input)
  __cxx_output                    = $(__cc_output)
  __cxx_dir                       = $(__cc_dir)
  __cxx_define                    = $(__cc_define)
  __cxx_debug                     = $(__cc_debug)
  __cxx_development               = $(__cc_development)
  __cxx_release                   = $(__cc_release)

ld = $(call __ld_$(1),$(subst $(1),,$(subst $(strip $(1),),,$(args))))

  __ld_                           = $(LD) $(LDFLAGS)
  __ld_shared                     = -shared
  __ld_input                      = $(1)
  __ld_output                     = -o $(1)
  __ld_dir                        = -L$(1)
  __ld_link                       = -l$(1)
  __ld_debug                      =
  __ld_development                =
  __ld_release                    =

ldxx = $(call __ldxx_$(1),$(subst $(1),,$(subst $(strip $(1),),,$(args))))

  __ldxx_                         = $(LDXX) $(LDXXFLAGS)
  __ldxx_shared                   = $(__ld_shared)
  __ldxx_input                    = $(__ld_input)
  __ldxx_output                   = $(__ld_output)
  __ldxx_dir                      = $(__ld_dir)
  __ldxx_link                     = $(__ld_link)
  __ldxx_debug                    = $(__ld_debug)
  __ldxx_development              = $(__ld_development)
  __ldxx_release                  = $(__ld_release)

################################################################################
# Prefix and suffixes:                                                         #
################################################################################

OBJECT_PREFIX :=
OBJECT_SUFFIX := .o
