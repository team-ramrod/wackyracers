#ifndef CAMERA_H
#define CAMERA_H

#include <util/delay.h>

#include "common.h"
#include "uart_comms.h"
#include "uart_common.h"

#define DEBUG_RESET
#define DEBUG_SNAP
#define DEBUG_FILESIZE
#define DEBUG_GETBLOCK

void camera_set_baudrate(uint32_t baudrate);

/** Write to camera. 
    @param buffer Byte array to send.
    @param length Length of byte array.
    @return Always zero. */
int camera_write(unsigned char *buffer, unsigned int len);

/** Read from camera. 
    @param buffer Read buffer with sufficient space.
    @param length Length of expected read string.
    @return Always zero. */
int camera_read(unsigned char *buffer, unsigned int len);

/** Dump rxed message to screen. */
void dump_contents(unsigned char *rxed, int len);

/** Take picture. Must be called BEFORE camera_get_filesize().
    @return Always zero. */
int camera_snap(void);

int camera_close(void);

/** Get a data chunk from the camera. */
int camera_get_block(unsigned int address,
                     unsigned int blocksize);

/** Get image from camera.
    NOTE imcomplete, returns nothing (0) currently. */
int camera_get_image(unsigned int filesize);

/** Get filesize of image. Must be called AFTER camera_start_image().
    @return File size in bytes. */
uint32_t camera_get_filesize(void);

int camera_stop_image(void);

/** Reset camera. */
int camera_reset(void);


#endif /* CAMERA_H */
