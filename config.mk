# Project: libDarnit
# Makefile configurations

LIB		=	$(TOPDIR)/bin/libdarnit.so

<<<<<<< HEAD
DBGFLAGS	=	-O0 -g -D__DEBUG__
=======
#DBGFLAGS	=	-O0 -g -D__DEBUG__
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
#DBGFLAGS	=	-O3 -g
#Flags specific for tools
LDTOOLS		:=	$(LDFLAGS)
CTOOLS		:=	$(CFLAGS) -Wall -O3
#General flags
CFLAGS		+=	-Wall -I../deps -shared -fvisibility=hidden $(DBGFLAGS)
<<<<<<< HEAD
LDFLAGS		+=	-Wl,-soname,libdarnit.so -lmodplug -ldl -lbz2
=======
LDFLAGS		+=	-Wl,-soname,libdarnit.so -lmodplug -lbz2
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
PREFIX		=	/usr/local

#Extra install targets
INSTARG		=	

RM		=	rm -Rf
MKDIR		=	mkdir -p

ifeq ($(strip $(OS)), Windows_NT)
	#Windows specifics
	LIB	=	$(TOPDIR)/bin/libdarnit.dll
	PREFIX	=	/mingw
<<<<<<< HEAD
	LDFLAGS	+=	-lSDL -lopengl32 -lws2_32 -lshlwapi
	LDTOOLS	+=	-lws2_32
	CFLAGS	+=	-DPLATFORM_SDL
=======
	LDFLAGS	+=	-lSDL -lopengl32 -lws2_32 -lshlwapi -ldl
	LDTOOLS	+=	-lws2_32
else 
ifeq ($(BUILDFOR), WIN32)
	#Windows specifics, for cross compiling to windows
	LIB	=	$(TOPDIR)/bin/libdarnit.dll
	PREFIX	=	/mingw
	LDFLAGS	+=	-lSDL -lopengl32 -lws2_32 -lshlwapi -lstdc++
	LDTOOLS	+=	-lws2_32
	CC	=	i586-mingw32msvc-gcc
	AR	=	i586-mingw32msvc-ar
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
else
ifeq ($(strip $(SBOX_UNAME_MACHINE)), arm)
	#Maemo specifics
	DATA_PATH=	\"/opt/usr/games\"
	CFLAGS	+=	-fPIC -DMAEMO -DHAVE_GLES `sdl-config --cflags`
<<<<<<< HEAD
	LDFLAGS	+=	`sdl-config --libs` -lSDL_gles -lEGL -lGLES_CM -lX11
	INSTARG	+=	strip
	CFLAGS	+=	-DPLATFORM_SDL
=======
	LDFLAGS	+=	`sdl-config --libs` -lSDL_gles -lEGL -lGLES_CM -lX11 -ldl
	INSTARG	+=	strip
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
else
ifneq (,$(findstring -DPANDORA, $(CFLAGS)))
	#Pandora specifics
	PREFIX	=	/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr
	CFLAGS	+=	-fPIC
<<<<<<< HEAD
	LDFLAGS	+=	-lGLES_CM -lEGL -lX11 -lSDL
	INSTARG	+=	strip
	CFLAGS	+=	-DPLATFORM_SDL
=======
	LDFLAGS	+=	-lGLES_CM -lEGL -lX11 -lSDL -ldl
	INSTARG	+=	strip
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
else
ifneq (,$(findstring -DGCW_ZERO, $(CFLAGS)))
	#GCWZero specifics
	CFLAGS	+=	-fPIC
<<<<<<< HEAD
	LDFLAGS	+=	-lGLES_CM -lEGL
	INSTARG	+=	strip
	CFLAGS	+=	-DPLATFORM_SDL
=======
	LDFLAGS	+=	-lGLES_CM -lEGL -ldl
	INSTARG	+=	strip
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
else
	#Linux defaults
	DATA_PATH=	\"/usr/share/games\"
	CFLAGS	+=	-fPIC
<<<<<<< HEAD
	LDFLAGS	+=	-lSDL -lGL
	CFLAGS	+=	-DPLATFORM_SDL
=======
	LDFLAGS	+=	-lSDL -lGL -ldl
endif
>>>>>>> 21eafb2752d367b675f1f5e5f75430333a4a015f
endif
endif
endif
endif
