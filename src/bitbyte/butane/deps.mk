#===-- bitbyte/butane/deps.mk -------------------------------*- Makefile -*-===#
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

BITBYTE_FOUNDATION_CONFIGURATION := $(BITBYTE_BUTANE_CONFIGURATION)
BITBYTE_FOUNDATION_LINKAGE := statically

BITBYTE_BUTANE_LDFLAGS += $(call ld-libraries,deps/foundation/bin)
BITBYTE_BUTANE_LDFLAGS += $(call ld-libraries,deps/foundation/lib)

ifeq ($(HOST_ARCHITECTURE),x86)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_HOST_ARCHITECTURE=0)
endif
ifeq ($(HOST_ARCHITECTURE),x86-64)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_HOST_ARCHITECTURE=1)
endif
ifeq ($(HOST_PLATFORM),windows-mingw)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_HOST_PLATFORM=0)
endif
ifeq ($(HOST_PLATFORM),macosx)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_HOST_PLATFORM=1)
endif
ifeq ($(HOST_PLATFORM),linux)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_HOST_PLATFORM=2)
endif

BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_TIER0_LINKAGE=1)
BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_TIER1_LINKAGE=1)
BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_TIER2_LINKAGE=1)
BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_TIER3_LINKAGE=1)

ifeq ($(BITBYTE_FOUNDATION_CONFIGURATION),debug)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_CONFIGURATION=1)
endif
ifeq ($(BITBYTE_FOUNDATION_CONFIGURATION),development)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_CONFIGURATION=2)
endif
ifeq ($(BITBYTE_FOUNDATION_CONFIGURATION),release)
  BITBYTE_BUTANE_CFLAGS += $(call cc-define,BITBYTE_FOUNDATION_CONFIGURATION=3)
endif

BITBYTE_BUTANE_INCLUDES += $(call cc-includes,deps/foundation/include)
BITBYTE_BUTANE_DEPENDENCIES += $(call ld-link,$(STATIC_LIB_PREFIX)tier0$(STATIC_LIB_SUFFIX))
BITBYTE_BUTANE_DEPENDENCIES += $(call ld-link,$(STATIC_LIB_PREFIX)tier1$(STATIC_LIB_SUFFIX))
BITBYTE_BUTANE_DEPENDENCIES += $(call ld-link,$(STATIC_LIB_PREFIX)tier2$(STATIC_LIB_SUFFIX))
BITBYTE_BUTANE_DEPENDENCIES += $(call ld-link,$(STATIC_LIB_PREFIX)tier3$(STATIC_LIB_SUFFIX))

#===------------------------------------------------------------------------===#
