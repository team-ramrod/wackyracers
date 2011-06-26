CC = cc
CFLAGS = -Wall
LDFLAGS = 

all: main

main: main.o camera.o
	$(CC) $(LDFLAGS) $(.ALLSRC) -o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

camera.o: camera.c camera.h
	$(CC) $(CFLAGS) -c camera.c

hack: hack.o serial.o
	$(CC) $(LDFLAGS) $(.ALLSRC) -o hack
	
hack.o: hack.c
	$(CC) $(CFLAGS) -c hack.c
	
serial.o: serial.c serial.h
	$(CC) $(CFLAGS) -c serial.c
	
clean:
	rm -f *.o
	