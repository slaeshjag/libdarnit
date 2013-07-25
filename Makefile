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
	@echo
	@echo "Source tree cleaned."
	@echo

windows-release:
	@echo "Preparing a windows release archive.."
	@mkdir -p release
	@mkdir -p release/bin
	@mkdir -p release/lib
	@mkdir -p release/lib/release
	@mkdir -p release/lib/debug
	@cp -a include/ release/
	@cp -a bin/darnit-fsimage release/bin/darnit-fsimage.exe
	@cp -a bin/darnit-mksprite release/bin/darnit-mksprite.exe
	@cp -a bin/darnit-spriteview release/bin/darnit-spriteview.exe
	@cp -a bin/darnit-stringtable release/bin/darnit-stringtable.exe
	@cp -a bin/darnit-tmxconv release/bin/darnit-tmxconv.exe
	@$(STRIP) release/bin/*
	@cp -a bin/libdarnit* release/lib/debug
	@cp -a bin/libdarnit.dll release/lib/release
	@$(STRIP) release/lib/release/*
	@mv release libdarnit-$(VERSION)-win32
	@zip -r9 libdarnit-$(VERSION)-win32 bin/libdarnit-$(VERSION)-win32.zip
	@rm -Rf libdarnit-$(VERSION)-win32
	@echo " Created bin/libdarnit-$(VERSION)-win32.zip"

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
	@echo " [INST] libdarnit_s.a"
	@install -D -m 0755 bin/libdarnit_s.a $(PREFIX)/lib
	-@ldconfig
	@echo " [INST] tools"
	@install -D -m 0755 bin/darnit-* $(PREFIX)/bin
	@echo
	@echo "libDarnit installed."
	@echo 
