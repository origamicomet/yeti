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
#  * Enviornment hack                                                          #
################################################################################

################################################################################
# CC & CXX & LD & LDXX                                                         #
################################################################################

CC   := mk/toolchains/msvc/cl
CXX  := mk/toolchains/msvc/cl
LD   := link
LDXX := link

################################################################################
# Flags:                                                                       #
################################################################################

# See http://superuser.com/a/376617.

CFLAGS    := -nologo -c -TC -favor:blend -GF -GR- -W4

CXXFLAGS  := -nologo -c -TP -favor:blend -GF -GR- -W4

LDFLAGS   := -nologo -manifest:embed

LDXXFLAGS := -nologo -manifest:embed

################################################################################
# Macros:                                                                      #
################################################################################

mingw-path = /$(subst //,/,$(subst :,/,$(1)))

# See http://msdn.microsoft.com/en-us/library/19z1t1wy.aspx.

cc = $(call __cc_$(1),$(subst $(1),,$(subst $(strip $(1),),,$(args))))

  __cc_                           = $(CC) $(CFLAGS)
  __cc_deps                       = mk/toolchains/msvc/cl-deps $(CFLAGS) -Zs -showIncludes -MT"$(1)"
  __cc_position_independent_code  =
  __cc_input                      = "$(1)"
  __cc_output                     = -Fo"$(1)" -Fd"$(patsubst %.obj,%.pdb,$(1))"
  __cc_dir                        = -I"$(1)"
  __cc_define                     = -D$(1)
  __cc_debug                      = -MDd -Od -Zi -RTCsu -fp:precise -fp:except
  __cc_development                = -MD -Zi -fp:fast -fp:except-
  __cc_release                    = -MD -GL -Ox -fp:fast -fp:except-

cxx = $(call __cxx_$(1),$(subst $(1),,$(subst $(strip $(1),),,$(args))))

  __cxx_                          = $(CXX) $(CXXFLAGS)
  __cxx_deps                      = mk/toolchains/msvc/cl-deps $(CXXFLAGS) -Zs -showIncludes -MT"$(1)"
  __cxx_position_independent_code = $(__cc_position_independent_code)
  __cxx_input                     = $(__cc_input)
  __cxx_output                    = $(__cc_output)
  __cxx_dir                       = $(__cc_dir)
  __cxx_define                    = $(__cc_define)
  __cxx_debug                     = $(__cc_debug)
  __cxx_development               = $(__cc_development)
  __cxx_release                   = $(__cc_release)

# See http://msdn.microsoft.com/en-us/library/y0zzbyt4.aspx.

ld = $(call __ld_$(1),$(subst $(1),,$(subst $(strip $(1),),,$(args))))

  __ld_                           = $(LD) $(LDFLAGS)
  __ld_shared                     = -DLL
  __ld_input                      = $(1)
  __ld_output                     = -OUT:"$(1)"
  __ld_dir                        = -LIBPATH:"$(1)"
  __ld_link                       = $(1).lib
  __ld_debug                      = -DEBUG
  __ld_development                = -DEBUG
  __ld_release                    = -LTCG

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
OBJECT_SUFFIX := .obj

################################################################################
# Environment hack:                                                            #
################################################################################

# See http://stackoverflow.com/a/84496.
WINDOWS_SDK_ := $(call mingw-path,$(WINDOWS_SDK))
VS_PATH_     := $(call mingw-path,$(VS_PATH))
export PATH := $(WINDOWS_SDK_)/Bin:$(VS_PATH_)/Common7/IDE:$(VS_PATH_)/VC/Bin:$(PATH)
CFLAGS    += -I"$(WINDOWS_SDK)/Include" -I"$(VS_PATH)/VC/include"
CXXFLAGS  += -I"$(WINDOWS_SDK)/Include" -I"$(VS_PATH)/VC/include"
LDFLAGS   += -LIBPATH:"$(WINDOWS_SDK)/Lib" -LIBPATH:"$(VS_PATH)/VC/Lib"
LDXXFLAGS += -LIBPATH:"$(WINDOWS_SDK)/Lib" -LIBPATH:"$(VS_PATH)/VC/Lib"

# See http://stackoverflow.com/questions/14363929.
# It might not fix the issues, though, but it's always worth a try...
ifeq ($(findstring 11.0,$(VS_PATH)),11.0)
  CFLAGS   += -D_USING_V110_SDK71_=1
  CXXFLAGS += -D_USING_V110_SDK71_=1
endif
