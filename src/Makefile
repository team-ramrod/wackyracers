##################################
##  Microcontroller Properties  ##
##################################
MCU = atxmega16a4
F_CPU = 32000000L
TARGET = ATXMega16


##  Toolchain Config  ##
CC = avr-gcc
LIB = 
CFLAGS = -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -std=gnu99 -pedantic
LDFLAGS = -mmcu=$(MCU)

##  Files  ##
CAM_FILES = main-cam camera bluetooth
MOTOR_FILES = main-motor motor camera bluetooth

all: build build/camera.hex build/motor.hex
rebuild: clean all

build/camera.hex: $(addprefix build/, $(addsuffix .o, $(CAM_FILES)))
build/motor.hex:  $(addprefix build/, $(addsuffix .o, $(MOTOR_FILES)))

build/main-camera.o: common.h uart.h bluetooth.h camera.h
build/main-motor.o:  common.h uart.h motor.h     ir.h

build/motor.o:     common.h motor.h
build/camera.o:    common.h camera.h
build/bluetooth.o: common.h bluetooth.h

build/%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

build/%.hex:
	$(CC) -o build/$*.hex $(LDFLAGS) $^

build:
	mkdir -p build

program-camera-stk:
	stk600 -cUSB$(SN) -d$(TARGET) -ms -e -pf -ifcamera.hex

program-camera-dude:
	avrdude ...? camera.hex

program-motor-stk:
	stk600 -cUSB$(SN) -d$(TARGET) -ms -e -pf -ifmotor.hex

program-motor-dude:
	avrdude ...? motor.hex

clean:
	rm -f build/*.o build/*.ls build/*.lis build/*.dbg build/*.mp build/*.s build/*.lst

cleanall: clean
	rm -rf build

.PHONY: clean cleanall all
.PHONY: program-camera-stk program-camera-dude
.PHONY: program-motor-stk program-motor-dude
