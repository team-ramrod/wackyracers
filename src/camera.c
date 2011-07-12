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

void camera_set_baudrate(uint32_t baudrate)
{
    uint8_t cmd[] = {0x56, 0x00, 0x24, 0x03, 0x01, 0x00, 0x00};
    
    switch (baudrate) {
        case 9600:
            cmd[5] = 0xAE;
            cmd[6] = 0xC8;
            break;
        case 19200:
            cmd[5] = 0x56; 
            cmd[6] = 0xE4;
            break;
        default:
            /* Default 38400 baud. */
            cmd[5] = 0x2A;
            cmd[6] = 0xF2;
            break;
    }
    
    camera_write(cmd, 7);

    unsigned char response[5];
    camera_read(response, 5);
    
    //fprintf(stream_board, "response from change baud:\n");
    //dump_contents(response, 5);
    
}

/** Read from camera. 
    @param buffer Read buffer with sufficient space.
    @param length Length of expected read string.
    @return Always zero. */
int camera_read(unsigned char *buffer, unsigned int len)
{
    //fprintf(stream_board, "inside camera_read\n\n");
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
        fprintf(stream_board, "%d = 0x%X\n", i, rxed[i]);
        i++;
    }
}

/** Take picture. Must be called BEFORE camera_get_filesize().
    @return Always zero. */
int camera_snap(void)
{
#ifdef DEBUG_SNAP
    fprintf(stream_board, "calling camera_snap\n");
#endif
    uint8_t cmd[] = {0x56, 0x00, 0x36, 0x01, 0x00};
    camera_write(cmd, 5);

    unsigned char response[5];
    camera_read(response, 5);

    /* DEBUG */
#ifdef DEBUG_SNAP
    fprintf(stream_board, "expecting:\n0x76\n0x00\n0x36\n0x00\n0x00\n\n");
    fprintf(stream_board, "camera_snap: camera returned:\n");
    dump_contents(response, 5);
#endif

    return 0;
}

/** Get a data chunk from the camera. */
int camera_get_block(unsigned int address,
                     unsigned int blocksize)
{

#ifdef DEBUG_GETBLOCK
    fprintf(stream_board, "Calling get_block(), blocksize = %d\n", blocksize);
#endif

    // Interval time. XX XX * 0.01m[sec]
    uint16_t interval = 0x0A; /* Put here out of reach intentionally. */

    uint8_t cmd[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,
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
    
    unsigned char response[blocksize + 10];
    camera_write(cmd, 16);
    
<<<<<<< .mine
//     /* "proper" code follows, debug is above. */
//     
//     /* Get header. */
//     unsigned char header[5];
//     camera_read(header, 5);
//     
// #ifdef DEBUG_GETBLOCK
//     fprintf(stream_board, "DEBUG: header is: \n");
//     dump_contents(header, 5);
// #endif
//     
//     /* Get data block. */
//     unsigned char datablock[blocksize];
//     camera_read(datablock, blocksize);
//     
// #ifdef DEBUG_GETBLOCK
//     fprintf(stream_board, "DEBUG: data is: \n");
//     dump_contents(datablock, 16);
// #endif
//     
//     /* Get footer. */
//     unsigned char footer[5];
//     camera_read(footer, 5);
//     
// #ifdef DEBUG_GETBLOCK
//     fprintf(stream_board, "DEBUG: footer is: \n");
//     dump_contents(footer, 5);
// #endif
=======
    /* Get header. */
    unsigned char header[5];
    camera_read(header, 5);
    
    /* Get data block. */
    unsigned char datablock[blocksize];
    camera_read(datablock, blocksize);
    
    /* Get footer. */
    unsigned char footer[5];
    camera_read(footer, 5);

#ifdef DEBUG_GETBLOCK
    fprintf(stream_board, "DEBUG: header is: \n");
    dump_contents(header, 5);
#endif
#ifdef DEBUG_GETBLOCK
    fprintf(stream_board, "DEBUG: data is: \n");
    dump_contents(datablock, 16);
#endif    
#ifdef DEBUG_GETBLOCK
    fprintf(stream_board, "DEBUG: footer is: \n");
    dump_contents(footer, 5);
#endif
>>>>>>> .r456
 
    /* TODO save the data block! */
    
    //fprintf(stream_board, "DEBUG: response is: \n");
    //dump_contents(response, 6);

//     fprintf(stream_board, "finished\n");
//     dump_contents(response, 26);
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
        camera_get_block(address, blocksize);
        address += blocksize;
        fprintf(stream_board, "address = %d\n", address);
    }
    
    return 0;
}

/** Get filesize of image. Must be called AFTER camera_start_image().
    @return File size in bytes. */
uint32_t camera_get_filesize(void)
{
    
#define DEBUG_FILESIZE
#ifdef DEBUG_FILESIZE
    fprintf(stream_board, "Calling get_filesize()");
#endif
    unsigned char cmd[] = {0x56, 0x00, 0x34, 0x01, 0x00};
    camera_write(cmd, 5);

    unsigned char response[9];
    camera_read(response, 9);

    /* DEBUG */
#ifdef DEBUG_FILESIZE
    fprintf(stream_board, "expecting:\n0x76\n0x00\n0x34\n0x00\n0x40\n0x00\n0x00\n0xXX\n0xXX\n\n");
    fprintf(stream_board, "camera_get_filesize: camera returned:\n");
    dump_contents(response, 9);
#endif

    int filesize = (*(response + 7) << 8) + *(response + 8);
    
#ifdef DEBUG_FILESIZE
    fprintf(stream_board, "i.e., filesize = %d\n", filesize);
#endif
    return filesize;
}

int camera_stop_image(void)
{
    unsigned char cmd[] = {0x56, 0x00, 0x36, 0x01, 0x03};
    camera_write(cmd, 5);

    unsigned char response[5];
    camera_read(response, 5);

    /* DEBUG */
    fprintf(stream_board, "camera_stop: camera returned:\n");
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
// #ifdef DEBUG_RESET
//     fprintf(stream_board, "expecting:\n0x76 0x00 0x26 0x00\n\n");
//     fprintf(stream_board, "camera_reset: camera returned:\n");
//     dump_contents(response, 4);
// #endif
    
//     uint8_t expected[4] = {0x76, 0x00, 0x26, 0x00};
//     uint8_t k;
//     for (k = 0; k <=3; k++) {
//         if (response[k] != expected[k])
//             camera_reset();
//     }

    /* HACK flush line */
    int i = 0;
    while (i < 67) {
        unsigned char c;
        camera_read(&c, 1);
        i++;
    }
#ifdef DEBUG_RESET
    fprintf(stream_board, "Init string seen\n\n");
#endif
    
#ifdef DEBUG_RESET
    fprintf(stream_board, "sleeping for 3 seconds after reset...\n\n");
#endif
    _delay_ms(3000);
    
    return 0;
}

