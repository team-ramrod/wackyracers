##################################
##  Microcontroller Properties  ##
##################################
MCU = atxmega16a4
F_CPU = 32000000L
TARGET = ATXMega16


########################
##  Toolchain Config  ##
########################
CC = avr-gcc
LIB = 
CFLAGS = -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -std=gnu99 -pedantic
LDFLAGS = -mmcu=$(MCU)

all: build build/camera.hex build/motor.hex
rebuild: clean all


#############
##  Files  ##
#############
CAM_FILES =   main-cam    uart  led  camera  bluetooth
MOTOR_FILES = main-motor  uart  led  motor   ir

build/camera.hex: $(addprefix build/, $(addsuffix .o, $(CAM_FILES)))
build/motor.hex:  $(addprefix build/, $(addsuffix .o, $(MOTOR_FILES)))

build/main-camera.o: common.h uart.h led.h camera.h  bluetooth.h
build/main-motor.o:  common.h uart.h led.h motor.h   ir.h

build/ir.o:        common.h ir.h
build/led.o:       common.h led.h
build/uart.o:      common.h uart.h
build/motor.o:     common.h motor.h
build/camera.o:    common.h camera.h
build/bluetooth.o: common.h bluetooth.h

build/test.hex:   build/test.o
build/test.o:     test.c

###############################
##  Internal Implementation  ##
###############################
build/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

build/%.hex:
	$(CC) $(LDFLAGS) -o build/$*.hex $^

build:
	mkdir -p build

program-camera-stk:
	stk600 -cUSB$(SN) -d$(TARGET) -ms -e -pf -ifcamera.hex

program-motor-stk:
	stk600 -cUSB$(SN) -d$(TARGET) -ms -e -pf -ifmotor.hex

dude-%: build/%.hex
	avrdude -p x16a4 -c avrispmkII -P usb -U $< -v

clean:
	rm -f build/*.o build/*.ls build/*.lis build/*.dbg build/*.mp build/*.s build/*.lst

cleanall:
	rm -rf build

.PHONY: all rebuild
.PHONY: clean cleanall
