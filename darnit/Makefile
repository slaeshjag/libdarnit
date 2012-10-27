#libDarnit itself

MAKEFLAGS += --no-print-directory

RM = rm -f

LIB = ../bin/libdarnit.so
CFLAGS += -Wall -I../deps -fPIC -shared -O0 -g -fvisibility=hidden -DDATA_PATH=\"/usr/share/games\"
LDFLAGS += -Wl,-soname,libdarnit.so -lSDL -lmodplug -ldl
DESKTOPFLAGS = -lGL
PANDORAFLAGS = -lGLES_CM -lEGL -lX11

DEPFILES = ../deps/deps.a
APIFILES = api/*.o
SRCFILES=$(wildcard *.c)
OBJFILES=$(SRCFILES:.c=.o)
.PHONY: all pandora clean

all: $(OBJFILES)
	@echo " [ CD ] darnit/api/"
	+@make -C api/
	@echo " [ LD ] $(LIB)"
	@$(CC) $(CFLAGS) $(OBJFILES) $(DEPFILES) $(APIFILES) -o $(LIB) $(LDFLAGS) $(DESKTOPFLAGS)
	@echo "Done."
	@echo

pandora: $(OBJFILES)
	@echo " [ CD ] darnit/api/"
	@make -C api/
	@echo " [ LD ] $(LIB)"
	@$(CC) $(CFLAGS) $(SRCFILES) $(DEPFILES) $(APIFILES) -o $(LIB) $(LDFLAGS) $(PANDORAFLAGS)
	@echo " [STRIP] $(LIB)"
	@$(STRIP) $(LIB)
	@echo "Done."
	@echo 

clean:
	@echo
	@echo " [ CD ] darnit/api/"
	@make -C api/ clean
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@echo "Done."
	@echo 

%.o: %.c %.h
	@echo " [ CC ] darnit/$<"
	@$(CC) $(CFLAGS) -c -o $@ $<
