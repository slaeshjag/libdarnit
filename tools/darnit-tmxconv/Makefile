# Project: libDarnit
#darnit-tmxconv is used to converted Tiled TMX maps into libDarnit LDMZ maps.
include $(TOPDIR)/config.mk

LDTOOLS		+=	-L$(TOPDIR)/bin -lmxml -lz 
CTOOLS		+=	-I$(TOPDIR)/include -O0 -g -Wall -Wextra

BIN		=	darnit-tmxconv

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
