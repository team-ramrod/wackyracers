import time

# Stolen from http://stackoverflow.com/questions/472977/binary-data-with-pyserialpython-serial-port
def a2s(arr):
    """ Array of integer byte values --> binary string
    """
    return ''.join(chr(b) for b in arr)

class Camera(object):
    
    # Takes in the serial_port it should write and read from.
    def __init__(self, serial_port):
            self.ser = serial_port
            self.DEBUG = False

    # Sends one command and reads the result then returns 'transferring' or 'idle'.
    def next_(self):
        
        if (self.address < self.filesize):
            self.image_buffer = self.get_block(self.address, self.chunksize, 
                self.image_buffer)
            self.address += self.chunksize
            return float(self.address) / self.chunksize
        else:
            self.outfile.write(self.image_buffer)
            self.outfile.close()
            self.reset() # Erase last image
            address = 0
            return 'idle'
        
    # Initialises everything and takes the picture.
    def start(self, filename):
        
        self.address = 0
        self.chunksize = 16
        self.image_buffer = ''
        self.outfile = open(filename, 'w')
        
        self.start_image() # Picture snapped.
        
        self.filesize = self.get_filesize()
        
        return 0

    def reset(self):
        cmd = [0x56, 0x00, 0x26, 0x00]
        data = a2s(cmd)
        self.ser.write(data)

        response = self.ser.read(4)
        #sys.stderr.write('DEBUG: reset returned:\n')
        #print_response(response)

        # Read serial data until we see 'Init end', see LinkSprite manual, pg 7
        # TODO timeout
        buffer = [''] * 8 # Pre-allocate to fit 'Init end'
        while ''.join(buffer) != 'Init end':
            for i in range(len(buffer)-1):
                buffer[i] = buffer[i+1]
            buffer[7] = self.ser.read()
        #print 'DEBUG: %s seen' % ('Init end')
        # grab the CR and LF tacked on the end so that they don't interface
        # with anything else. Could just have a flush fucntion.
        self.ser.read(2)

        # Manual says we must sleep for 2-3 seconds now
        #print "Sleeping for 3 seconds..."
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
