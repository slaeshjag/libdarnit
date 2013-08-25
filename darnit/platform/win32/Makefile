# Project: libDarnit - tpw (Tiny Platform Wrapper)
include $(TOPDIR)/config.mk

DEPENDS		=

SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES) $(DEPENDS)
	@echo "Done."
	@echo
	
clean:
	@echo
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@echo "Done."
	@echo 

%.o: %.c %.h
	@echo " [ CC ] darnit/platform/$(PLATFORM)/$<"
	$(CC) $(CFLAGS) -c -o $@ $<
	
