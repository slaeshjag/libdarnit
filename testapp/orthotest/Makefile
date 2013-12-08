LDFLAGS += -L../../bin -ldarnit
CFLAGS += -g -Wall 

default:
	$(CC) $(CFLAGS) orthotest.c -o orthotest.elf $(LDFLAGS)

clean:
	@echo " [ RM ] testapp/orthotest/orthotest.elf"
	@rm -f *.elf
