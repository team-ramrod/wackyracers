import sys, tty, termios, select, os, threading, time
from camera import Camera
from serial import Serial, SerialException
from mutex import mutex

CMD_NONE             = '\000'
CMD_FORWARD          = '\001'
CMD_BACK             = '\002'
CMD_DASTARDLY_IR_OFF = '\003'
CMD_LEFT             = '\004'
CMD_DASTARDLY_IR_ON  = '\005'
CMD_RIGHT            = '\010'
CMD_STOP             = '\020'
CMD_ASSUME_CTRL      = '\040'
CMD_CAM_OFF          = '\100'
CMD_CAM_ON           = '\200'

translation = {
  'q': CMD_ASSUME_CTRL,
  'w': CMD_FORWARD,
  'a': CMD_LEFT,
  'x': CMD_BACK,
  'd': CMD_RIGHT,
  's': CMD_STOP,
  '[': CMD_CAM_OFF,
  ']': CMD_CAM_ON
}

def getc():
  return sys.stdin.read(1)

def key_pressed():
  return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

class CamResetThread( threading.Thread ):
  def __init__(self, send_command):
    self.send_command = send_command
    threading.Thread.__init__(self)

  def run(self):
    print 'Resetting camera...'
    self.send_command('[')
    time.sleep(4)
    self.send_command(']')
    print '...Camera reset.'

class Client:
  def __init__(self):
    self.bt = Serial();
    self.bt.port = '/dev/cu.Group_2_are_shit-SPP'
    self.bt.baudrate = 57600
    self.bt.timeout = 1
    self.camera = Camera(self.bt)
    self.camera_state = 'idle'
    self.mut = mutex()

    fd = sys.stdin.fileno()
    tty.setcbreak(sys.stdin.fileno())

    try:
      self.bt.open()
    except SerialException, e:
      sys.stderr.write("Could not open serial port %s: %s\n" % (self.bt.portstr, e)) 
      sys.exit(1)

  def send_command(self, c):
    while not self.mut.testandset():
      pass
    if c in translation:
      #print 'writing [%d]' % ord(translation[c])
      self.bt.write(translation[c] + '\001')
    self.mut.unlock()

  def get_filename(self):
    i = 1
    while os.path.exists('images/%i.jpg' % i):
      i += 1
    return 'images/%i.jpg' % i

  def run(self):
    print 'Connected, driving enabled.'
    running = True
    while running or self.camera_state != 'idle':
      if key_pressed():
        c = getc()
        if c == 'e' and self.camera_state == 'idle':
          print 'Starting to take a picture, driving disabled...'
          current_filename = self.get_filename()
          self.camera_state = self.camera.start(current_filename)
          print '...picture taken, driving enabled.  Transferring image...'
        elif ord(c) == 27: # Escape
          running = False
        elif c == '\\':
          CamResetThread( self.send_command ).start()
        else:
          #print 'character [%c,%d] pushed'%(c,ord(c))
          self.send_command(c)
      elif self.camera_state is not 'idle':
        self.camera_state = self.camera.next_()
        if self.camera_state == 'idle':
          print '...image transferred, saved to [' + current_filename + '].'

if __name__ == '__main__':
  Client().run()
