import serial
import time
import sys

# Stolen from http://stackoverflow.com/questions/472977/binary-data-with-pyserialpython-serial-port
def a2s(arr):
    """ Array of integer byte values --> binary string
    """
    return ''.join(chr(b) for b in arr)

# @param block string
def print_response(block, n=0):
    if n == 0:
        n = len(block)
    byte_array = [c for c in block]
    i = 0
    for i in range(0, n):
        print "Byte %d: %s" % (i, hex(ord(block[i])))

class Camera(object):

    def __init__(self, port):
        self.ser = serial.Serial()
        self.ser.port = port
        self.ser.baudrate = 38400
        self.ser.timeout = 1
        self.DEBUG = False

        try:
            self.ser.open()
        except serial.SerialException, e:
            sys.stderr.write("Could not open serial port %s: %s\n" % (self.ser.portstr, e))
            sys.exit(1)

    def check_response(self, response, expected):
        pass

    def reset(self):
        cmd = [0x56, 0x00, 0x26, 0x00]
        data = a2s(cmd)
        self.ser.write(data)

        response = self.ser.read(4)
        sys.stderr.write('DEBUG: reset returned:\n')
        print_response(response)

        # Read serial data until we see 'Init end', see LinkSprite manual, pg 7
        # TODO timeout
        buffer = [''] * 8 # Pre-allocate to fit 'Init end'
        while ''.join(buffer) != 'Init end':
            for i in range(len(buffer)-1):
                buffer[i] = buffer[i+1]
            buffer[7] = self.ser.read()
        print 'DEBUG: %s seen' % ('Init end')
        # grab the CR and LF tacked on the end so that they don't interface
        # with anything else. Could just have a flush fucntion.
        self.ser.read(2)

        # Manual says we must sleep for 2-3 seconds now
        print "Sleeping for 3 seconds..."
        time.sleep(3)

    def start_image(self):
        cmd = [0x56, 0x00, 0x36, 0x01, 0x00]
        data = a2s(cmd)
        self.ser.write(data)

        # Read the response from camera.
        response = self.ser.read(5)
        expected = [0x76, 0x00, 0x36, 0x00, 0x00]

        return (0 if [ord(r) for r in response] == expected else -1)

    def stop_image(self):
        cmd = [0x56, 0x00, 0x36, 0x01, 0x03]
        data = a2s(cmd)
        self.ser.write(data)

        # Read the response from camera.
        response = self.ser.read(5)
        expected = [0x76, 0x00, 0x36, 0x00, 0x00]

        return (0 if [ord(r) for r in response] == expected else -1)

    def get_filesize(self):
        """ Get filesize of image. Can only be called after start_image()

            @return filesize in bytes
        """
        cmd = [0x56, 0x00, 0x34, 0x01, 0x00]
        data = a2s(cmd)
        self.ser.write(data)

        x = self.ser.read(7)
        msb = self.ser.read()
        msb = ord(msb)
        lsb = self.ser.read()
        lsb = ord(lsb)

        # Convert to filsize in bytes:
        filesize = (msb << 8) + lsb

        return filesize

    def set_baud(self):
        pass

    def get_image(self, filesize):
        chunksize = 16
        address = 0
        image_buffer = ''

        while (address < filesize):
            image_buffer = self.get_block(address, chunksize, image_buffer)
            address += chunksize

        return image_buffer

    def get_block(self, address, chunksize, image_buffer):
        interval = 0x0A     # Put here out of reach intentionally.

        cmd =  [0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,
                0x00, # MH
                0x00, # ML
                0x00, 0x00,
                0x00, # KH
                0x00, # KL
                0x00, # XH
                0x00,]

        cmd[8] = (address >> 8) & 0xFF
        cmd[9] = (address >> 0) & 0xFF
        cmd[12] = (chunksize >> 8) & 0xFF
        cmd[13] = (chunksize >> 0) & 0xFF
        cmd[14] = (interval >> 8) & 0xFF
        cmd[15] = (interval >> 0) & 0xFF

        data = a2s(cmd)

        self.ser.write(data)

        # Get header
        header = self.ser.read(5)
        if self.DEBUG:
            print 'header: '
            print_response(header)

        # Get data
        data = self.ser.read(chunksize)
        if self.DEBUG:
            print 'data: '
            print_response(data)

        # Get footer
        footer = self.ser.read(5)
        if self.DEBUG:
            print 'footer: '
            print_response(footer)

        return image_buffer + data

    def close(self):
        self.ser.close()

if __name__ == "__main__":
    print 'cannot run this standalone'


    