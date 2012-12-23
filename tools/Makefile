
MAKEFLAGS += --no-print-directory
.PHONY: all windows clean

all:
	@echo " [ CD ] tools/darnit-fsimage"
	+@make -C darnit-fsimage
	@echo " [ CD ] tools/darnit-mksprite"
	+@make -C darnit-mksprite
	@echo " [ CD ] tools/darnit-spriteview"
	+@make -C darnit-spriteview
	@echo " [ CD ] tools/darnit-stringtable"
	+@make -C darnit-stringtable
	@echo "Done."
	@echo 
	
windows:
	@echo " [ CD ] tools/darnit-fsimage"
	+@make -C darnit-fsimage windows
	@echo " [ CD ] tools/darnit-mksprite"
	+@make -C darnit-mksprite windows
	@echo " [ CD ] tools/darnit-spriteview"
	+@make -C darnit-spriteview windows
	@echo " [ CD ] tools/darnit-stringtable"
	+@make -C darnit-stringtable windows
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
	@echo "Done."
