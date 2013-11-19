from gui.gui import *
from gui.KeyManager import *
from robot.command import *
from thread import *
from Queue import Queue
from gamepad.controller import RobotController
from socketEndpoint import Server, Client

from serialize import Serialize

def main():
    control_side_out = Queue()
    control_side_in = Queue()

    conClient = Client('localhost', 8050, control_side_out.get, control_side_in.put)

    g = Gui(control_side_in, control_side_out)
    z = RobotController(0, control_side_in)

    b = start_new_thread(g.read_inputs, ())
    j = start_new_thread(z.update_loop, ())
    conClient.start()
    a = g.gui_loop()

    z.shut_off()

if __name__ == '__main__':
    main()
