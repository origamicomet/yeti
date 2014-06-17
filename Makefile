#===-- Makefile ------------------------------------------------------------===#
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

-include config.mk
ifndef _BITBYTE_BUTANE_BUILD_CONFIG_
  $(error Please ./configure first.)
endif

include mk/detect/platform.mk
include mk/detect/architecture.mk
include mk/toolchain.mk
include mk/platform.mk
include mk/architecture.mk

.PHONY: all docs clean butane

all: butane

include src/Makefile

butane: $(BUTANE)

docs:
	@echo "[DOXYGEN] Doxyfile"
	@doxygen Doxyfile

clean:
	@echo "Cleaning..."
	@rm -R -f bin
	@rm -R -f lib
	@rm -R -f obj
	@rm -R -f documentation/html
