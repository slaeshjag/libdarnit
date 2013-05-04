LDFLAGS += -L../bin -ldarnit
CFLAGS += -g -Wall 

default:
	$(CC) $(CFLAGS) testapp.c -o testapp $(LDFLAGS)
	mkdir libldi
	$(CC) testapp_dynlib.c -shared -fPIC -Wl,-soname,dynlib_test.so -o libldi/dynlib_test.so
	darnit-fsimage libldi lib.ldi
	cp libldi/* .
	rm -Rf libldi
	cat lib.ldi >> testapp

clean:
	@echo " [ RM ] testapp/testapp_old/testapp"
	@rm -f testapp
