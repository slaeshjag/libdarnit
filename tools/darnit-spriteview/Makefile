# Project: libDarnit
#darnit-spriteview is used to view sprites
include $(TOPDIR)/config.mk

LDTOOLS		+=	-L$(TOPDIR)/bin -ldarnit 
CTOOLS		+=	-I$(TOPDIR)/include

BIN		=	darnit-spriteview

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
