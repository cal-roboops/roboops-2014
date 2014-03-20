from gui.gui import *
from gui.KeyManager import *
from robot.command import *
from thread import *
from Queue import Queue
from gamepad.controller import RobotController
from socketEndpoint import Server, Client

from serialize import Serialize

from constants.constants import *

import sys

def main():
    control_side_out = Queue()
    control_side_in = Queue()

    conClient = Client(sys.argv[1], int(sys.argv[2]), control_side_out.get, control_side_in.put)

    g = Gui(control_side_in, control_side_out)
    z = RobotController(0, control_side_in)
    z.set_drive_mode()

    b = start_new_thread(g.read_inputs, ())
    z = start_new_thread(z.update_loop, ())
    conClient.start()
    a = g.gui_loop()

    z.shut_off()

if __name__ == '__main__':
    main()
