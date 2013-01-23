# Project: libDarnit
include config.mk
MAKEFLAGS	+=	--no-print-directory

TOPDIR		=	$(shell pwd)
export TOPDIR

.PHONY: all install clean

all:
	@echo " [INIT] bin/"
	@$(MKDIR) bin/
	@echo " [ CD ] deps/"
	+@make -C deps/
	@echo " [ CD ] darnit/"
	+@make -C darnit/
	@echo " [ CD ] tools/"
	+@make -C tools/
	@echo "Build complete."
	@echo 
	
clean:
	@echo " [ RM ] bin/"
	+@$(RM) bin/
	@echo " [ CD ] deps/"
	+@make -C deps/ clean
	@echo " [ CD ] darnit/"
	+@make -C darnit/ clean
	@echo " [ CD ] tools/"
	+@make -C tools/ clean
	@echo
	@echo "Source tree cleaned."
	@echo
	
install: 
	@echo " [INST] include/darnit"
	@$(MKDIR) $(PREFIX)/include/darnit
	@install -D -m 0755 include/darnit/* $(PREFIX)/include/darnit
	@echo " [INST] libdarnit.so"
	@install -D -m 0755 $(LIB) $(PREFIX)/lib
	-@ldconfig
	@echo " [INST] tools"
	@install -D -m 0755 bin/darnit-* $(PREFIX)/bin
	@echo
	@echo "libDarnit installed."
	@echo 
