CC = avr-gcc
LIB = 
CFLAGS =   
LFLAGS =  

SRCS = "$(wildcard *.c)"
OBJS = "$(wildcard *.o)"

TARGET = ATXMega16
SN = :

all: build program

rebuild: clean build

build:	$(OBJS)
	$(CC) -o out $(LFLAGS) $(OBJS) 

.PHONY : clean %v

clean:
	rm -f *.o *.ls *.lis *.dbg *.mp *.s *.lst *~ 
	
program: 
	stk600 -cUSB$(SN) -d$(TARGET) -ms -e -pf -ifout.hex
	
