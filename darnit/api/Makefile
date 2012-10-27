#libDarnit API

MAKEFLAGS += --no-print-directory

CFLAGS += -fPIC -shared -I../ -I../../deps -Wall -fvisibility=hidden

SRCFILES=$(wildcard *.c)
OBJFILES=$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES)
	@echo 

clean:
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@echo

%.o: %.c
	@echo " [ CC ] darnit/api/$<"
	@$(CC) $(CFLAGS) -c -o $@ $<
