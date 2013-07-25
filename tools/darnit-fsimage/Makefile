# Project: libDarnit
#darnit-fsimage is used to create LDI image files
#include $(TOPDIR)/config.mk

BIN		=	darnit-fsimage

SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES)
	@echo " [ LD ] tools/$(BIN)/$(BIN)"
	@$(CC) $(CTOOLS) $(OBJFILES) -o $(TOPDIR)/bin/$(BIN) $(LDTOOLS)
	@echo 

clean:
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@echo 


%.o: %.c %.h
	@echo " [ CC ] tools/$(BIN)/$<"
	@$(CC) $(CTOOLS) -c -o $@ $<
