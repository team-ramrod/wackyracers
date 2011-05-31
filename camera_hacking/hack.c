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
void dump_contents(const unsigned char *rxed, int len)
{
    int i = 0;
    while (i < len) {
        fprintf(stderr, "element %d = %X\n", i, rxed[i]);
        i++;
    }
}

/** Check for the correct return sequence from the camera. */
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

/* Send read block command */
int camera_get_block(int fd, unsigned int *addr, unsigned int len,
                      unsigned char *buff)
{

    unsigned char interval = 0x0a;

    unsigned char cmd[] = {0x56, 0x00, 0x32, 0x0c, 0x00, 0x0a, 0x00, 0x00,
                           *addr >> 8, *addr & 0xff, 0x00, 0x00,
                           len >> 8,  len & 0xff, 0x00, interval};

    serial_write(fd, cmd, sizeof(cmd));

    /* Get data */
    serial_read(fd, buff, len);
    
    /* Get footer */
    serial_read(fd, cmd, 5);

    *addr += len;

    return(0);
}

/* Must sleep for 2-3 seconds after camera power on. */
int main(int argc, char *argv[])
{
    char rx_buffer[MAX_MESSAGE_LENGTH];
    memset(&rx_buffer, 3, MAX_MESSAGE_LENGTH);

    unsigned char *compressed_image = NULL;
    compressed_image = (unsigned char *) malloc(640 * 480 * 1000 * 2);
    /* Default block size should be f8. */
    unsigned int blocksize = 0xf8;
    /* Default interval should be 0x0A */
    unsigned char interval = 0x0a;
    unsigned int addr = 0;

    /* WARNING this is probably not setting the speed correctly */
    int serial_fd = serial_open("/dev/ttyU1", B38400);

    camera_reset(serial_fd);
    usleep(1000000);
    camera_start_image(serial_fd);
    usleep(1000000);


    uint16_t filesize = camera_get_filesize(serial_fd);
    fprintf(stderr, "DEBUG: filesize = %d\n", filesize);


    unsigned char cmd[] =  {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,
                            addr >> 8, addr, 0x00, 0x00,
                            blocksize >> 8, blocksize, 0x00, interval};
    while (1) {


                            
        serial_write(serial_fd, cmd, sizeof(cmd));
        int bytes_rxed = serial_read(serial_fd, rx_buffer, MAX_MESSAGE_LENGTH);
        addr += blocksize;
        fprintf(stderr, "bytes_rxed: %d\n", bytes_rxed);
        dump_contents(rx_buffer, bytes_rxed + 5);
    //         fprintf(stderr, "cmd is: %s\n");
        dump_contents(cmd, sizeof(cmd));
        fprintf(stderr, "addr is %d\n", addr);
    }



    camera_stop_image(serial_fd);
    usleep(1000000);
    
    close(serial_fd);
    
    return 0;
}
