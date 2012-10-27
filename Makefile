#Project: libDarnit

PREFIX = /usr/local
MAKEFLAGS += --no-print-directory
MKDIR = mkdir -p
RM = rm -Rf
.PHONY: all pandora install clean

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

pandora:
	@echo " [INIT]"
	@$(MKDIR) bin/
	@echo " [ CD ] deps/"
	+@make -C deps/
	@echo " [ CD ] darnit/"
	+@make -C darnit/ pandora
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

install: all
	@echo " [INST] include/darnit"
	@install -d -m 0755 include/darnit $(PREFIX)/include
	@echo " [INST] bin/libdarnit.so"
	@install -m 0755 bin/libdarnit.so $(PREFIX)/lib
	@ldconfig
	@echo " [INST] tools"
	@install -m 0755 bin/darnit-* $(PREFIX)/bin
	@echo
	@echo "libDarnit installed."
	@echo 
