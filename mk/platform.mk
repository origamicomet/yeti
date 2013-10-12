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

# Determine the current platform based on `uname -s`:
# http://en.wikipedia.org/wiki/Uname is immensely helpful here.
UNAME_S := $(shell uname -s)
ifeq ($(findstring CYGWIN_NT,$(UNAME_S)),CYGWIN_NT)
  HOST_PLATFORM := windows-cygwin
endif
ifeq ($(findstring MINGW32_NT,$(UNAME_S)),MINGW32_NT)
  HOST_PLATFORM := windows-mingw
endif
ifeq ($(findstring Darwin,$(UNAME_S)),Darwin)
  HOST_PLATFORM := macosx
endif
ifeq ($(findstring Linux,$(UNAME_S)),Linux)
  HOST_PLATFORM := linux
endif

# Determine the current architecture based on `uname -m`:
# Again, http://en.wikipedia.org/wiki/Uname is immensely helpful here.
UNAME_M := $(shell uname -m)
ifeq ($(findstring i386,$(UNAME_M)),i386)
  HOST_ARCHITECTURE := x86
endif
ifeq ($(findstring i686,$(UNAME_M)),i686)
  HOST_ARCHITECTURE := x86
endif
ifeq ($(findstring i686-64,$(UNAME_M)),i686-64)
  HOST_ARCHITECTURE := x86-64
endif
ifeq ($(findstring amd64,$(UNAME_M)),amd64)
  HOST_ARCHITECTURE := x86-64
endif
ifeq ($(findstring x86_64,$(UNAME_M)),x86_64)
  HOST_ARCHITECTURE := x86-64
endif
ifeq ($(findstring arm,$(UNAME_M)),arm)
  HOST_ARCHITECTURE := arm
endif

ifeq ($(PLATFORM),windows-cygwin)
  include mk/platforms/windows-cygwin.mk
endif
ifeq ($(PLATFORM),windows-mingw)
  include mk/platforms/windows-mingw.mk
endif
ifeq ($(PLATFORM),macosx)
  include mk/platforms/macosx.mk
endif
ifeq ($(PLATFORM),linux)
  include mk/platforms/linux.mk
endif
