from gui.gui import *
from gui.KeyManager import *
from robot.command import *
from thread import *
from Queue import Queue
from gamepad.controller import RobotController

from serialize import Serialize

class pseudoComLink():
    def __init__(self, queue_in, queue_out) :
        self.queue_in = queue_in
        self.queue_out = queue_out
        self.is_active = True
        print("ComLink online.")

    def read_inputs(self, queue):
        while(self.is_active) :
            self.queue_in.put(queue.get())
           # print("Transmission received!")

    def transmit_outputs(self, queue):
        while(self.is_active) :
            queue.put(self.queue_out.get())
            #print("Transmission sent!")

    def shut_off(self) :
        self.is_active = False

def main():
    control_side_out = Queue()
    control_side_in = Queue()
    robot_side_out = Queue()
    robot_side_in = Queue()

    control_side_buffer = Queue()
    robot_side_buffer = Queue()

    g = Gui(control_side_in, control_side_out)
    r = motorManager(robot_side_in, robot_side_out)
    z = RobotController(0, control_side_out)
    robot_side_com_link = pseudoComLink(robot_side_in, robot_side_out)
    control_side_com_link = pseudoComLink(control_side_in, control_side_out)


    b = start_new_thread(g.read_inputs, ())
    c = start_new_thread(r.read_inputs, ())
    t = start_new_thread(r.check_timeouts, ())
    j = start_new_thread(z.update_loop, ())
    d = start_new_thread(control_side_com_link.read_inputs, (control_side_buffer,))
    e = start_new_thread(control_side_com_link.transmit_outputs, (robot_side_buffer,))
    f = start_new_thread(robot_side_com_link.read_inputs, (robot_side_buffer,))
    h = start_new_thread(robot_side_com_link.transmit_outputs, (control_side_buffer,))
    a = g.gui_loop()

    while(g.is_active()):
        pass
    control_side_com_link.shut_off()
    robot_side_com_link.shut_off()
    r.shut_off()
    z.shut_off()

if __name__ == '__main__':
    main()
