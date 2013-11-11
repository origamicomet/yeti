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
  CFLAGS  += $(call cc-define,BUTANE_CONFIGURATION=1) $(call cc-debug)
  LDFLAGS += $(call ld-debug)
  ARFLAGS += $(call ar-debug)
endif
ifeq ($(CONFIGURATION),development)
  CFLAGS  += $(call cc-define,BUTANE_CONFIGURATION=2) $(call cc-development)
  LDFLAGS += $(call ld-development)
  ARFLAGS += $(call ar-development)
endif
ifeq ($(CONFIGURATION),release)
  CFLAGS  += $(call cc-define,BUTANE_CONFIGURATION=3) $(call cc-release)
  LDFLAGS += $(call ld-release)
  ARFLAGS += $(call ar-release)
endif

################################################################################
# Rules:                                                                       #
################################################################################

FOUNDATION := $(FOUNDATION_PATH)/lib/$(STATIC_LIB_PREFIX)foundation$(STATIC_LIB_SUFFIX)$(STATIC_LIB_EXTENSION)

ifeq ($(LINKAGE),static)
  BUTANE := $(LIB_DIR)/$(STATIC_LIB_PREFIX)butane$(STATIC_LIB_SUFFIX)$(STATIC_LIB_EXTENSION)
endif
ifeq ($(LINKAGE),dynamic)
  BUTANE := $(BIN_DIR)/$(SHARED_LIB_PREFIX)butane$(SHARED_LIB_SUFFIX)$(SHARED_LIB_EXTENSION)
endif

.PHONY: all docs clean \
  $(FOUNDATION)

all: $(BUTANE)

docs:
	@echo "[DOXYGEN] Doxyfile"
	@doxygen Doxyfile

clean:
	@echo "Cleaning..."
	@rm -R -f $(BIN_DIR)
	@rm -R -f $(LIB_DIR)
	@rm -R -f $(OBJ_DIR)
	@rm -R -f docs/html
	cd $(FOUNDATION_PATH); make clean

SOURCES      := $(shell find $(SRC_DIR) -name '*.c')
OBJECTS      := $(addprefix $(OBJ_DIR)/, $(subst $(SRC_DIR)/,,$(SOURCES:%.c=%.o)))

INCLUDES     := $(call cc-includes,include) $(call cc-includes,$(FOUNDATION_PATH)/include)
DEPENDENCIES := $(call ld-libraries,$(FOUNDATION_PATH)/lib) $(call ld-link,foundation)

DEFINES      := $(call cc-define,BUTANE_COMPILING)
DEFINES      := $(call cc-define-str,BUTANE_BUILD,$(BUILD))
DEFINES      += $(call cc-define,FND_LINKAGE=1)
ifeq ($(LINKAGE),static)
  DEFINES    += $(call cc-define,BUTANE_LINKAGE=1)
endif
ifeq ($(LINKAGE),dynamic)
  DEFINES    += $(call cc-define,BUTANE_LINKAGE=2)
endif

-include $(OBJECTS:%.o=%.d)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "[CXX] $<"
	@mkdir -p ${@D}
	@$(call c++) $(INCLUDES) $(DEFINES) $(call cc-input,$<) $(call cc-output,$@)
	@$(call c++) $(INCLUDES) $(DEFINES) $(call cc-input,$<) -MM -MT $@ >$(patsubst %.o,%.d,$@)

$(FOUNDATION):
	cd $(FOUNDATION_PATH); make all

$(BUTANE): $(FOUNDATION) $(OBJECTS)
	@echo "[LD] $@"
	@mkdir -p ${@D}
ifeq ($(LINKAGE),static)
	@$(call ar++) $(call ld-output,$@) $(foreach input,$(OBJECTS),$(call ar-input,$(input)))
endif
ifeq ($(LINKAGE),dynamic)
	@$(call ld++) $(call ld-shared) $(call ld-output,$@) $(foreach input,$(OBJECTS),$(call ld-input,$(input))) $(DEPENDENCIES)
endif
