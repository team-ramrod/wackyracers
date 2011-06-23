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
rebuild: cleanall all


#############
##  Files  ##
#############
CAM_FILES              = main-cam          uart  led  button camera            bluetooth
MOTOR_FILES            = main-motor        uart  led  button motor motor_controller  ir   charger
LED_TEST_FILES         = test                    led  button
MOTOR_TEST_FILES       = motor_test        uart  led                 motor motor_controller
MOTOR_UART_TEST_FILES  = motor_uart_test   uart  led         charger motor motor_controller
CAM_UART_TEST_FILES    =   cam_uart_test   uart  led 
UART_TEST_FILES        = uart_test         uart  led

build/camera.out: $(addprefix build/, $(addsuffix .o, $(CAM_FILES)))
build/motor.out:  $(addprefix build/, $(addsuffix .o, $(MOTOR_FILES)))
build/test.out:   $(addprefix build/, $(addsuffix .o, $(LED_TEST_FILES)))
build/motor_test.out: $(addprefix build/, $(addsuffix .o, $(MOTOR_TEST_FILES)))
build/motor_uart_test.out: $(addprefix build/, $(addsuffix .o, $(MOTOR_UART_TEST_FILES)))
build/cam_uart_test.out: $(addprefix build/, $(addsuffix .o, $(CAM_UART_TEST_FILES)))
build/uart_test.out: $(addprefix build/, $(addsuffix .o, $(UART_TEST_FILES)))

build/main-camera.o:     common.h        led.h button.h uart_cam_board.h clock.h camera.h  
build/main-motor.o:      common.h uart.h led.h button.h clock.h motor_controller.h   ir.h      charger.h
build/test.o:                            led.h button.h clock.h
build/motor_test.o:               uart.h led.h button.h clock.h motor_controller.h
build/motor_uart_test.o: common.h uart.h led.h charger.h motor.h motor_controller.h
build/cam_uart_test.o:   common.h uart.h led.h
build/uart_test.o:       common.h uart_cam_board.h

build/uart.o:      common.h uart.h
build/ir.o:        common.h ir.h
build/led.o:       common.h led.h
build/uart.o:      common.h uart.h
build/motor.o:     common.h motor.h
build/camera.o:    common.h camera.h
build/charger.o:   common.h charger.h
build/bluetooth.o: common.h bluetooth.h
build/motor_controller.o: common.h motor_controller.h motor.h


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

dude-%: build/%.hex
	avrdude -p x16a4 -c avrispmkII -P usb -U $< -qq

v-dude-%: build/%.hex
	avrdude -p x16a4 -c avrispmkII -P usb -U $< -v

clean:
	rm -f build/*.o build/*.ls build/*.lis build/*.dbg build/*.mp build/*.s build/*.lst build/*.out

cleanall:
	rm -f build/*

.PHONY: all rebuild
.PHONY: clean cleanall
