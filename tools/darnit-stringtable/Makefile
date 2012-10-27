#darnit-fsimage is used to create LDI image files

CFLAGS += -O3 -Wall
LDFLAGS = -lz

SRCFILES=$(wildcard *.c)
OBJFILES=$(SRCFILES:.c=.o)
BIN = darnit-stringtable
RM = rm -f
.PHONY: all clean

all: $(OBJFILES)
	@echo " [ LD ] tools/$(BIN)/$(BIN)"
	@$(CC) $(CFLAGS) $(OBJFILES) -o ../../bin/$(BIN) $(LDFLAGS)
	@echo 


clean:
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@echo 


%.o: %.c %.h
	@echo " [ CC ] tools/$(BIN)/$<"
	@$(CC) $(CFLAGS) -c -o $@ $<

