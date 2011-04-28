CC = avr-gcc
LIB = 
CFLAGS = -Wall -mmcu=atxmega16a4
LFLAGS = -mmcu=atxmega16a4

MCU=atxmega16a4

TARGET = ATXMega16
SN = :

CAM_FILES = main-cam camera bluetooth
CAM_OBJS = $(addsuffix .o, $(CAM_FILES))

MOTOR_FILES = main-motor motor camera bluetooth
MOTOR_OBJS = $(addsuffix .o, $(MOTOR_FILES))

all: build-camera build-motor

main-motor.o: motor.h common.h
main-cam.o: bluetooth.h camera.h common.h

motor.o: motor.h common.h
camera.o: camera.h common.h
bluetooth.o: bluetooth.h common.h

rebuild: clean build

#%.o: %.c
#	$(CC) $(CFLAGS) -c $<

build-camera: $(CAM_OBJS)
	$(CC) -o camera $(LFLAGS) $(CAM_OBJS)

build-motor: $(MOTOR_OBJS)
	$(CC) -o motor $(LFLAGS) $(MOTOR_OBJS)

program-camera-stk:
	stk600 -cUSB$(SN) -d$(TARGET) -ms -e -pf -ifcamera.hex

program-camera-dude:
	avrdude ...? camera.hex

program-motor-stk:
	stk600 -cUSB$(SN) -d$(TARGET) -ms -e -pf -ifmotor.hex

program-motor-dude:
	avrdude ...? motor.hex

clean:
	rm -f *.o *.ls *.lis *.dbg *.mp *.s *.lst *~ 

cleanall: clean
	rm -f camera motor

.PHONY : clean cleanall program-camera program-motor
