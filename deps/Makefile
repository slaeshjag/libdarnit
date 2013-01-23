# Project: libDarnit
include $(TOPDIR)/config.mk

LIBOUT		=	deps.a

SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES)
	@echo " [ AR ] $(LIBOUT)"
	@$(AR) -cq $(LIBOUT) $(OBJFILES)
	@echo "Done."
	@echo 

clean:
	@echo
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@echo " [ RM ] $(LIBOUT)"
	@$(RM) $(LIBOUT)
	@echo "Done."
	@echo 

%.o: %.c %.h
	@echo " [ CC ] deps/$<"
	@$(CC) $(CFLAGS) -o $@ $<
