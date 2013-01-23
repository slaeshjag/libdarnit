# Project: libDarnit
include $(TOPDIR)/config.mk

CFLAGS		+=	-I../ -I../../deps

SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.o)
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
