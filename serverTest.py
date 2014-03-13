from gui.gui import *
from gui.KeyManager import *
from robot.command import *
from thread import *
from Queue import Queue
from gamepad.controller import RobotController
from socketEndpoint import Server, Client

from serialize import Serialize

import sys

def main():
    robot_side_out = Queue()
    robot_side_in = Queue()

    print(sys.argv)

    robServer = Server(sys.argv[1], int(sys.argv[2]), robot_side_out.get, robot_side_in.put)

    r = motorManager(robot_side_in, robot_side_out)

    c = start_new_thread(r.read_inputs, ())
    t = start_new_thread(r.check_timeouts, ())

    robServer.start()
    while(r.is_active):
        pass

    r.shut_off()

if __name__ == '__main__':
    main();
