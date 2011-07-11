#!/usr/bin/env python
"""Treedisk_plot1 V0.01
Copyright (c) 2011 Michael P. Hayes, UC ECE, NZ

Usage: treedisk_plot1 [options] files
"""

"""
options:
outfile
port
"""
import getopt
from camera import *

def capture(camera, filename):

    outfile = open('images/' + filename, 'w')

    camera.start_image()

    filesize = camera.get_filesize()
    print 'Filesize = %d B' % filesize

    print 'Saving image... Please wait.'
    image_buffer = camera.get_image(filesize)

    camera.stop_image()

    outfile.write(image_buffer)
    print 'Image written successfully.'
    
    outfile.close()

def main (argv = None):

    if argv is None:
        argv = sys.argv

    # Create a camera instance
    mycam = Camera('/dev/cu.Group_2_are_shit-SPP')

    #Reset the camera
    mycam.reset()

    cmd = 'cc'
    while (cmd != 'quit'):
        cmd = raw_input('Enter command: ')
        if cmd == 'capture':
            filename = raw_input('Enter filename to save: ')
            print 'Press enter to take picture'
            x = raw_input("# ")
            capture(mycam, filename)
        else:
            print 'invalid command'

    mycam.close()

if __name__ == "__main__":
    sys.exit (main())
