CC = cc
CFLAGS =
LDFLAGS = 

hack: hack.o serial.o
	$(CC) $(LDFLAGS) $(.ALLSRC) -o hack
	
hack.o: hack.c
	$(CC) $(CFLAGS) -c hack.c
	
serial.o: serial.c serial.h
	$(CC) $(CFLAGS) -c serial.c
	
clean:
	rm -f serial.o hack.o
	