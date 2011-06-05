#include <stdio.h>
#include <stdlib.h>

#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h>

#include "serial.h"

//#define DEBUG

#define MAX_MESSAGE_LENGTH 120

unsigned char * global_image_buffer;

unsigned char CAM_RESET_CMD[] = {0x56, 0x00, 0x26, 0x00};
unsigned char CAM_RESET_RET[] = {0x76, 0x00, 0x26, 0x00};

/** Dump rxed message to screen. */
void dump_contents(unsigned char *rxed, int len)
{
    int i = 0;
    while (i < len) {
        fprintf(stderr, "element %d = %X\n", i, rxed[i]);
        i++;
    }
}

/** Check for the correct return sequence from the camera. */
int camera_check_retcode(uint8_t buffer[], uint8_t retcode[], int len)
{
    int i;

    for (i = 0; i < len; i++) {
        if (buffer[i] != retcode[i]) {
            return -1;
        }
    }
    return 0;
}

/** Reset the camera device. */
int camera_reset(int serial_fd)
{
    unsigned char rx_buffer[MAX_MESSAGE_LENGTH];
    memset(&rx_buffer, 0, MAX_MESSAGE_LENGTH);
    
    serial_write(serial_fd, CAM_RESET_CMD, 4);
    serial_read(serial_fd, rx_buffer, 4);
#ifdef DEBUG
    fprintf(stderr, "camera_reset returned:\n");
    dump_contents(rx_buffer, 4);
    fprintf(stderr, "\n");
#endif
    return 0;
}

/** Take a picture */
int camera_start_image(int serial_fd) 
{
    uint8_t cmd[] = {0x56, 0x00, 0x36, 0x01, 0x00};
    uint8_t ret[] = {0x76, 0x00, 0x36, 0x00, 0x00};
    uint8_t response[sizeof(ret)];
    
    serial_write(serial_fd, cmd, sizeof(cmd));
    serial_read(serial_fd, response, sizeof(response));

    int result = camera_check_retcode(response, ret, sizeof(ret));
    if (result < 0) {
        fprintf(stderr, "ERROR uexpected response in camera_start_image\n");
        return -1;
    }
#ifdef DEBUG
    fprintf(stderr, "camera_start_image returned:\n");
    dump_contents(rx_buffer, 5);
    fprintf(stderr, "\n");
#endif
    return 0;
}

/** Stop taking an image.
    @note Still yet to figure out exactly what this means. */
int camera_stop_image(int serial_fd)
{
    uint8_t cmd[] = {0x56, 0x00, 0x36, 0x01, 0x03};
    uint8_t ret[] = {0x76, 0x00, 0x36, 0x00, 0x00};
    uint8_t response[sizeof(ret)];
    
    serial_write(serial_fd, cmd, sizeof(cmd));
    serial_read(serial_fd, response, sizeof(response));

    int result = camera_check_retcode(response, ret, sizeof(ret));
    if (result < 0) {
        fprintf(stderr, "ERROR uexpected response in camera_stop_image\n");
        return -1;
    }
#ifdef DEBUG
    fprintf(stderr, "camera_stop_image returned:\n");
    dump_contents(rx_buffer, 5);
    fprintf(stderr, "\n");
#endif
    return 0;
}

uint16_t
camera_get_filesize(int serial_fd)
{
    uint8_t cmd[] = {0x56, 0x00, 0x34, 0x01, 0x00};
    /* NOTE Last two bytes of ret represent filesize, they are set to 0x00 to
     * keep the correct return length. */
    uint8_t ret[]= {0x76, 0x00, 0x34, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00};
    uint8_t response[sizeof(ret)];
    
    serial_write(serial_fd, cmd, sizeof(cmd));
    serial_read(serial_fd, response, sizeof(response));

    /* NOTE Non-standard use of camera_check_response. See above */
    int result = camera_check_retcode(response, ret, 7);
    if (result < 0) {
        fprintf(stderr, "ERROR uexpected response in camera_get_filesize\n");
        return -1;
    }
#ifdef DEBUG
    fprintf(stderr, "camera_get_filesize returned:\n");
    dump_contents(rx_buffer, 9);
    fprintf(stderr, "\n");
#endif
    uint16_t size;
    size = ((uint8_t)response[7] << 8) + (uint8_t)response[8];
    return size;
}

/** Set camera baud rate.
    @param baud An integer BUT be sure to enter only POSIX valid speeds.
    @return -1 on error, 0 on success. */
int camera_set_baud(int serial_fd, int baud)
{
    uint8_t cmd[] = {0x56, 0x00, 0x24, 0x03, 0x01, 0x00, 0x00};
    uint8_t ret[] = {0x76, 0x00, 0x24, 0x00, 0x00};
    uint8_t response[5];

    switch (baud) {
        case 9600:
            cmd[5] = 0xAE;
            cmd[6] = 0xC8;
            break;
        case 19200:
            cmd[5] = 0x56;
            cmd[6] = 0xE4;
            break;
        case 38400:
            cmd[5] = 0x2A;
            cmd[6] = 0xF2;
            break;
        case 57600:
            cmd[5] = 0x1C;
            cmd[6] = 0x4C;
            break;
        case 115200:
            cmd[5] = 0x0D;
            cmd[6] = 0xA6;
            break;
        default:
            fprintf(stderr, "ERROR invalid baud");
            return -1;
    }

    serial_write(serial_fd, cmd, sizeof(cmd));
    serial_read(serial_fd, response, sizeof(response));

    int result = camera_check_retcode(response, ret, sizeof(ret));
    if (result < 0) {
        fprintf(stderr, "ERROR uexpected response in camera_set_baud\n");
        return -1;
    }
#ifdef DEBUG
    fprintf(stderr, "camera_set_baud returned:\n");
    dump_contents(response, sizeof(response));
    fprintf(stderr, "\n");
#endif
    return 0;
}

/** Get an image from the camera.
    @param serial_fd Serial connection file descriptor.
    @param buffer Pre-allocated buffer to store image data.
    @return */
int camera_get_image(int serial_fd, unsigned char *buffer, uint16_t filesize)
{
    unsigned char data[16];
    uint16_t m = 0; // Starting address.
    //uint16_t k = 0xf8; // Chunk size.
    uint16_t k = sizeof(data);

    while (m < filesize) {
        camera_get_block(serial_fd, m, k, data);
        m += sizeof(data); /* Bump up address. */
        fprintf(stderr, "DEBUG: m = %d\n", m);
    }
    return 0;
}

/** Get a data chunk from the camera. */
int camera_get_block(int serial_fd, unsigned int address,
                     unsigned int blocksize, unsigned char *buff)
{
    // Interval time. XX XX * 0.01m[sec]
    unsigned char interval = 0x0A; /* Put here out of reach intentionally. */

    unsigned char cmd[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,
                           0x00, // MH
                           0x00, // ML
                           0x00, 0x00,
                           0x00, // KH
                           0x00, // KL
                           0x00, // XH
                           0x00, // XL
    };

    cmd[8] = (address >> 8) & 0xFF;
    cmd[9] = (address >> 0) & 0xFF;
    cmd[12] = (blocksize >> 8) & 0xFF;
    cmd[13] = (blocksize >> 0) & 0xFF;
    cmd[14] = (interval >> 8) & 0xFF;
    cmd[15] = (interval >> 0) & 0xFF;

    /* Send command and get header, then throw away. */
    serial_write(serial_fd, cmd, sizeof(cmd));
    //serial_read(serial_fd, cmd, sizeof(cmd));

    //serial_read(serial_fd, buff, blocksize);

    /* Get footer and throw away. */
    //serial_read(serial_fd, cmd, sizeof(cmd));

    /* Read the full response. */
    uint8_t response[26];
    serial_read(serial_fd, response, sizeof(response));

    /* Copy data (from between headers) into global image buffer. */
    bcopy((response + 5), &global_image_buffer[address], 16);
    
    dump_contents(response, sizeof(response));
    //fprintf(stderr, "contents of glob_im_buff:\n");
    //dump_contents(global_image_buffer, 26*3);

    return 0;
}

/* Must sleep for 2-3 seconds after camera power on. */
int main(int argc, char *argv[])
{
    /* Set up global image buffer. */
    global_image_buffer = malloc(600*240*100);
    memset(global_image_buffer, 0, sizeof(global_image_buffer));
    
    char rx_buffer[MAX_MESSAGE_LENGTH];
    memset(&rx_buffer, 3, MAX_MESSAGE_LENGTH);

    uint16_t filesize = 0;

    /* WARNING this is probably not setting the speed correctly */
    int serial_fd = serial_open("/dev/ttyU1", B38400);

    /* Sleep for 3 seconds after reset. */
    camera_reset(serial_fd);
    fprintf(stderr, "INFO: sleeping for 3 seconds after resetting...");
    usleep(3000000);
    
    /* set cam baud */
    camera_set_baud(serial_fd, 38400);

    /* Take picture */
    camera_start_image(serial_fd);

    /* Get the filesize of picture. */
    filesize = camera_get_filesize(serial_fd);
    fprintf(stderr, "DEBUG: filesize = %d\n", filesize);

    camera_get_image(serial_fd, global_image_buffer, 25000);

    fprintf(stderr, "RXed data:\n");
    int i;
    for (i = 0; i < 40; i++) {
        fprintf(stderr, "image[%d] = %x\n",i, global_image_buffer[i]);
    }

    camera_stop_image(serial_fd);
    
    close(serial_fd);
    
    return 0;
}


// 

// 

