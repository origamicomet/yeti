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
#  * Flags, includes, dependencies, directories, etc.                          #
#  * Compiler, architecture, and platform auto-detection                       #
#  * Prefixes and suffixes                                                     #
#  * Debugging                                                                 #
#  * Linking                                                                   #
################################################################################

# See ./configure
-include config.mk

################################################################################
# Flags, includes, dependencies, directories, etc.                             #
################################################################################

CFLAGS   := -std=c99 -pedantic -Wall -Wextra -Wfloat-equal -Wshadow \
            -Wunsafe-loop-optimizations -Wpointer-arith -Wcast-qual \
            -Wcast-align -Wmissing-field-initializers -Wpacked \
            -Wpadded -Wredundant-decls -Wunreachable-code -Winline
CXXFLAGS := -std=c++98 -pedantic -Wall -Wextra -Wfloat-equal -Wshadow \
            -Wunsafe-loop-optimizations -Wpointer-arith -Wcast-qual \
            -Wcast-align -Wmissing-field-initializers -Wpacked \
            -Wpadded -Wredundant-decls -Wunreachable-code -Winline
INCLUDES := -Iinclude

################################################################################

LDFLAGS      :=
DEPENDENCIES :=

################################################################################

BIN_DIR := bin
LIB_DIR := lib
OBJ_DIR := obj
SRC_DIR := src

COMMIT   := $(shell git log --pretty=oneline | wc -l)
REVISION := $(shell git rev-parse HEAD)

################################################################################
# Compiler, architecture, and platform auto-detection                          #
################################################################################

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

# Determine the current platform based on `uname -s`:
# http://en.wikipedia.org/wiki/Uname is immensely helpful here.
ifeq ($(findstring CYGWIN_NT,$(UNAME_S)),CYGWIN_NT)
  PLATFORM := windows-cygwin
endif
ifeq ($(findstring MINGW32_NT,$(UNAME_S)),MINGW32_NT)
  PLATFORM := windows-mingw
endif
ifeq ($(findstring Darwin,$(UNAME_S)),Darwin)
  PLATFORM := macosx
endif
ifeq ($(findstring Linux,$(UNAME_S)),Linux)
  PLATFORM := linux
endif

# Determine the current architecture based on `uname -m`:
# Again, http://en.wikipedia.org/wiki/Uname is immensely helpful here.
ifeq ($(findstring i386,$(UNAME_M)),i386)
  ARCHITECTURE := x86
endif
ifeq ($(findstring i686,$(UNAME_M)),i686)
  ARCHITECTURE := x86
endif
ifeq ($(findstring i686-64,$(UNAME_M)),i686-64)
  ARCHITECTURE := x86-64
endif
ifeq ($(findstring amd64,$(UNAME_M)),amd64)
  ARCHITECTURE := x86-64
endif
ifeq ($(findstring x86_64,$(UNAME_M)),x86_64)
  ARCHITECTURE := x86-64
endif

# Default to the current platform if no target is specified:
ifndef TARGET_PLATFORM
  TARGET_PLATFORM := $(PLATFORM)
endif

# Make sure the target platform is supported:
ifeq ($(TARGET_PLATFORM),windows-cygwin)
  TARGET_PLATFORM_IS_SUPPORTED := no
endif
ifeq ($(TARGET_PLATFORM),windows-mingw)
  TARGET_PLATFORM_IS_SUPPORTED := yes
endif
ifeq ($(TARGET_PLATFORM),macosx)
  TARGET_PLATFORM_IS_SUPPORTED := no
endif
ifeq ($(TARGET_PLATFORM),linux)
  TARGET_PLATFORM_IS_SUPPORTED := yes
endif
ifeq ($(TARGET_PLATFORM),android)
  TARGET_PLATFORM_IS_SUPPORTED := no
endif
ifeq ($(TARGET_PLATFORM),ios)
  TARGET_PLATFORM_IS_SUPPORTED := no
endif
ifneq ($(TARGET_PLATFORM_IS_SUPPORTED),yes)
  $(error Compilation support for '$(TARGET_PLATFORM)' isn't available right now.)
endif

# Cross-compilation support is too much effort for now...
ifneq ($(PLATFORM),$(TARGET_PLATFORM))
  $(error Cross-compilation support isn't available right now.)
endif

# If CC or CXX is exported then use it instead:
ifdef CC
  ifeq ($(CC),g++)
    CC := gcc
  endif
  ifeq ($(CC),clang++)
    CC := clang
  endif
else
  ifeq ($(TARGET_PLATFORM),windows-cygwin)
    $(error Compilation support for '$(TARGET_PLATFORM)' isn't available right now.)
  endif
  ifeq ($(TARGET_PLATFORM),windows-mingw)
    CC := gcc
  endif
  ifeq ($(TARGET_PLATFORM),macosx)
    CC := clang
  endif
  ifeq ($(TARGET_PLATFORM),linux)
    CC := gcc
  endif
  ifeq ($(TARGET_PLATFORM),android)
    $(error Compilation support for '$(TARGET_PLATFORM)' isn't available right now.)
  endif
  ifeq ($(TARGET_PLATFORM),ios)
    $(error Compilation support for '$(TARGET_PLATFORM)' isn't available right now.)
  endif
endif
ifdef CXX
  ifeq ($(CXX),gcc)
    CXX := g++
  endif
  ifeq ($(CXX),clang)
    CXX := clang++
  endif
else
  ifeq ($(TARGET_PLATFORM),windows-cygwin)
    $(error Compilation support for '$(TARGET_PLATFORM)' isn't available right now.)
  endif
  ifeq ($(TARGET_PLATFORM),windows-mingw)
    CC := g++
  endif
  ifeq ($(TARGET_PLATFORM),macosx)
    CC := clang++
  endif
  ifeq ($(TARGET_PLATFORM),linux)
    CC := g++
  endif
  ifeq ($(TARGET_PLATFORM),android)
    $(error Compilation support for '$(TARGET_PLATFORM)' isn't available right now.)
  endif
  ifeq ($(TARGET_PLATFORM),ios)
    $(error Compilation support for '$(TARGET_PLATFORM)' isn't available right now.)
  endif
endif

################################################################################
# Prefixes and suffixes                                                        #
################################################################################

ifeq ($(TARGET_PLATFORM),windows-cygwin)
  EXECUTABLE_PREFIX :=
  EXECUTABLE_SUFFIX := .exe
  STATIC_LIB_PREFIX := lib
  STATIC_LIB_SUFFIX := .a
  SHARED_LIB_PREFIX := lib
  SHARED_LIB_SUFFIX := .dll
endif
ifeq ($(TARGET_PLATFORM),windows-mingw)
  EXECUTABLE_PREFIX :=
  EXECUTABLE_SUFFIX := .exe
  STATIC_LIB_PREFIX := lib
  STATIC_LIB_SUFFIX := .a
  SHARED_LIB_PREFIX := lib
  SHARED_LIB_SUFFIX := .dll
endif
ifeq ($(TARGET_PLATFORM),macosx)
  EXECUTABLE_PREFIX :=
  EXECUTABLE_SUFFIX :=
  STATIC_LIB_PREFIX := lib
  STATIC_LIB_SUFFIX := .a
  SHARED_LIB_PREFIX := lib
  SHARED_LIB_SUFFIX := .dylib
endif
ifeq ($(TARGET_PLATFORM),linux)
  EXECUTABLE_PREFIX :=
  EXECUTABLE_SUFFIX :=
  STATIC_LIB_PREFIX := lib
  STATIC_LIB_SUFFIX := .a
  SHARED_LIB_PREFIX := lib
  SHARED_LIB_SUFFIX := .so
endif
ifeq ($(TARGET_PLATFORM),android)
  $(error Compilation support for '$(TARGET_PLATFORM)' isn't available right now.)
endif
ifeq ($(TARGET_PLATFORM),ios)
  $(error Compilation support for '$(TARGET_PLATFORM)' isn't available right now.)
endif

################################################################################
# Debugging                                                                    #
################################################################################

ifeq ($(DEBUG),yes)
  CFLAGS += -g -DBT_DEBUG -D_DEBUG
  CXXFLAGS += -g -DBT_DEBUG -D_DEBUG
  ifeq ($(PARANOID),yes)
    CFLAGS += -DBT_PARANOID
    CXXFLAGS += -DBT_PARANOID
  endif
endif

ifneq ($(DEBUG),yes)
  CFLAGS += -O3 -DBT_RELEASE -DNDEBUG
  CXXFLAGS += -O3 -DBT_RELEASE -DNDEBUG
endif

################################################################################
# Linking                                                                      #
################################################################################

ifeq ($(STATICALLY_LINKING),yes)
  CFLAGS += -DBT_STATICALLY_LINKING
  CXXFLAGS += -DBT_STATICALLY_LINKING
endif

ifeq ($(TARGET_PLATFORM),windows-mingw)
  DEPENDENCIES := -lkernel32 -luser32
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
	@rm -R -f $(BIN_DIR)
	@rm -R -f $(LIB_DIR)
	@rm -R -f $(OBJ_DIR)
	@rm -R -f docs/html

################################################################################
# Engine                                                                       #
################################################################################

SOURCES := $(shell find $(SRC_DIR) -name '*.cc')
OBJECTS := $(addprefix $(OBJ_DIR)/, $(subst $(SRC_DIR)/,,$(SOURCES:%.cc=%.o)))

-include $(OBJECTS:.o=.d)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@echo "[CXX] $<"
	@mkdir -p ${@D}
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -DBT_BUILD=$(COMMIT) -DBT_COMPILING -c $< -o $@
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -DBT_BUILD=$(COMMIT) -DBT_COMPILING -MM -MT $@ -c $< > $(patsubst %.o,%.d,$@)

$(BUTANE): $(OBJECTS)
	@echo "[LD] $@"
	@mkdir -p ${@D}
	@$(CXX) $(LDFLAGS) -o $@ $^ $(DEPENDENCIES)
