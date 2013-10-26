################################################################################
##                                                                            ##
## This file is part of Butane.                                               ##
##                                                                            ##
## Author(s):                                                                 ##
##                                                                            ##
##   Michael Williams <devbug@bitbyte.ca>                                     ##
##                                                                            ##
## See LICENSE.md for licensing information.                                  ##
##                                                                            ##
################################################################################
## Makefile                                                                   ##
##  Specifies all build rules for Butane.                                     ##
################################################################################

-include build/config
ifndef _BUTANE_BUILD_CONFIG_
  $(error Please ./configure first.)
endif

include build/toolchain.mk
include build/platform.mk
include build/architecture.mk
include build/detect/platform.mk
include build/detect/architecture.mk

################################################################################
# Version:                                                                     #
################################################################################

COMMIT   := $(shell git log --pretty=oneline | wc -l)
REVISION := $(shell git rev-parse HEAD)
BUILD    := $(COMMIT) ($(REVISION))

################################################################################
# Binary, library, object, and source directories:                             #
################################################################################

BIN_DIR := bin
LIB_DIR := lib
OBJ_DIR := obj
SRC_DIR := src

################################################################################
# Debugging and optimization:                                                  #
################################################################################

ifeq ($(CONFIGURATION),debug)
  CFLAGS  += $(call cc-define,BT_CONFIGURATION=1) $(call cc-debug)
  LDFLAGS += $(call ld-debug)
  ARFLAGS += $(call ar-debug)
endif
ifeq ($(CONFIGURATION),development)
  CFLAGS  += $(call cc-define,BT_CONFIGURATION=2) $(call cc-development)
  LDFLAGS += $(call ld-development)
  ARFLAGS += $(call ar-development)
endif
ifeq ($(CONFIGURATION),release)
  CFLAGS  += $(call cc-define,BT_CONFIGURATION=3) $(call cc-release)
  LDFLAGS += $(call ld-release)
  ARFLAGS += $(call ar-release)
endif

################################################################################
# Rules:                                                                       #
################################################################################

FOUNDATION := deps/foundation/lib/$(STATIC_LIB_PREFIX)foundation$(STATIC_LIB_SUFFIX)$(STATIC_LIB_EXTENSION)
BUTANE     := $(BIN_DIR)/$(EXECUTABLE_PREFIX)butane$(EXECUTABLE_SUFFIX)$(EXECUTABLE_EXTENSION)

.PHONY: all docs clean \
	$(FOUNDATION)

all: $(BUTANE)

docs:
	@echo "[DOXYGEN] docs/doxygen.conf"
	@doxygen docs/doxygen.conf

clean:
	@echo "Cleaning..."
	@rm -R -f $(BIN_DIR)
	@rm -R -f $(LIB_DIR)
	@rm -R -f $(OBJ_DIR)
	@rm -R -f docs/html
	cd deps/foundation; make clean

SOURCES      := $(shell find $(SRC_DIR) -name '*.c')
OBJECTS      := $(addprefix $(OBJ_DIR)/, $(subst $(SRC_DIR)/,,$(SOURCES:%.c=%.o)))
INCLUDES     := $(call cc-includes,include) $(call cc-includes,deps/foundation/include)
DEFINES      := $(call cc-define-str,BT_BUILD,$(BUILD))
DEPENDENCIES := $(call ld-libraries,deps/foundation/lib) $(call ld-link,foundation)

-include $(OBJECTS:%.o=%.d)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "[CXX] $<"
	@mkdir -p ${@D}
	@$(call c++) $(INCLUDES) $(DEFINES) $(call cc-input,$<) $(call cc-output,$@)
	@$(call c++) $(INCLUDES) $(DEFINES) $(call cc-input,$<) -MM -MT $@ >$(patsubst %.o,%.d,$@)

$(FOUNDATION):
	cd deps/foundation; make all

$(BUTANE): $(FOUNDATION) $(OBJECTS)
	@echo "[LD] $@"
	@mkdir -p ${@D}
	@$(call ld++) $(call ld-output,$@) $(foreach input,$(OBJECTS),$(call ld-input,$(input))) $(DEPENDENCIES)
