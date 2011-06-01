#include <stdio.h>
#include <stdlib.h>

#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h>

#include "serial.h"

#define DEBUG

#define ARRAY_SIZE(array) (sizeof(array) / sizeof (array[0]))

#define MAX_MESSAGE_LENGTH 120

/* Camera commands:
 * RESET: reset the camera device
 * STARTPIC: take a picture
 * SIZE: read JPEG file size
 */
unsigned char CAM_RESET_CMD[] = {0x56, 0x00, 0x26, 0x00};
unsigned char CAM_RESET_RET[] = {0x76, 0x00, 0x26, 0x00};
unsigned char CAM_STARTIMAGE_CMD[] = {0x56, 0x00, 0x36, 0x01, 0x00};
unsigned char CAM_STARTIMAGE_RET[] = {0x76, 0x00, 0x36, 0x00, 0x00};
unsigned char CAM_STOPIMAGE_CMD[] = {0x56, 0x00, 0x36, 0x01, 0x03};
unsigned char CAM_STOPIMAGE_RET[] = {0x76, 0x00, 0x36, 0x00, 0x00};
unsigned char CAM_SIZE_CMD[] = {0x56, 0x00, 0x34, 0x01, 0x00};
unsigned char CAM_SIZE_RET[] = {0x76, 0x00, 0x34, 0x00, 0x04, 0x00, 0x00};

/** Dump rxed message to screen. */
void dump_contents(unsigned char *rxed, int len)
{
    int i = 0;
    while (i < len) {
        fprintf(stderr, "element %d = %X\n", i, rxed[i]);
        i++;
    }
}

/** Check for the correct return sequence from the camera.
    @todo */
int camera_check_retcode(char * buffer, int len, unsigned char retcode[])
{
    return 1;
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
    
    return camera_check_retcode(rx_buffer, 4, CAM_RESET_RET);
}

/** Take a picture */
int camera_start_image(int serial_fd) 
{
    unsigned char rx_buffer[MAX_MESSAGE_LENGTH];
    memset(&rx_buffer, 0, MAX_MESSAGE_LENGTH);
    
    serial_write(serial_fd, CAM_STARTIMAGE_CMD, 5);
    serial_read(serial_fd, rx_buffer, 5);
#ifdef DEBUG
    fprintf(stderr, "camera_start_image returned:\n");
    dump_contents(rx_buffer, 5);
    fprintf(stderr, "\n");
#endif
    
    return camera_check_retcode(rx_buffer, 5, CAM_STARTIMAGE_RET);
}

/** Stop taking an image.
    @note Still yet to figure out exactly what this means. */
int camera_stop_image(int serial_fd)
{
    unsigned char rx_buffer[MAX_MESSAGE_LENGTH];
    memset(&rx_buffer, 0, MAX_MESSAGE_LENGTH);
    
    serial_write(serial_fd, CAM_STOPIMAGE_CMD, 5);
    serial_read(serial_fd, rx_buffer, 5);  
#ifdef DEBUG
    fprintf(stderr, "camera_stop_image returned:\n");
    dump_contents(rx_buffer, 5);
    fprintf(stderr, "\n");
#endif
    
    return camera_check_retcode(rx_buffer, 5, CAM_STOPIMAGE_RET);
}

uint16_t
camera_get_filesize(int serial_fd)
{
    unsigned char rx_buffer[MAX_MESSAGE_LENGTH];
    memset(&rx_buffer, 0, MAX_MESSAGE_LENGTH);
    
    serial_write(serial_fd, CAM_SIZE_CMD, 5);
    serial_read(serial_fd, rx_buffer, 9);
#ifdef DEBUG
    fprintf(stderr, "camera_get_filesize returned:\n");
    dump_contents(rx_buffer, 9);
    fprintf(stderr, "\n");
#endif
    
    /* NOTE: Non-standard use of camera_check_retcode here. Only the first 7 
     * bytes are passed for checking as the last two bytes are the image size 
     * and will be different for each image. */
    if (camera_check_retcode(rx_buffer, 7, CAM_SIZE_RET)) {
        uint16_t size;
        fprintf(stderr, "DEBUG: rx_buffer[7] << 8 = %d\n", rx_buffer[7] << 8);
        fprintf(stderr, "DEBUG: rx_buffer[8] = %x\n", rx_buffer[8]);
        size = ((uint8_t)rx_buffer[7] << 8) + (uint8_t)rx_buffer[8];
        return size;
    }
    else {
        return -1;
    }
}

/** Get an image from the camera.
    @param serial_fd Serial connection file descriptor.
    @param buffer Pre-allocated buffer to store image data.
    @return */
int camera_get_image(int serial_fd, unsigned char *buffer)
{
    unsigned char cmd[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,
                           0x00, // MH
                           0x00, // ML
                           0x00, 0x00,
                           0x00, // KH
                           0x00, // KL
                           0x00, // XH
                           0x00, // XL
    };

    unsigned char data[32]; 
    uint16_t m = 0; // Starting address.
    uint16_t k = sizeof(data); // Chunk size.
    uint16_t x = 10; // Interval time. XX XX * 0.01m[sec]

    while (m < 1000) { // for testing
        cmd[8] = (m >> 8) & 0xFF;
        cmd[9] = (m >> 0) & 0xFF;
        cmd[12] = (k >> 8) & 0xFF;
        cmd[13] = (k >> 0) & 0xFF;
        cmd[14] = (x >> 8) & 0xFF;
        cmd[15] = (x >> 0) & 0xFF;

        serial_write(serial_fd, cmd, sizeof(cmd));
        //unsigned char chunk[5+5+32];
        //serial_read(serial_fd, chunk, sizeof(chunk));

//         unsigned char header[5];
//         serial_read(serial_fd, header, sizeof(header));
// 
//         serial_read(serial_fd, data, sizeof(data));
//         fprintf(stderr, "data dump:\n");
//         dump_contents(data, sizeof(data)-1);
// 
//         unsigned char footer[5];
//         serial_read(serial_fd, footer, sizeof(footer));

        m += sizeof(data); /* Bump up address. */

        fprintf(stderr, "DEBUG: m = %d\n", m);
    }

    return 0;
        
}

/* Must sleep for 2-3 seconds after camera power on. */
int main(int argc, char *argv[])
{
    char rx_buffer[MAX_MESSAGE_LENGTH];
    memset(&rx_buffer, 3, MAX_MESSAGE_LENGTH);

    unsigned char image_buffer[600*240*100];
    memset(image_buffer, 0, sizeof(image_buffer));

    /* WARNING this is probably not setting the speed correctly */
    int serial_fd = serial_open("/dev/ttyU1", B38400);

    camera_reset(serial_fd);
    usleep(1000000);
    /* set cam baud */
    unsigned char baud_cmd[] = {0x56, 0x00, 0x24, 0x03, 0x01, 0x2A, 0xF2};
    serial_write(serial_fd, baud_cmd, sizeof(baud_cmd));
    camera_start_image(serial_fd);
    usleep(1000000);



    uint16_t filesize = camera_get_filesize(serial_fd);
    fprintf(stderr, "DEBUG: filesize = %d\n", filesize);

    int ret = camera_get_image(serial_fd, image_buffer);

    camera_stop_image(serial_fd);
    usleep(100);

    printf("rxed image:\n");
    int i = 0;
    while (*(image_buffer + i) != 0) {
        printf("%x, ", image_buffer[i]);
        i++;
    }
    
    close(serial_fd);
    
    return 0;
}

//     unsigned char cmd1[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,
//                             0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x0A};
// 
//     unsigned char cmd2[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,
//                             0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x0A};
// 
//     unsigned char cmd3[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,
//                             0x00, 0x40, 0x00, 0x00, 0x00, 0x20, 0x00, 0x0A};
// 
//     unsigned char cmd4[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,
//                             0x00, 0x60, 0x00, 0x00, 0x00, 0x20, 0x00, 0x0A};
