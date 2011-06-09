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
OBJCOPY = avr-objcopy
LIB = 
CFLAGS = -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -std=gnu99 -pedantic
LDFLAGS = -mmcu=$(MCU)

all: build/camera.hex build/motor.hex
rebuild: clean all


#############
##  Files  ##
#############
CAM_FILES        = main-cam    uart  led  button camera  bluetooth
MOTOR_FILES      = main-motor  uart  led  button motor   ir
TEST_FILES       = test              led  button
MOTOR_TEST_FILES = motor_test        led  motor
UART_TEST_FILES  = uart_test   uart

build/camera.out: $(addprefix build/, $(addsuffix .o, $(CAM_FILES)))
build/motor.out:  $(addprefix build/, $(addsuffix .o, $(MOTOR_FILES)))
build/test.out:   $(addprefix build/, $(addsuffix .o, $(TEST_FILES)))
build/motor_test.out: $(addprefix build/, $(addsuffix .o, $(MOTOR_TEST_FILES)))
build/uart_test.out: $(addprefix build/, $(addsuffix .o, $(UART_TEST_FILES)))

build/main-camera.o: common.h uart.h led.h button.h clock.h camera.h  bluetooth.h
build/main-motor.o:  common.h uart.h led.h button.h clock.h motor.h   ir.h
build/test.o:                        led.h button.h clock.h
build/motor_test.o:                  led.h button.h clock.h motor.h
build/uart_test.o: common.h uart.h

build/uart.o:      common.h uart.h
build/ir.o:        common.h ir.h
build/led.o:       common.h led.h
build/uart.o:      common.h uart.h
build/motor.o:     common.h motor.h
build/camera.o:    common.h camera.h
build/bluetooth.o: common.h bluetooth.h


###############################
##  Internal Implementation  ##
###############################
build/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

build/%.hex: build/%.out
	$(OBJCOPY) -O ihex $< $@

build/%.out:
	$(CC) $(LDFLAGS) -o $@ $^

build:
	mkdir -p build

program-camera-stk:
	stk600 -cUSB$(SN) -d$(TARGET) -ms -e -pf -ifcamera.hex

program-motor-stk:
	stk600 -cUSB$(SN) -d$(TARGET) -ms -e -pf -ifmotor.hex

dude-%: build/%.hex
	avrdude -p x16a4 -c avrispmkII -P usb -U $< -qq

v-dude-%: build/%.hex
	avrdude -p x16a4 -c avrispmkII -P usb -U $< -v

clean:
	rm -f build/*.o build/*.ls build/*.lis build/*.dbg build/*.mp build/*.s build/*.lst build/*.out

cleanall:
	rm -rf build

.PHONY: all rebuild
.PHONY: clean cleanall
