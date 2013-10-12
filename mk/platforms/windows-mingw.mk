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
# Platform:                                                                    #
#  * Compilation support                                                       #
#  * Prefix and suffixes                                                       #
#  * System Libraries                                                          #
################################################################################

################################################################################
# Compilation support:                                                         #
################################################################################

# Cross compiling?
COMPILATION_IS_SUPPORTED := no
ifeq ($(HOST_PLATFORM),windows-cygwin)
  COMPILATION_IS_SUPPORTED := no
endif
ifeq ($(HOST_PLATFORM),windows-mingw)
  COMPILATION_IS_SUPPORTED := yes
endif
ifeq ($(HOST_PLATFORM),macosx)
  COMPILATION_IS_SUPPORTED := no
endif
ifeq ($(HOST_PLATFORM),linux)
  COMPILATION_IS_SUPPORTED := no
endif
ifneq ($(COMPILATION_IS_SUPPORTED),yes)
  $(error Compilation support on '$(HOST_PLATFORM)' for 'windows-mingw' isn't available right now.)
endif

# Architecture?
COMPILATION_IS_SUPPORTED := no
ifeq ($(ARCHITECTURE),x86)
  COMPILATION_IS_SUPPORTED := yes
endif
ifeq ($(ARCHITECTURE),x86-64)
  COMPILATION_IS_SUPPORTED := yes
endif
ifeq ($(ARCHITECTURE),arm)
  COMPILATION_IS_SUPPORTED := no
endif
ifneq ($(COMPILATION_IS_SUPPORTED),yes)
  $(error Compilation support for 'windows-mingw' on '$(ARCHITECTURE)' isn't available right now.)
endif

################################################################################
# Prefix and suffixes:                                                         #
################################################################################

EXECUTABLE_PREFIX :=
EXECUTABLE_SUFFIX := .exe
STATIC_LIB_PREFIX := lib
STATIC_LIB_SUFFIX := .a
SHARED_LIB_PREFIX := lib
SHARED_LIB_SUFFIX := .dll

################################################################################
# System Libraries:                                                            #
################################################################################

ifeq ($(ARCHITECTURE),x86)
  DEPENDENCIES += $(call link kernel32) $(call link user32)
endif
ifeq ($(ARCHITECTURE),x86-64)
  $(error System libraries are not known for 'windows-mingw' on 'x86-64'.)
endif
