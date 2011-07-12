class Camera:
  # Takes in the serial_port it should write and read from.
  def __init__(self, serial_port):
    pass
  # Sends one command and reads the result then returns 'transferring' or 'idle'.
  def next(self):
    return 'idle'
  # Initialises everything and takes the picture.
  def start(self):
    return 'taking'
