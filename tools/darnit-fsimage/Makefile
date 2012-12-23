#darnit-fsimage is used to create LDI image files

CFLAGS += -O3 -Wall
LDFLAGS = 

SRCFILES=$(wildcard *.c)
OBJFILES=$(SRCFILES:.c=.o)
BIN = darnit-fsimage
RM = rm -f
.PHONY: all windows clean

all: $(OBJFILES)
	@echo " [ LD ] tools/$(BIN)/$(BIN)"
	@$(CC) $(CFLAGS) $(OBJFILES) -o ../../bin/$(BIN) $(LDFLAGS)
	@echo 

windows: $(OBJFILES)
	@echo " [ LD ] tools/$(BIN)/$(BIN)"
	@$(CC) $(CFLAGS) $(OBJFILES) -o ../../bin/$(BIN) $(LDFLAGS) -lws2_32
	@echo 

clean:
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@echo 


%.o: %.c %.h
	@echo " [ CC ] tools/$(BIN)/$<"
	@$(CC) $(CFLAGS) -c -o $@ $<

