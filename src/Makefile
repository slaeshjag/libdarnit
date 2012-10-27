#libDarnit itself

MAKEFLAGS += --no-print-directory

RM = rm -f

LIB = ../bin/libdarnit.so
CFLAGS += -Wall -I../deps -fPIC -shared -O0 -g -fvisibility=hidden -DDATA_PATH=\"/usr/share/games\"
LDFLAGS += -Wl,-soname,libdarnit.so -lSDL -lmodplug -ldl
DESKTOPFLAGS = -lGL
PANDORAFLAGS = -lGLES_CM -lEGL -lX11

DEPFILES = ../deps/deps.a
SRCFILES=$(wildcard *.c)
OBJFILES=$(SRCFILES:.c=.o)
.PHONY: add pandora clean

all: $(OBJFILES)
	@echo " [ LD ] $(LIB)"
	@$(CC) $(CFLAGS) $(OBJFILES) $(DEPFILES) $(LDFLAGS) $(DESKTOPFLAGS)
	@echo "Done."
	@echo

pandora: $(OBJFILES)
	@echo " [ LD ] $(LIB)"
	@$(CC) $(CFLAGS) $(SRCFILES) $(DEPFILES) $(LDFLAGS) $(PANDORAFLAGS)
	@echo " [STRIP] $(LIB)"
	@$(STRIP) $(LIB)
	@echo "Done."
	@echo 

clean:
	@echo
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@echo "Done."
	@echo 

%.o: %.c %.h
	@echo " [ CC ] darnit/$<"
	@$(CC) $(CFLAGS) -c -o $@ $<
