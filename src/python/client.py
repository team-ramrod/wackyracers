import sys, tty, termios, select
from camera import Camera
from serial import Serial, SerialException

def getc():
  return sys.stdin.read(1)

def key_pressed():
  return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

class Client:
  def __init__(self):
    self.bt = Serial();
    self.bt.port = '/dev/cu.Group_2_are_shit-SPP'
    self.bt.baudrate = 57600
    self.bt.timeout = 1
    self.camera = Camera(self.bt)

    fd = sys.stdin.fileno()
    tty.setcbreak(sys.stdin.fileno())

    try:
      self.bt.open()
    except SerialException, e:
      sys.stderr.write("Could not open serial port %s: %s\n" % (self.bt.portstr, e)) 
      sys.exit(1)

  def send_command(self, c):
    self.bt.write(c + '\001')

  def run(self):
    while 1:
      if key_pressed():
        c = getc()
        if c == 'e' and self.camera_state == 'idle':
          self.camera_state = self.camera.start()
        else:
          self.send_command(c)
      else:
        self.camera_state = self.camera.next_()

if __name__ == '__main__':
  Client().run()
