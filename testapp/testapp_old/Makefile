LDFLAGS += -L../bin -ldarnit
CFLAGS += -g -Wall 

default:
	$(CC) $(CFLAGS) testapp.c -o testapp $(LDFLAGS)

clean:
	@echo " [ RM ] testapp/testapp_old/testapp"
	@rm -f testapp
