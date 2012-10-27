# dependencies for libDarnit

MAKEFLAGS += --no-print-directory

RM = rm -f
CFLAGS = -c -Wall -fPIC -fvisibility=hidden
DEBUG = -g -O0
RELEASE = -O3
LIBOUT = deps.a

SRCFILES=$(wildcard *.c)
OBJFILES=$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES)
	@echo " [ AR ] $(LIBOUT)"
	@$(AR) -cq $(LIBOUT) $(OBJFILES)
	@echo "Done."
	@echo 

clean:
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@echo " [ RM ] $(LIBOUT)"
	@$(RM) $(LIBOUT)
	@echo "Done."
	@echo 

%.o: %.c %.h
	@echo " [ CC ] deps/$<"
	@$(CC) $(CFLAGS) -o $@ $<
