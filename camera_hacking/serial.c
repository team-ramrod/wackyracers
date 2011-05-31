#include "serial.h"

/** Open a serial port for communication.
    @param baud A baud speed from termios.h
    @return A file descriptor on success or -1 on error. */
int
serial_open(const char *device, int baud)
{
    int fd; /* File descriptor */

    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("Unable to open port");
    }
    else {
        fcntl(fd, F_SETFL, 0); /* Set descriptor status flags to 0. */
    }
    
    /* Set serial port baud. */
    struct termios options;
    tcgetattr(fd, &options);
    cfsetspeed(&options, baud);
    tcsetattr(fd, TCSANOW, &options);
    
    return (fd);
}

/** Write buffer contents to serial port. */
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
    
    return 1;
}

/** Read serial data into buffer. 
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
        return -1;
    }
    
    return 1;
}

int
serial_send_command()
{
    
}
