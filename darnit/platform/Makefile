# Project: libDarnit - tpw (Tiny Platform Wrapper)
include $(TOPDIR)/config.mk

DEPENDS		=
PLATFORMLIB	=	tpw.a

SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES) $(DEPENDS)
	@echo " [ CD ] darnit/platform/$(PLATFORM)/"
	+@make -C $(PLATFORM)
	@echo " [ AR ] $(PLATFORMLIB)"
	@$(AR) -cr $(PLATFORMLIB) $(OBJFILES) $(PLATFORM)/*.o
	@echo "Done."
	@echo
	
clean:
	@echo " [ CD ] darnit/platform/$(PLATFORM)/"
	+@make -C $(PLATFORM) clean
	@echo
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@echo " [ RM ] $(PLATFORMLIB)"
	@$(RM) $(PLATFORMLIB)
	@$(RM) darnit.res
	@echo "Done."
	@echo 

%.o: %.c %.h
	@echo " [ CC ] darnit/platform/$<"
	@$(CC) $(CFLAGS) -c -o $@ $<
	
