CC = iccavr
LIB = ilibw
CFLAGS =  -e -D__ICC_VERSION=723 -D__BUILD=0 -DATMega32  -l -g -MLongJump -MHasMul -MEnhanced 
ASFLAGS = $(CFLAGS) 
LFLAGS =  -g -nb:0 -e:0x8000 -ucrtatmega.o -bfunc_lit:0x54.0x8000 -dram_end:0x85f -bdata:0x60.0x85f -dhwstk_size:30 -beeprom:0.1024 -fihx_coff -S2
OBJS="$(wildcard *.o)"
SN = :0045E550816F

all: build program

build:	$(OBJS)
	$(CC) -o out $(LFLAGS) $(OBJS)  -lfpatmega -lcatmega	

.PHONY : clean 

clean:
	rm -f *.o *.ls *.lis *.dbg *.mp *.s *.lst *~ 
	
.PHONY : %v
%v:
	stk600 -cUSB$(SN) -dATMega32 -ut$@

program: 
	stk600 -cUSB$(SN) -dATMega32 -ms -e -pf -ifout.hex
	
rebuild: clean build
