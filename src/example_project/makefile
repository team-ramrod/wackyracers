CC = iccavr
LIB = ilibw
CFLAGS =  -e -D__ICC_VERSION=722 -D__BUILD=627 -D_EE_EXTIO -DATxmega128A3  -l -g -MLongJump -MXmega -MHasMul -MEnhanced -Wf-use_elpm  
ASFLAGS = $(CFLAGS) 
LFLAGS =  -g -nb:627 -e:0x22000 -xcall -ucrtxmega.o -bfunc_lit:0x200.0x22000 -dram_end:0x3fff -bdata:0x2000.0x3fff -dhwstk_size:30 -beeprom:0.2048 -fihx_coff -S2
FILES = dataflash.o ee.o fat32.o global.o gprs.o main.o sd.o spid.o spie.o timer.o twic.o usartc1.o usartd0.o adc.o mcu.o rtc.o compass.o
SN = :0045E550816F

build:	$(FILES)
	$(CC) -o out $(LFLAGS) $(FILES)  -lfpatmega -lcatmega	

	
.PHONY : clean 

clean:
	rm -f *.o *.ls *.lis *.dbg *.cof *.mp *.s *.lst *~ 
	
.PHONY : %v
%v:
	stk600 -cUSB -dATmega644P -ut$@

program: build
	stk600 -cUSB$(SN) -dATmega644P -ms -e -pf -ifout.hex
	
rebuild: clean build
