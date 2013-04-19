# Project: libDarnit
include $(TOPDIR)/config.mk

DEPFILES	=	../deps/deps.a platform/tpw.a
APIFILES	=	api/*.o

CFLAGS		+=	-DDATA_PATH=$(DATA_PATH)

DEPENDS		=

ifeq ($(strip $(OS)), Windows_NT)
	DEPENDS	+=	darnit.res
endif

SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES) $(DEPENDS)
	@echo " [ CD ] darnit/api/"
	+@make -C api/
	@echo " [ CD ] darnit/platform/"
	+@make -C platform/
	@echo " [ LD ] $(LIB)"
	@$(CC) $(CFLAGS) $(OBJFILES) $(DEPFILES) $(APIFILES) -o $(LIB) $(LDFLAGS)
	@echo "Done."
	@echo
	
clean:
	@echo
	@echo " [ CD ] darnit/api/"
	+@make -C api/ clean
	@echo " [ CD ] darnit/platform/"
	+@make -C platform/ clean
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@$(RM) darnit.res
	@echo "Done."
	@echo 

%.o: %.c %.h
	@echo " [ CC ] darnit/$<"
	@$(CC) $(CFLAGS) -c -o $@ $<
	
darnit.res:
	@windres darnit.rc -O coff -o darnit.res

