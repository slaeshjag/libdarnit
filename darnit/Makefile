#libDarnit itself

MAKEFLAGS += --no-print-directory

RM = rm -f

LIB = ../bin/libdarnit.so
CFLAGS += -Wall -I../deps -fPIC -shared -O0 -g -fvisibility=hidden -DDATA_PATH=\"/usr/share/games\"
LDFLAGS += -Wl,-soname,libdarnit.so -lSDL -lmodplug -ldl -lbz2
DESKTOPFLAGS = -lGL
WINDOWSFLAGS = -lopengl32 -lws2_32 -lshlwapi
PANDORAFLAGS = -lGLES_CM -lEGL -lX11
GCWZEROFLAGS = -lGLES_CM -lEGL

DEPFILES = ../deps/deps.a
APIFILES = api/*.o
SRCFILES=$(wildcard *.c)
OBJFILES=$(SRCFILES:.c=.o)
.PHONY: all windows pandora gcwzero clean

all: $(OBJFILES)
	@echo " [ CD ] darnit/api/"
	+@make -C api/
	@echo " [ LD ] $(LIB)"
	@$(CC) $(CFLAGS) $(OBJFILES) $(DEPFILES) $(APIFILES) -o $(LIB) $(LDFLAGS) $(DESKTOPFLAGS)
	@echo "Done."
	@echo
	
windows: $(OBJFILES)
	@echo " [ CD ] darnit/api/"
	+@make -C api/
	@echo " [ LD ] $(LIB)"
	@windres darnit.rc -O coff -o darnit.res
	@$(CC) $(CFLAGS) $(OBJFILES) $(DEPFILES) $(APIFILES) darnit.res -o ../bin/libdarnit.dll $(LDFLAGS) $(WINDOWSFLAGS)
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

gcwzero: $(OBJFILES)
	@echo " [ CD ] darnit/api/"
	@make -C api/
	@echo " [ LD ] $(LIB)"
	@$(CC) $(CFLAGS) $(SRCFILES) $(DEPFILES) $(APIFILES) -o $(LIB) $(LDFLAGS) $(GCWZEROFLAGS)
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
	@$(RM) darnit.res
	@echo "Done."
	@echo 

%.o: %.c %.h
	@echo " [ CC ] darnit/$<"
	@$(CC) $(CFLAGS) -c -o $@ $<
