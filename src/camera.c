#include "camera.h"

/** Write to camera. 
    @param buffer Byte array to send.
    @param length Length of byte array.
    @return Always zero. */
int camera_write(unsigned char *buffer, unsigned int len)
{
    int i = 0;
    for (i = 0; i < len; i++) {
        fputc(buffer[i], stream_cam);
    }

    return 0;
}

/** Read from camera. 
    @param buffer Read buffer with sufficient space.
    @param length Length of expected read string.
    @return Always zero. */
int camera_read(unsigned char *buffer, unsigned int len)
{
    //int nbytes = read(fd, buffer, len);
    int i;
    for (i = 0; i < len; i++) {
        buffer[i] = fgetc(stream_cam);
    }
        
    return 0;
}

int camera_close(void)
{
    return 0;
}

/** Dump rxed message to screen. */
void dump_contents(unsigned char *rxed, int len)
{
    int i = 0;
    while (i < len) {
        fprintf(stream_debug, "%d = 0x%X\n", i, rxed[i]);
        i++;
    }
}

/** Take picture. Must be called BEFORE camera_get_filesize().
    @return Always zero. */
int camera_snap(void)
{
    uint8_t cmd[] = {0x56, 0x00, 0x36, 0x01, 0x00};
    camera_write(cmd, 5);

    unsigned char response[5];
    camera_read(response, 5);

    /* DEBUG */
    fprintf(stream_debug, "camera_snap: camera returned:\n");
    dump_contents(response, 5);

    return 0;
}

/** Get a data chunk from the camera. */
int camera_get_block(unsigned int address,
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

    /* Get header. */
    unsigned char header[5];
    camera_read(header, 5);
    
    /* Get data block. */
    unsigned char datablock[16];
    camera_read(datablock, 16);
    
    /* Get footer. */
    unsigned char footer[5];
    camera_read(footer, 5);
 
    /* TODO save the data block! */
    
    fprintf(stream_debug, "DEBUG: data block is: \n");
    dump_contents(datablock, 16);

    return 0;
}

/** Get image from camera.
    NOTE imcomplete, returns nothing (0) currently. */
int camera_get_image(unsigned int filesize)
{
    unsigned int blocksize = 16;
    unsigned int address = 0x00;
    /* TODO image buffer */
    
    while (address < filesize) {
        camera_get_block(address, blocksize, NULL);
        address += blocksize;
    }
    
    return 0;
}

/** Get filesize of image. Must be called AFTER camera_start_image().
    @return File size in bytes. */
unsigned int camera_get_filesize(void)
{
    unsigned char cmd[] = {0x56, 0x00, 0x34, 0x01, 0x00};
    camera_write(cmd, 5);

    unsigned char response[9];
    camera_read(response, 9);

    /* DEBUG */
    fprintf(stream_debug, "camera_get_filesize: camera returned:\n");
    dump_contents(response, 9);

    int filesize = (*(response + 7) << 8) + *(response + 8);
    
    return filesize;
}

int camera_stop_image(void)
{
    unsigned char cmd[] = {0x56, 0x00, 0x36, 0x01, 0x03};
    camera_write(cmd, 5);

    unsigned char response[5];
    camera_read(response, 5);

    /* DEBUG */
    fprintf(stream_debug, "camera_stop: camera returned:\n");
    dump_contents(response, 5);

    return 0;
}

/** Reset camera. */
int camera_reset(void)
{   
    unsigned char cmd[] = {0x56, 0x00, 0x26, 0x00};
    camera_write(cmd, 4);
    
    unsigned char response[4];
    camera_read(response, 4);

    /* DEBUG */
    fprintf(stream_debug, "camera_reset: camera returned:\n");
    dump_contents(response, 4);

    /* HACK flush line */
    int i = 0;
    while (i < 67) {
        unsigned char c;
        camera_read(&c, 1);
        //printf("DEBUG: c = %c\n", c);
        i++;
    }

    printf("sleeping for 3 seconds after reset...\n");
    _delay_ms(3000);
    
    return 0;
}

