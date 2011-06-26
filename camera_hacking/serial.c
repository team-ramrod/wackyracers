#include "serial.h"

/** Open a serial port for communication.
    @param baud A baud speed from termios.h
    @return A file descriptor on success or -1 on error. */
int
serial_open(const char *device, int baud)
{

    struct termios options; /* Termios options */
    
    int fd; /* File descriptor */

    fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (fd == -1) {
        perror("Unable to open port");
    }
    else {
        fcntl(fd, F_SETFL, 0); /* Set descriptor status flags to 0. */
    }

    /* Make the read calls non-blocking. */
    /* WARNING Don't do this. */
    //fcntl(fd, F_SETFL, FNDELAY);
    
    tcgetattr(fd, &options);
    options.c_cflag |= CLOCAL | CREAD;
    
     // Raw mode.
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ISIG | IEXTEN);
    options.c_oflag &= ~OPOST;
    options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IGNBRK);

    /* Set baud */
    options.c_ispeed = B38400;
    options.c_ospeed = B38400;

    /* Setup char len */
    //options.c_cflag &= ~(CSIZE);
    options.c_cflag |= CS8;

    /* Setup stopbits (1)*/
    //options.c_cflag &= ~(CSTOPB);

    /* No parity */
    //options.c_iflag &= ~(INPCK | ISTRIP);
   // options.c_cflag &= ~(PARENB | PARODD);

    /* No flow control */
    //options.c_iflag &= ~(IXON | IXOFF | IXANY);
    //options.c_cflag &= ~(CRTSCTS);

    /* timeout */
    //options.c_cc[VTIME] = 20; // ???

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);
    
    return (fd);
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

int
serial_send_command()
{
    
}
