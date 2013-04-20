# Project: libDarnit
# Makefile configurations

LIB		=	$(TOPDIR)/bin/libdarnit.so

DBGFLAGS	=	-O0 -g -D__DEBUG__
#DBGFLAGS	=	-O3 -g
#Flags specific for tools
LDTOOLS		:=	$(LDFLAGS)
CTOOLS		:=	$(CFLAGS) -Wall -O3
#General flags
CFLAGS		+=	-Wall -I../deps -shared -fvisibility=hidden $(DBGFLAGS) -DPLATFORM=$(PLATFORM) -I$(TOPDIR)/darnit/platform/$(PLATFORM)
LDFLAGS		+=	-Wl,-soname,libdarnit.so -lmodplug -ldl -lbz2
PREFIX		=	/usr/local

#Extra install targets
INSTARG		=	

#Makefile tools
RM		=	rm -Rf
MKDIR		=	mkdir -p

ifeq ($(strip $(OS)), Windows_NT)
	#Windows specifics
	LIB	=	$(TOPDIR)/bin/libdarnit.dll
	PREFIX	=	/mingw
	LDFLAGS	+=	-lSDL -lopengl32 -lws2_32 -lshlwapi
	LDTOOLS	+=	-lws2_32
	PLATFORM=	win32
else 
ifeq ($(BUILDFOR), WIN32)
	#Windows specifics, for cross compiling to windows
	LIB	=	$(TOPDIR)/bin/libdarnit.dll
	PREFIX	=	/mingw
	LDFLAGS	+=	-lSDL -lopengl32 -lws2_32 -lshlwapi -lstdc++
	LDTOOLS	+=	-lws2_32
	CC	=	i586-mingw32msvc-gcc
	AR	=	i586-mingw32msvc-ar
	PLATFORM=	win32
else
ifeq ($(strip $(SBOX_UNAME_MACHINE)), arm)
	#Maemo specifics
	DATA_PATH=	\"/opt/usr/games\"
	CFLAGS	+=	-fPIC -DMAEMO -DHAVE_GLES `sdl-config --cflags`
	LDFLAGS	+=	`sdl-config --libs` -lSDL_gles -lEGL -lGLES_CM -lX11
	INSTARG	+=	strip
	PLATFORM=	sdl
else
ifneq (,$(findstring -DPANDORA, $(CFLAGS)))
	#Pandora specifics
	PREFIX	=	/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr
	CFLAGS	+=	-fPIC
	LDFLAGS	+=	-lGLES_CM -lEGL -lX11 -lSDL
	INSTARG	+=	strip
	PLATFORM=	sdl
else
ifneq (,$(findstring -DGCW_ZERO, $(CFLAGS)))
	#GCWZero specifics
	CFLAGS	+=	-fPIC
	LDFLAGS	+=	-lGLES_CM -lEGL
	INSTARG	+=	strip
	PLATFORM=	sdl
else
	#Linux defaults
	DATA_PATH=	\"/usr/share/games\"
	CFLAGS	+=	-fPIC
	LDFLAGS	+=	-lSDL -lGL
	PLATFORM=	sdl
endif
endif
endif
endif
endif
