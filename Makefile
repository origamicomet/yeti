#===-- Makefile ---------------------------------------------*- Makefile -*-===#
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

-include conf.mk
ifndef _BITBYTE_BUTANE_BUILD_CONFIG_
  $(error Please ./configure first.)
endif

include build/detect/platform.mk
include build/detect/architecture.mk
include build/toolchain.mk
include build/platform.mk
include build/architecture.mk

.PHONY: all docs clean butane foundation

all: butane

include src/Makefile

foundation:
	cd deps/foundation; make all

butane: $(BUTANE)

docs:
	@echo "[DOXYGEN] Doxyfile"
	@doxygen Doxyfile

clean:
	@echo "Cleaning..."
	@rm -R -f bin
	@rm -R -f lib
	@rm -R -f obj
	@rm -R -f docs/html
	@cd deps/foundation; make clean

#===------------------------------------------------------------------------===#
