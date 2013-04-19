# Project: libDarnit - tpw (Tiny Platform Wrapper)
include $(TOPDIR)/config.mk

OBJECTFILES	=	*.o

DEPENDS		=
PLATFORMLIB	=	tpw.a

SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES) $(DEPENDS)
	@echo " [ AR ] $(PLATFORMLIB)"
	@$(AR) -cr $(PLATFORMLIB) $(OBJECTFILES)
	@echo "Done."
	@echo
	
clean:
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
	
