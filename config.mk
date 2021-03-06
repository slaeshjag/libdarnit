# Project: libDarnit
# Makefile configurations

LIB		=	$(TOPDIR)/bin/libdarnit.so

#DBGFLAGS	=	-O0 -g -D__DEBUG__
VERSION		=	v.0.2b5r2
DBGFLAGS	=	-O3 -g
#Flags specific for tools
LDTOOLS		:=	$(LDFLAGS)
CTOOLS		:=	$(CFLAGS) -Wall -O3
#General flags
CFLAGS		+=	-Wall -I../deps -shared $(DBGFLAGS) -DPLATFORM=$(PLATFORM) -I$(TOPDIR)/darnit/platform/$(PLATFORM)
LDFLAGS		+=	-Wl,-soname,libdarnit.so -lmodplug -lbz2
PREFIX		=	/usr/local

#Extra install targets
INSTARG		=	

#Makefile tools
RM		=	rm -Rf
MKDIR		=	mkdir -p


ifeq ($(strip $(OS)), Windows_NT)
	#Windows specifics
	CFLAGS	+=	-DPLATFORM_STRING=\"win32-i386\"
	LIB	=	$(TOPDIR)/bin/libdarnit.dll
	PREFIX	=	/mingw
	LDFLAGS	+=	-lopengl32 -lws2_32 -lshlwapi -lwinmm -lgdi32 -ldl
	LDTOOLS	+=	-lws2_32
	PLATFORM=	win32
else 
ifeq ($(BUILDFOR), WIN32)
	#Windows specifics, for cross compiling to windows
	CFLAGS	+=	-DPLATFORM_STRING=\"win32-i386\"
	LIB	=	$(TOPDIR)/bin/libdarnit.dll
	PREFIX	=	/usr/i586-mingw32msvc
	LDFLAGS	+=	-lopengl32 -lws2_32 -lshlwapi -lstdc++ -lgdi32 -lwinmm
	LDTOOLS	+=	-lws2_32
	CC	=	i586-mingw32msvc-gcc
	STRIP	=	i586-mingw32msvc-strip
	AR	=	i586-mingw32msvc-ar
	PLATFORM=	win32
	#PLATFORM=	glfw
else
ifeq ($(BUILDFOR), WIN64)
	#Windows64 specifics, for cross compiling to windows
	CFLAGS	+=	-DPLATFORM_STRING=\"win32-amd64\"
	LIB	=	$(TOPDIR)/bin/libdarnit.dll
	PREFIX	=	/usr/x86_64-w64-mingw32
	LDFLAGS	+=	-lopengl32 -lws2_32 -lshlwapi -lstdc++ -lgdi32 -lwinmm
	LDTOOLS	+=	-lws2_32
	CC	=	x86_64-w64-mingw32-gcc
	STRIP	=	x86_64-w64-mingw32-strip
	AR	=	x86_64-w64-mingw32-ar
	PLATFORM=	win32
else
ifeq ($(strip $(SBOX_UNAME_MACHINE)), arm)
	#Maemo specifics
	CFLAGS	+=	-DPLATFORM_STRING=\"linux-armv7\"
	DATA_PATH=	\"/opt/usr/games\"
	CFLAGS	+=	-fvisibility=hidden -fPIC -DMAEMO -DHAVE_GLES `sdl-config --cflags`
	LDFLAGS	+=	`sdl-config --libs` -lSDL_gles -lEGL -lGLES_CM -lX11 -ldl
	PTHREAD_L +=	-lpthread
	INSTARG	+=	$(STRIP)
	PLATFORM=	sdl
else
ifneq (,$(findstring -DPANDORA, $(CFLAGS)))
	#Pandora specifics
	CFLAGS	+=	-DPLATFORM_STRING=\"linux-armv7\"
	PREFIX	=	/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr
	CFLAGS	+=	-fvisibility=hidden -fPIC
	LDFLAGS	+=	-lGLES_CM -lEGL -lX11 -lSDL -ldl
	PTHREAD_L +=	-lpthread
	INSTARG	+=	$(STRIP)
	PLATFORM=	sdl
else
ifeq ($(BUILDFOR), PANDORA)
	#Pandora specifics
	CFLAGS	+=	-DPLATFORM_STRING=\"linux-armv7\" -DPANDORA -DHAVE_GLES -O3 -DNOREDEF
	PREFIX	=	/usr/local
	CFLAGS	+=	-fvisibility=hidden -fPIC
	LDFLAGS	+=	-lGLES_CM -lEGL -lX11 -lSDL -ldl
	PTHREAD_L +=	-lpthread
	INSTARG	+=	$(STRIP)
	PLATFORM=	sdl
else
ifneq (,$(findstring -DDB_PYRA, $(CFLAGS)))
	#Pandora specifics
	CFLAGS	+=	-DPLATFORM_STRING=\"linux-armv7hf\" -DPANDORA
	PREFIX	=	/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr
	CFLAGS	+=	-fvisibility=hidden -fPIC
	LDFLAGS	+=	-lGLES_CM -lEGL -lX11 -lSDL -ldl
	PTHREAD_L +=	-lpthread
	INSTARG	+=	$(STRIP)
	PLATFORM=	sdl
else
ifneq (,$(findstring -DGCW_ZERO, $(CFLAGS)))
	#GCWZero specifics
	CFLAGS	+=	-DPLATFORM_STRING=\"gcwzero-mips32\"
	CFLAGS	+=	-fvisibility=hidden -fPIC
	LDFLAGS	+=	-lGLES_CM -lEGL -ldl
	PTHREAD_L +=	-lpthread
	INSTARG	+=	$(STRIP)
	PLATFORM=	sdl
else
ifeq ($(BUILDFOR), RPI)
	#RaspberryPi specifics
	CFLAGS	+=	-DPLATFORM_STRING=\"linux-armv6\"
	DATA_PATH=	\"/usr/local/games\"
	PREFIX	=	/usr/local/
	CFLAGS	+=	-fvisibility=hidden -fPIC -DHAVE_GLES -DRASPBERRYPI -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux/
	LDFLAGS	+=	-L/opt/vc/lib -lGLESv1_CM -lEGL -lX11 -lSDL -ldl
	PTHREAD_L +=	-lpthread
	PLATFORM=	sdl
else
	#Linux defaults
	UNAME	=	`uname -m`
ifneq (,$(findstring x86_64, $(UNAME)))
	UNAME	=	i386
endif
	CFLAGS	+=	-DPLATFORM_STRING=\"linux-$(UNAME)\"
	DATA_PATH=	\"/usr/share/games\"
	CFLAGS	+=	-fvisibility=hidden -fPIC -D__ALSA_CAPTURE__
	LDFLAGS	+=	-lSDL -lGL -ldl -lasound
	PTHREAD_L +=	-lpthread
	PLATFORM=	sdl
	STRIP	=	strip
endif
endif
endif
endif
endif
endif
endif
endif
endif
