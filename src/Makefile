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
CFLAGS += -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -std=gnu99 -pedantic

# Generate dependency information
CFLAGS += -MMD -MP -MF .dep/$(@F).d

ifeq      "$(strip $(DEBUG_LEVEL))" "VERBOSE"
	CFLAGS += -DVERBOSE_ENABLE
else ifeq "$(strip $(DEBUG_LEVEL))" "DEBUG"
	CFLAGS += -DDEBUG_ENABLE
else ifeq "$(strip $(DEBUG_LEVEL))" "ERROR"
	CFLAGS += -DERROR_ENABLE
else ifndef DEBUG_LEVEL
	CFLAGS += -DERROR_ENABLE
endif

ifdef DEBUG_MODULE
	CFLAGS += -D'DEBUG_ONLY="$(DEBUG_MODULE)"'
endif

ifdef DEBUG_UART
	CFLAGS += -D'DEBUG_UART=stream_$(DEBUG_UART)'
endif

LDFLAGS += -mmcu=$(MCU)

all: build/camera.hex build/motor.hex
rebuild: cleanall all


#############
##  Files  ##
#############
CAM_FILES              = main-cam   uart_comms led debug dastardly
MOTOR_FILES            = main-motor uart_motor led button commander chassis ir charger keyboard debug

build/camera.elf: $(addprefix build/, $(addsuffix .o, $(CAM_FILES)))
build/motor.elf:  $(addprefix build/, $(addsuffix .o, $(MOTOR_FILES)))

## Testing files

LED_TEST_FILES         = test led button
MOTOR_TEST_FILES       = motor_test uart_motor led button motor motor_controller charger ir commander keyboard debug
MOTOR_UART_TEST_FILES  = motor_uart_test uart led charger motor motor_controller
CAM_UART_TEST_FILES    = cam_uart_test uart led 
UART_TEST_FILES        = uart_test uart led
CAM_TEST_FILES         = cam_test led uart_comms debug 
MOTOR_TEST2_FILES      = motor_test2 led uart_motor debug

CAMERA_BYPASS_FILES = camera_bypass uart_comms debug

build/test.elf:   $(addprefix build/, $(addsuffix .o, $(LED_TEST_FILES)))
build/motor_test.elf: $(addprefix build/, $(addsuffix .o, $(MOTOR_TEST_FILES)))
build/motor_uart_test.elf: $(addprefix build/, $(addsuffix .o, $(MOTOR_UART_TEST_FILES)))
build/cam_uart_test.elf: $(addprefix build/, $(addsuffix .o, $(CAM_UART_TEST_FILES)))
build/uart_test.elf: $(addprefix build/, $(addsuffix .o, $(UART_TEST_FILES)))
build/cam_test.elf: $(addprefix build/, $(addsuffix .o, $(CAM_TEST_FILES)))
build/motor_test2.elf: $(addprefix build/, $(addsuffix .o, $(MOTOR_TEST2_FILES)))

build/camera_bypass.elf: $(addprefix build/, $(addsuffix .o, $(CAMERA_BYPASS_FILES)))

###############################
##  Internal Implementation  ##
###############################
build/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

build/%.hex: build/%.elf
	$(OBJCOPY) -O ihex $< $@

build/%.elf:
	$(CC) $(LDFLAGS) -o $@ $^

dude-%: build/%.hex
	avrdude -p x16a4 -c avrispmkII -P usb -U $< -qq

v-dude-%: build/%.hex
	avrdude -p x16a4 -c avrispmkII -P usb -U $< -v

clean:
	rm -f build/*.o build/*.ls build/*.lis build/*.dbg build/*.mp build/*.s build/*.lst build/*.elf

cleanall:
	rm -f build/*
	rm -f .dep/*

.PHONY: all rebuild
.PHONY: clean cleanall

# Include the dependency files, should be the last of the makefile
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
