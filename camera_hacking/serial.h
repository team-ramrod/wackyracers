#ifndef SERIAL_H
#define SERIAL_H

#include <stdio.h>
#include <stdlib.h>

#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h>

/** Open a serial port for communication.
    @return A file descriptor on success or -1 on error. */
int serial_open(const char *device, int baud);

/** Write buffer contents to serial port. */
int serial_write(int fd, unsigned char *buffer, unsigned int length);

/** Read serial data into buffer. 
    @note Blocks.*/
int serial_read(int fd, unsigned char *buffer, unsigned int length);

int serial_send_command();

#endif /* SERIAL_H */
