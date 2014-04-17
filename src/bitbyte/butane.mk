#===-- bitbyte/butane.mk ------------------------------------*- Makefile -*-===#
#
#  Butane, a data-driven game engine.
#
#  This file is distributed under the terms described in LICENSE.
#
#  Author(s):
#
#    * Michael Williams <mwilliams@bitbyte.ca>
#
#===------------------------------------------------------------------------===#

BITBYTE_BUTANE_CONFIGURATION := $(CONFIGURATION)
BITBYTE_BUTANE_LINKAGE := $(LINKAGE)

BITBYTE_BUTANE_CFLAGS  :=
BITBYTE_BUTANE_LDFLAGS := $(call ld-libraries,bin) $(call ld-libraries,lib)
BITBYTE_BUTANE_ARFLAGS :=

ifeq ($(BITBYTE_BUTANE_LINKAGE),statically)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_BUTANE_LINKAGE=1)
endif
ifeq ($(BITBYTE_BUTANE_LINKAGE),dynamically)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_BUTANE_LINKAGE=2)
endif

ifeq ($(BITBYTE_BUTANE_CONFIGURATION),debug)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_BUTANE_CONFIGURATION=1)
  BITBYTE_BUTANE_CFLAGS += $(call cc-debug)
  BITBYTE_BUTANE_LDFLAGS += $(call ld-debug)
  BITBYTE_BUTANE_ARFLAGS += $(call ar-debug)
endif
ifeq ($(BITBYTE_BUTANE_CONFIGURATION),development)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_BUTANE_CONFIGURATION=2)
  BITBYTE_BUTANE_CFLAGS += $(call cc-development)
  BITBYTE_BUTANE_LDFLAGS += $(call ld-development)
  BITBYTE_BUTANE_ARFLAGS += $(call ar-development)
endif
ifeq ($(BITBYTE_BUTANE_CONFIGURATION),release)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_BUTANE_CONFIGURATION=3)
  BITBYTE_BUTANE_CFLAGS += $(call cc-release)
  BITBYTE_BUTANE_LDFLAGS += $(call ld-release)
  BITBYTE_BUTANE_ARFLAGS += $(call ar-release)
endif

#===------------------------------------------------------------------------===#

ifeq ($(BITBYTE_BUTANE_LINKAGE),statically)
  BUTANE := lib/$(STATIC_LIB_PREFIX)butane$(STATIC_LIB_SUFFIX)$(STATIC_LIB_EXTENSION)
endif
ifeq ($(BITBYTE_BUTANE_LINKAGE),dynamically)
  BUTANE := bin/$(SHARED_LIB_PREFIX)butane$(SHARED_LIB_SUFFIX)$(SHARED_LIB_EXTENSION)
endif

ifeq ($(TOOLCHAIN),msvc)
  BUTANE_IMPORT_LIB := $(STATIC_LIB_PREFIX)butane$(STATIC_LIB_SUFFIX)
else
  BUTANE_IMPORT_LIB := $(SHARED_LIB_PREFIX)butane$(SHARED_LIB_SUFFIX)
endif

BITBYTE_BUTANE_INCLUDES := $(call cc-includes,include)
BITBYTE_BUTANE_DEPENDENCIES :=

BITBYTE_BUTANE_SOURCES := $(shell find src/bitbyte/butane -name '*.cc')
BITBYTE_BUTANE_OBJECTS := $(addprefix obj/, $(subst src/,,$(BITBYTE_BUTANE_SOURCES:%.cc=%.o)))
BITBYTE_BUTANE_OBJECTS += obj/bitbyte/butane.o

BITBYTE_BUTANE_DEFINES := $(call cc-define,BITBYTE_BUTANE_COMPILING)

include src/bitbyte/butane/deps.mk

-include $(BITBYTE_BUTANE_OBJECTS:%.o=%.d)
obj/bitbyte/butane/%.o: src/bitbyte/butane/%.cc
	@echo "[CC] $<"
	@mkdir -p ${@D}
	$(call c++) $(BITBYTE_BUTANE_INCLUDES) $(BITBYTE_BUTANE_DEFINES) $(BITBYTE_BUTANE_CFLAGS) $(call cc-input,$<) $(call cc-output,$@)
	$(call c++) $(BITBYTE_BUTANE_INCLUDES) $(BITBYTE_BUTANE_DEFINES) $(BITBYTE_BUTANE_CFLAGS) $(call cc-input,$<) -MM -MT $@ >$(patsubst %.o,%.d,$@)

obj/bitbyte/butane.o: src/bitbyte/butane.cc
	@echo "[CC] $<"
	@mkdir -p ${@D}
	$(call c++) $(BITBYTE_BUTANE_INCLUDES) $(BITBYTE_BUTANE_DEFINES) $(BITBYTE_BUTANE_CFLAGS) $(call cc-input,$<) $(call cc-output,$@)
	$(call c++) $(BITBYTE_BUTANE_INCLUDES) $(BITBYTE_BUTANE_DEFINES) $(BITBYTE_BUTANE_CFLAGS) $(call cc-input,$<) -MM -MT $@ >$(patsubst %.o,%.d,$@)

$(BUTANE): $(BITBYTE_BUTANE_OBJECTS) foundation
	@echo "[LD] $@"
	@mkdir -p ${@D}
ifeq ($(BITBYTE_BUTANE_LINKAGE),statically)
	$(call ar++) $(BITBYTE_BUTANE_ARFLAGS) $(call ar-output,$@) $(foreach input,$(BITBYTE_BUTANE_OBJECTS),$(call ar-input,$(input)))
endif
ifeq ($(BITBYTE_BUTANE_LINKAGE),dynamically)
	$(call ld++) $(call ld-shared) $(BITBYTE_BUTANE_LDFLAGS) $(call ld-output,$@) $(foreach input,$(BITBYTE_BUTANE_OBJECTS),$(call ld-input,$(input))) $(BITBYTE_BUTANE_DEPENDENCIES)
endif

#===------------------------------------------------------------------------===#
