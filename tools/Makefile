# Project: libDarnit
include $(TOPDIR)/config.mk

.PHONY: all clean

all:
	@echo " [ CD ] tools/darnit-fsimage"
	+@make -C darnit-fsimage
	@echo " [ CD ] tools/darnit-mksprite"
	+@make -C darnit-mksprite
	@echo " [ CD ] tools/darnit-spriteview"
	+@make -C darnit-spriteview
	@echo " [ CD ] tools/darnit-stringtable"
	+@make -C darnit-stringtable
	@echo " [ CD ] tools/darnit-tmxconv"
	+@make -C darnit-tmxconv
	@echo "Done."
	@echo 

clean:
	@echo " [ CD ] tools/darnit-fsimage"
	+@make -C darnit-fsimage clean
	@echo " [ CD ] tools/darnit-mksprite"
	+@make -C darnit-mksprite clean
	@echo " [ CD ] tools/darnit-spriteview"
	+@make -C darnit-spriteview clean
	@echo " [ CD ] tools/darnit-stringtable"
	+@make -C darnit-stringtable clean
	@echo " [ CD ] tools/darnit-tmxconv"
	+@make -C darnit-tmxconv clean
	@echo "Done."
	@echo 
