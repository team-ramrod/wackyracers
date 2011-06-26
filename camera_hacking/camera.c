#include "camera.h"

/** Reset camera. */
int camera_reset(int fd)
{
    //unsigned char CAM_RESET_RET[] = {0x76, 0x00, 0x26, 0x00};
    
    unsigned char cmd[] = {0x56, 0x00, 0x26, 0x00};
    camera_write(fd, cmd, 4);
    
    unsigned char response[4];
    camera_read(fd, response, 4);

    /* DEBUG */
    fprintf(stderr, "camera_reset: camera returned:\n");
    dump_contents(response, 4);

    /* HACK flush line */
    int i = 0;
    while (i < 67) {
        unsigned char c;
        camera_read(fd, &c, 1);
        //printf("DEBUG: c = %c\n", c);
        i++;
    }

    printf("sleeping for 3 seconds after reset...\n");
    usleep(3000000);
    
    return 0;
}

int camera_snap(int fd)
{
    uint8_t cmd[] = {0x56, 0x00, 0x36, 0x01, 0x00};
    camera_write(fd, cmd, 5);

    unsigned char response[5];
    camera_read(fd, response, 5);

    /* DEBUG */
    fprintf(stderr, "camera_snap: camera returned:\n");
    dump_contents(response, 5);

    return 0;
}

/** Get a data chunk from the camera. */
int camera_get_block_extern(int fd, unsigned int address,
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
    serial_write(fd, cmd, sizeof(cmd));
    //serial_read(serial_fd, cmd, sizeof(cmd));

    //serial_read(serial_fd, buff, blocksize);

    /* Get footer and throw away. */
    //serial_read(serial_fd, cmd, sizeof(cmd));

    /* Read the full response. */
    /* WARNING coupled to above function. */
    uint8_t response[16 + 10];
    serial_read(fd, response, sizeof(response));

    /* Copy data (from between headers) into global image buffer. */
    /* WARNING last argument is coupled to camera_get_image */
    //bcopy((response + 5), &global_image_buffer[address], 16);

    dump_contents(response, sizeof(response));
    //fprintf(stderr, "contents of glob_im_buff:\n");
    //dump_contents(global_image_buffer, 26*3);

    return 0;
}

/** Write buffer contents to serial port.
    @return Number of bytes txed or -1 on error. */
int serial_write(int fd, unsigned char *buffer, unsigned int length)
{
    int nbytes = write(fd, buffer, length);

    if (!nbytes) {
        perror("ERROR in serial_write: Unable to send data");
        return -1;
    }

    if (nbytes < length) {
        perror("ERROR in serial_write: Incomplete transmission");
        return -1;
    }

    return nbytes;
}

/** Read serial data into buffer.
    @return Number of bytes rxed or -1 on error.
    @note Blocks.
 */
int serial_read(int fd, unsigned char *buffer, unsigned int length)
{
    int nbytes = read(fd, buffer, length);

    if (!nbytes) {
        perror("ERROR in serial_read: Unable to read data");
        return -1;
    }

    if (nbytes < length) {
        fprintf(stderr, "WARNING in serial_read: Requested %d bytes, received only %d.\n", length, nbytes);
    }

    return nbytes;
}

int camera_stop(int fd)
{
    unsigned char cmd[] = {0x56, 0x00, 0x36, 0x01, 0x03};
    camera_write(fd, cmd, 5);

    unsigned char response[5];
    camera_read(fd, response, 5);

    /* DEBUG */
    fprintf(stderr, "camera_stop: camera returned:\n");
    dump_contents(response, 5);

    return 0;
}

int camera_get_filesize(int fd)
{
    unsigned char cmd[] = {0x56, 0x00, 0x34, 0x01, 0x00};
    camera_write(fd, cmd, 5);

    unsigned char response[9];
    camera_read(fd, response, 9);

    /* DEBUG */
    fprintf(stderr, "camera_get_filesize: camera returned:\n");
    dump_contents(response, 9);

    int filesize = (*(response + 7) << 8) + *(response + 8);
    
    return filesize;
}

int camera_get_image(int fd, int filesize)
{
    int chunksize = 16;
    int address = 0;
    //unsigned char[] image_buffer = (*uint32_t)malloc(30000*1);

    while (address < filesize) {
        camera_get_block(fd, address, chunksize);
        address += chunksize;
        printf("address = %d\n", address);
    }

    return 0;
    
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

    camera_write(serial_fd, cmd, sizeof(cmd));
    camera_read(serial_fd, response, sizeof(response));

#ifdef DEBUG
    fprintf(stderr, "camera_set_baud returned:\n");
    dump_contents(response, sizeof(response));
    fprintf(stderr, "\n");
#endif
    return 0;
}

int
camera_get_block(int fd, unsigned int address, unsigned char chunksize)
{
    int ret = 0;
    unsigned char interval = 0x0A; // Don't touch.

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
    cmd[12] = (chunksize >> 8) & 0xFF;
    cmd[13] = (chunksize >> 0) & 0xFF;
    cmd[14] = (interval >> 8) & 0xFF;
    cmd[15] = (interval >> 0) & 0xFF;

    camera_write(fd, cmd, sizeof(cmd));

    unsigned char response[16+10];
    int x = camera_read(fd, response, 16+10);
    if (x < 16+10) {
        ret = -1;
    }
    dump_contents(response, sizeof(response));

//     /* Get header */
//     unsigned char header[5];
//     camera_read(fd, header, 5);
//     /* DEBUG */
//     fprintf(stderr, "header:\n");
//     dump_contents(header, 5);
// 
//     /* Get data */
//     unsigned char data_block[16];
//     camera_read(fd, data_block, 16);
//     /* DEBUG */
//     fprintf(stderr, "data:\n");
//     dump_contents(data_block, 16);
// 
//     /* Get footer */
//     unsigned char footer[5];
//     camera_read(fd, footer, 5);
//     /* DEBUG */
//     fprintf(stderr, "footer:\n");
//     dump_contents(footer, 5);

    //return data_block;
    return ret;
}

/** Dump rxed message to screen. */
void dump_contents(unsigned char *rxed, int len)
{
    int i = 0;
    while (i < len) {
        fprintf(stderr, "%d = 0x%X\n", i, rxed[i]);
        i++;
    }
}

int reconfigure_baud(int fd, int baud)
{
    struct termios options;

    tcgetattr(fd, &options);

    options.c_ispeed = baud;
    options.c_ospeed = baud;

    //tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);
}

/** @return A file descriptor. */
int camera_init(char * port)
{
    
    if (port == NULL) {
        port = "/dev/cuaU1"; // Default port.
    }

    volatile int fd;
    struct termios options;

    fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (fd == -1) {
        perror("Unable to open port");
    }
    else {
        fcntl(fd, F_SETFL, 0); // Set descriptor status flags to 0.
    }

    tcgetattr(fd, &options);

    /* Set up serial options */

    options.c_cflag |= CLOCAL | CREAD;
    
    /* Raw mode */
    /* NOTE needs this block of stuff to work! */
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ISIG | IEXTEN | ECHOCTL | ECHOKE);
    options.c_oflag &= ~(OPOST);
    options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IGNBRK | PARMRK);
    
    /* Set baud */
    options.c_ispeed = B38400;
    options.c_ospeed = B38400;
    
    options.c_cflag &= ~(CSIZE);
    options.c_cflag |= CS8;

    options.c_iflag &= ~(INPCK | ISTRIP);
    options.c_cflag &= ~(PARENB | PARODD);

    options.c_cflag &= ~(CSTOPB);

    /* Timeout */
    options.c_cc[VTIME] = 20;
    options.c_cc[VMIN] = 0;

    /* Apply options. */
    //tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    return fd;
}

int camera_write(int fd, unsigned char *buffer, unsigned int len)
{
    int nbytes = write(fd, buffer, len);
    
    if (!nbytes) {
        perror("ERROR in serial_write: Unable to send data");
        return -1;
    }
    
    if (nbytes < len) {
        perror("ERROR in serial_write: Incomplete transmission");
        return -1;
    }

    return nbytes;
}

int camera_read(int fd, unsigned char *buffer, unsigned int len)
{
    int nbytes = read(fd, buffer, len);
    
    if (!nbytes) {
        perror("ERROR in serial_read: Unable to read data");
        return -1;
    }

    if (nbytes < len) {
        fprintf(stderr, "WARNING in serial_read: Requested %d bytes, received only %d.\n", len, nbytes);
    }

    return nbytes;
}

int camera_close(int fd)
{
    close(fd);

    return 0;
}
