#Project: libDarnit

PREFIX = /usr/local
MAKEFLAGS += --no-print-directory
MKDIR = mkdir -p
RM = rm -Rf
.PHONY: all pandora gcwzero install clean

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

windows:
	@echo " [INIT] bin/"
	@$(MKDIR) bin/
	@echo " [ CD ] deps/"
	+@make -C deps/
	@echo " [ CD ] darnit/"
	+@make -C darnit/ windows
	@echo " [ CD ] tools/"
	+@make -C tools/ windows
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

gcwzero:
	@echo " [INIT]"
	@$(MKDIR) bin/
	@echo " [ CD ] deps/"
	+@make -C deps/
	@echo " [ CD ] darnit/"
	+@make -C darnit/ gcwzero
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
	
install-windows: 
	@echo " [INST] include/darnit"
	@$(MKDIR) /mingw/include/darnit
	@install -m 0755 include/darnit/* /mingw/include/darnit
	@echo " [INST] bin/libdarnit.dll"
	@install -m 0755 bin/libdarnit.dll /mingw/lib
	@echo " [INST] tools"
	@install -m 0755 bin/darnit-* /mingw/bin
	@echo
	@echo "libDarnit installed."
	@echo 

install: 
	@echo " [INST] include/darnit"
	@$(MKDIR) $(PREFIX)/include/darnit
	@install -m 0755 include/darnit/* $(PREFIX)/include/darnit
	@echo " [INST] bin/libdarnit.so"
	@install -m 0755 bin/libdarnit.so $(PREFIX)/lib
	@ldconfig
	@echo " [INST] tools"
	@install -m 0755 bin/darnit-* $(PREFIX)/bin
	@echo
	@echo "libDarnit installed."
	@echo 
