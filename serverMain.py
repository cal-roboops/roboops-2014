from robot.command import *
from thread import *
from Queue import Queue
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

    try:
        r = motorManager(robot_side_in, robot_side_out, sys.argv[2], sys.argv[3])
    except:
        r = motorManager(robot_side_in, robot_side_out, "", "")
        print("No arduino specified. Running testing state. It goes 'arm controller' 'drive controller'")

    try:
        robServer = Server('0.0.0.0', int(sys.argv[1]), lambda : robot_side_out.get(block=True, timeout=1), robot_side_in.put, r.shut_off)
    except:
        print("No port specified. Please specify the fucking port.")
        return

    c = start_new_thread(r.read_inputs, ())

    robServer.start()

    while(robServer.isOn):
        pass
        sleep(0.001)

    r.shut_off()
    robServer.close()

    print("Robot communications has exited.")

if __name__ == '__main__':
    main();
