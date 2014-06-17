#===-- src/bitbyte/Makefile ------------------------------------------------===#
#
#  Butane
#
#  This file is distributed under the terms described in LICENSE.
#
#  Author(s):
#
#    * Michael Williams <mwilliams@bitbyte.ca>
#
#===------------------------------------------------------------------------===#

## \file src/bitbyte/Makefile
## \brief ...
##

#==============================================================================#

BITBYTE_BUTANE_CONFIGURATION := $(CONFIGURATION)
BITBYTE_BUTANE_LINKAGE       := $(LINKAGE)
BITBYTE_BUTANE_CFLAGS        :=
BITBYTE_BUTANE_LDFLAGS       := $(call ld-libraries,bin) $(call ld-libraries,lib) \
                                $(call ld-libraries,deps/foundation/bin) $(call ld-libraries,deps/foundation/lib)
BITBYTE_BUTANE_ARFLAGS       :=

ifeq ($(BITBYTE_BUTANE_LINKAGE),static)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_BUTANE_LINKAGE=BITBYTE_BUTANE_STATIC_LINKAGE)
endif
ifeq ($(BITBYTE_BUTANE_LINKAGE),dynamic)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_BUTANE_LINKAGE=BITBYTE_BUTANE_DYNAMIC_LINKAGE)
endif

ifeq ($(BITBYTE_BUTANE_CONFIGURATION),debug)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_BUTANE_CONFIGURATION=BITBYTE_BUTANE_CONFIGURATION_DEBUG)
  BITBYTE_BUTANE_CFLAGS += $(call cc-debug)
  BITBYTE_BUTANE_LDFLAGS += $(call ld-debug)
  BITBYTE_BUTANE_ARFLAGS += $(call ar-debug)
endif
ifeq ($(BITBYTE_BUTANE_CONFIGURATION),development)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_BUTANE_CONFIGURATION=BITBYTE_BUTANE_CONFIGURATION_DEVELOPMENT)
  BITBYTE_BUTANE_CFLAGS += $(call cc-development)
  BITBYTE_BUTANE_LDFLAGS += $(call ld-development)
  BITBYTE_BUTANE_ARFLAGS += $(call ar-development)
endif
ifeq ($(BITBYTE_BUTANE_CONFIGURATION),release)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_BUTANE_CONFIGURATION=BITBYTE_BUTANE_CONFIGURATION_RELEASE)
  BITBYTE_BUTANE_CFLAGS += $(call cc-release)
  BITBYTE_BUTANE_LDFLAGS += $(call ld-release)
  BITBYTE_BUTANE_ARFLAGS += $(call ar-release)
endif

#===------------------------------------------------------------------------===#

ifeq ($(BITBYTE_BUTANE_LINKAGE),static)
  BUTANE := lib/$(STATIC_LIB_PREFIX)butane$(STATIC_LIB_SUFFIX)$(STATIC_LIB_EXTENSION)
endif
ifeq ($(BITBYTE_BUTANE_LINKAGE),dynamic)
  BUTANE := bin/$(SHARED_LIB_PREFIX)butane$(SHARED_LIB_SUFFIX)$(SHARED_LIB_EXTENSION)
endif

FOUNDATION_TIER0_IMPORT_LIB := $(STATIC_LIB_PREFIX)tier0$(STATIC_LIB_SUFFIX)
FOUNDATION_TIER1_IMPORT_LIB := $(STATIC_LIB_PREFIX)tier1$(STATIC_LIB_SUFFIX)
FOUNDATION_TIER2_IMPORT_LIB := $(STATIC_LIB_PREFIX)tier2$(STATIC_LIB_SUFFIX)

BITBYTE_BUTANE_INCLUDES := $(call cc-includes,include)
BITBYTE_BUTANE_DEPENDENCIES := $(call ld-link,$(FOUNDATION_TIER0_IMPORT_LIB)) \
                               $(call ld-link,$(FOUNDATION_TIER1_IMPORT_LIB)) \
                               $(call ld-link,$(FOUNDATION_TIER2_IMPORT_LIB))

ifeq ($(TOOLCHAIN),msvc)
  BITBYTE_FOUNDATION_TIER2_DEPENDENCIES += $(call ld-link,dbghelp)
endif

BITBYTE_BUTANE_SOURCES := $(shell find src/bitbyte/butane -name '*.c')
BITBYTE_BUTANE_OBJECTS := $(addprefix obj/, $(subst src/,,$(BITBYTE_BUTANE_SOURCES:%.c=%.o)))
BITBYTE_BUTANE_OBJECTS += obj/bitbyte/butane.o

BITBYTE_BUTANE_DEFINES := $(call cc-define,BITBYTE_BUTANE_COMPILING)

-include $(BITBYTE_BUTANE_OBJECTS:%.o=%.d)

obj/bitbyte/butane/%.o: src/bitbyte/butane/%.c
	@echo "[CC] $<"
	@mkdir -p ${@D}
	$(call c++) $(BITBYTE_BUTANE_INCLUDES) $(BITBYTE_BUTANE_DEFINES) $(BITBYTE_BUTANE_CFLAGS) $(call cc-input,$<) $(call cc-output,$@)
	$(call c++) $(BITBYTE_BUTANE_INCLUDES) $(BITBYTE_BUTANE_DEFINES) $(BITBYTE_BUTANE_CFLAGS) $(call cc-input,$<) -MM -MT $@ >$(patsubst %.o,%.d,$@)

obj/bitbyte/butane.o: src/bitbyte/butane.c
	@echo "[CC] $<"
	@mkdir -p ${@D}
	$(call c++) $(BITBYTE_BUTANE_INCLUDES) $(BITBYTE_BUTANE_DEFINES) $(BITBYTE_BUTANE_CFLAGS) $(call cc-input,$<) $(call cc-output,$@)
	$(call c++) $(BITBYTE_BUTANE_INCLUDES) $(BITBYTE_BUTANE_DEFINES) $(BITBYTE_BUTANE_CFLAGS) $(call cc-input,$<) -MM -MT $@ >$(patsubst %.o,%.d,$@)

$(BUTANE): $(BITBYTE_BUTANE_OBJECTS)
	@echo "[LD] $@"
	@mkdir -p ${@D}
ifeq ($(BITBYTE_BUTANE_LINKAGE),static)
	$(call ar++) $(BITBYTE_BUTANE_ARFLAGS) $(call ar-output,$@) $(foreach input,$^,$(call ar-input,$(input)))
endif
ifeq ($(BITBYTE_BUTANE_LINKAGE),dynamic)
	$(call ld++) $(call ld-shared) $(BITBYTE_BUTANE_LDFLAGS) $(call ld-output,$@) $(foreach input,$^,$(call ld-input,$(input))) $(BITBYTE_BUTANE_DEPENDENCIES)
endif

#==============================================================================#
