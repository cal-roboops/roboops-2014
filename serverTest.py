from gui.gui import *
from gui.KeyManager import *
from robot.command import *
from thread import *
from Queue import Queue
from gamepad.controller import RobotController
from socketEndpoint import Server, Client

from serialize import Serialize

import sys

"""
    First argument is port
    Second argument is port of arm controller
    Third argument is port of drive controller
"""

def main():
    robot_side_out = Queue()
    robot_side_in = Queue()

    robServer = Server('0.0.0.0', int(sys.argv[1]), robot_side_out.get, robot_side_in.put)

    r = motorManager(robot_side_in, robot_side_out, sys.argv[2], sys.argv[3])

    c = start_new_thread(r.read_inputs, ())
    t = start_new_thread(r.check_timeouts, ())
    #c = start_new_thread(r.update, ())

    robServer.start()
    while(r.is_active):
        pass

    r.shut_off()

if __name__ == '__main__':
    main();
