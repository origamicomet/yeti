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

INCLUDE_DIRS := $(call include_dir,include)
LIB_DIRS     := $(call lib_dir,include)

################################################################################
# Debugging and optimization:                                                  #
################################################################################

ifeq ($(PARANOID),yes)
  CFLAGS   += $(call def,BT_PARANOID)
  CXXFLAGS += $(call def,BT_PARANOID)
endif

ifeq ($(DEBUG),yes)
  CFLAGS   += $(call generate_debug_info)
  CFLAGS   += $(call def,BT_DEBUG) $(call def,_DEBUG)
  CXXFLAGS += $(call generate_debug_info)
  CXXFLAGS += $(call def,BT_DEBUG) $(call def,_DEBUG)
else
  CFLAGS   += $(call def,_NDEBUG)
  CXXFLAGS += $(call def,_NDEBUG)
endif

ifeq ($(DEVELOPMENT),yes)
  CFLAGS   += $(call def,BT_DEVELOPMENT)
  CXXFLAGS += $(call def,BT_DEVELOPMENT)
  OPTIMIZE := yes
endif

ifeq ($(RELEASE),yes)
  CFLAGS   += $(call def,BT_RELEASE)
  CXXFLAGS += $(call def,BT_RELEASE)
  OPTIMIZE := yes
endif

ifeq ($(OPTIMIZE),yes)
  CFLAGS   += $(call optimize)
  CXXFLAGS += $(call optimize)
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
OBJECTS := $(addprefix $(OBJ_DIR)/, $(subst $(SRC_DIR)/,,$(SOURCES:%.cc=%.o)))
DEFINES := $(call def,BT_BUILD=$(COMMIT)) $(call def,BT_COMPILING)

-include $(OBJECTS:.o=.d)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@echo "[CXX] $<"
	@mkdir -p ${@D}
	@$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(DEFINES) -c $< -o $@
	@$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(DEFINES) -MM -MT $@ -c $< > $(patsubst %.o,%.d,$@)

$(BUTANE): $(OBJECTS)
	@echo "[LD] $@"
	@mkdir -p ${@D}
	@$(CXX) $(LDFLAGS) $(LIB_DIRS) -o $@ $^ $(DEPENDENCIES)
