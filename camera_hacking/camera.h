#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <stdlib.h>

#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h>


/** Reset camera. */
int camera_reset(int fd);

void dump_contents(unsigned char *rxed, int len);

int camera_init(char * port);

int camera_write(int fd, unsigned char *buffer, unsigned int len);

int camera_read(int fd, unsigned char *buffer, unsigned int len);

int camera_close(int fd);

int camera_get_filesize(int fd);

int camera_get_image(int fd, int filesize);

int
camera_get_block(int fd, unsigned int address, unsigned char chunksize);


int camera_snap(int fd);

int camera_stop(int fd);

int serial_write(int fd, unsigned char *buffer, unsigned int length);

int serial_read(int fd, unsigned char *buffer, unsigned int length);

int camera_set_baud(int serial_fd, int baud);

int reconfigure_baud(int fd, int baud);

#endif /* CAMERA_H */
