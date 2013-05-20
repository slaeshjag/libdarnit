# Project: libDarnit
include config.mk
MAKEFLAGS	+=	--no-print-directory

TOPDIR		=	$(shell pwd)
export TOPDIR

.PHONY: all tools lib install strip clean

all:
	@cp darnit/platform/keysym.h include/darnit/darnit_keysym.h
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

lib:
	@$(MKDIR) bin/
	@echo " [ CD ] deps/"
	+@make -C deps/
	@echo " [ CD ] darnit/"
	+@make -C darnit/
	
tools:
	@$(MKDIR) bin/
	@echo " [ CD ] tools/"
	+@make -C tools/
	
clean:
	@echo " [ RM ] bin/"
	+@$(RM) bin/
	@echo " [ CD ] deps/"
	+@make -C deps/ clean
	@echo " [ CD ] darnit/"
	+@make -C darnit/ clean
	@echo " [ CD ] tools/"
	+@make -C tools/ clean
	@echo " [ CD ] testapp/"
	+@make -C testapp/ clean
	@echo " [ RM ] libdarnit.a"
	@rm -f libdarnit.a
	@echo
	@echo "Source tree cleaned."
	@echo

strip:
	@echo " [STRP] bin/"
	@strip $(LIB)
	@strip bin/darnit-*
	
install: $(INSTARG)
	@echo " [INST] include/darnit"
	@$(MKDIR) $(PREFIX)/include/darnit
	@$(MKDIR) $(PREFIX)/lib
	@$(MKDIR) $(PREFIX)/bin
	@install -D -m 0755 include/darnit/* $(PREFIX)/include/darnit
	@echo " [INST] libdarnit.so"
	@install -D -m 0755 $(LIB) $(PREFIX)/lib
	-@ldconfig
	@echo " [INST] tools"
	@install -D -m 0755 bin/darnit-* $(PREFIX)/bin
	@echo
	@echo "libDarnit installed."
	@echo 
