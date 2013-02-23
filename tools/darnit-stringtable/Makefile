# Project: libDarnit
#darnit-stringtable is used to compile string tables
include $(TOPDIR)/config.mk

LDTOOLS		+=	-lz

BIN		=	darnit-stringtable

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


%.o: %.c %.h
	@echo " [ CC ] tools/$(BIN)/$<"
	@$(CC) $(CTOOLS) -c -o $@ $<
