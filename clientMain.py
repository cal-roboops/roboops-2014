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
    """
    1 = ip
    2 = port
    """
    control_side_out = Queue()
    control_side_in = Queue()

    try:
        conClient = Client(sys.argv[1], int(sys.argv[2]), lambda : control_side_out.get(block=True, timeout=1), control_side_in.put)
    except:
        print("Please specifiy ip address and port.")
        return

    gui = Gui(control_side_in, control_side_out)
    controller_1 = RobotController(0, control_side_in)
    controller_1.set_arm_mode()

    controller_2 = RobotController(1, control_side_in)
    controller_2.set_drive_mode()

    gui_input_thread_id = start_new_thread(gui.read_inputs, ())
    controller_1_thread_id = start_new_thread(controller_1.update_loop, ())
    controller_2_thread_id = start_new_thread(controller_2.update_loop, ())
    conClient.start()
    gui_thread_id = gui.gui_loop()

    controller_1.shut_off()
    controller_2.shut_off()
    conClient.close()

    print("Command center has exited.")

if __name__ == '__main__':
    main()
