# Project: libDarnit
#darnit-mksprite is used to create sprites
include $(TOPDIR)/config.mk

BIN		=	darnit-mksprite
CFLAGS		=	

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
