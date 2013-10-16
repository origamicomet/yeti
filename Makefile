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
# Configuration:                                                               #
#  * Version                                                                   #
#  * Include and library directories, etc.                                     #
#  * Debugging and optimization                                                #
################################################################################

# See ./configure
include config.mk

include mk/varargs.mk
include mk/toolchain.mk
include mk/platform.mk

################################################################################
# Version:                                                                     #
################################################################################

COMMIT   := $(shell git log --pretty=oneline | wc -l)
REVISION := $(shell git rev-parse HEAD)

################################################################################
# Include and library directories, etc.:                                       #
################################################################################

BIN_DIR := build/bin
LIB_DIR := build/lib
OBJ_DIR := build/obj
SRC_DIR := src

################################################################################

INCLUDES := $(call cc,dir,include)

################################################################################
# Debugging and optimization:                                                  #
################################################################################

ifeq ($(PARANOID),yes)
  CFLAGS   += $(call cc,define,BT_PARANOID)
  CXXFLAGS += $(call cxx,define,BT_PARANOID)
endif

ifeq ($(DEBUG),yes)
  CFLAGS   += $(call cc,debug) $(call cc,define,BT_DEBUG)  $(call cc,define,_DEBUG)
  CXXFLAGS += $(call cxx,debug) $(call cxx,define,BT_DEBUG) $(call cxx,define,_DEBUG)
else
  CFLAGS   += $(call cc,define,_NDEBUG)
  CXXFLAGS += $(call cc,define,_NDEBUG)
endif

ifeq ($(DEVELOPMENT),yes)
  CFLAGS   += $(call cc,define,BT_DEVELOPMENT)
  CXXFLAGS += $(call cxx,define,BT_DEVELOPMENT)
  ifneq ($(DEBUG),yes)
    CFLAGS   += $(call cc,development)
    CXXFLAGS += $(call cxx,development)
    LDFLAGS  += $(call ld,development)
  endif
endif

ifeq ($(RELEASE),yes)
  CFLAGS   += $(call cc,define,BT_RELEASE)
  CXXFLAGS += $(call cxx,define,BT_RELEASE)
  ifneq ($(DEVELOPMENT),yes)
    CFLAGS   += $(call cc,release)
    CXXFLAGS += $(call cxx,release)
    LDFLAGS  += $(call ld,release)
  endif
endif

################################################################################
# Rules:                                                                       #
#  * General                                                                   #
#  * Engine                                                                    #
################################################################################

################################################################################
# General                                                                      #
################################################################################

BUTANE := $(BIN_DIR)/$(EXECUTABLE_PREFIX)butane$(EXECUTABLE_SUFFIX)

.PHONY: all docs install clean

all: $(BUTANE)

docs:
	@echo "[DOXYGEN] docs/doxygen.conf"
	@doxygen docs/doxygen.conf

clean:
	@rm -R -f build
	@rm -R -f docs/html

################################################################################
# Engine                                                                       #
################################################################################

SOURCES := $(shell find $(SRC_DIR) -name '*.cc')
OBJECTS := $(addprefix $(OBJ_DIR)/, $(subst $(SRC_DIR)/,,$(SOURCES:%.cc=%$(OBJECT_SUFFIX))))
DEFINES := $(call cc,define,BT_BUILD="$(COMMIT) ($(REVISION))") $(call cc,define,BT_COMPILING)

-include $(OBJECTS:%$(OBJECT_SUFFIX)=%.d)
$(OBJ_DIR)/%$(OBJECT_SUFFIX): $(SRC_DIR)/%.cc
	@echo "[CXX] $<"
	@mkdir -p ${@D}
	@$(call cxx) $(INCLUDES) $(DEFINES) $(call cc,output,$@) $(call cc,input,$<)
	@$(call cxx,deps,$@) $(INCLUDES) $(DEFINES) $(call cc,input,$<) > $(patsubst %$(OBJECT_SUFFIX),%.d,$@)

$(BUTANE): $(OBJECTS)
	@echo "[LD] $@"
	@mkdir -p ${@D}
	@$(call ldxx) $(call ld,output,$@) $(call ld,input,$^) $(call ld,input,$(DEPENDENCIES))
