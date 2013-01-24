# Project: libDarnit
# Makefile configurations

LIB		=	$(TOPDIR)/bin/libdarnit.so

DBGFLAGS	=	-O0 -g -D__DEBUG__
#Flags specific for tools
LDTOOLS		:=	$(LDFLAGS)
CTOOLS		:=	$(CFLAGS) -Wall -O3
#General flags
CFLAGS		+=	-Wall -I../deps -shared -fvisibility=hidden $(DBGFLAGS)
LDFLAGS		+=	-Wl,-soname,libdarnit.so -lmodplug -ldl -lbz2
PREFIX		=	/usr/local

#Extra install targets
INSTARG		=	

RM		=	rm -Rf
MKDIR		=	mkdir -p

ifeq ($(strip $(OS)), Windows_NT)
	#Windows specifics
	LIB	=	$(TOPDIR)/bin/libdarnit.dll
	PREFIX	=	/mingw
	LDFLAGS	+=	-lSDL -lopengl32 -lws2_32 -lshlwapi
	LDTOOLS	+=	-lws2_32
else
ifeq ($(strip $(SBOX_UNAME_MACHINE)), arm)
	#Maemo specifics
	DATA_PATH=	\"/opt/usr/games\"
	CFLAGS	+=	-fPIC -DMAEMO -DHAVE_GLES `sdl-config --cflags`
	LDFLAGS	+=	`sdl-config --libs` -lSDL_gles -lEGL -lGLES_CM -lX11
	INSTARG	+=	strip
else
ifneq (,$(findstring -DPANDORA, $(CFLAGS)))
	#Pandora specifics
	PREFIX	=	/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr
	CFLAGS	+=	-fPIC
	LDFLAGS	+=	-lGLES_CM -lEGL -lX11
	INSTARG	+=	strip
else
ifneq (,$(findstring -DGCW_ZERO, $(CFLAGS)))
	#GCWZero specifics
	CFLAGS	+=	-fPIC
	LDFLAGS	+=	-lGLES_CM -lEGL
	INSTARG	+=	strip
else
	#Linux defaults
	DATA_PATH=	\"/usr/share/games\"
	CFLAGS	+=	-fPIC
	LDFLAGS	+=	-lSDL -lGL
endif
endif
endif
endif
