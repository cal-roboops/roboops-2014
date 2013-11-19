"""Command pattern implementation for robot actions"""

import pickle

from Queue import Queue

def run_robot(pickled, robot):
    try:
        pickle.loads(pickled).run_robot(robot)
    except Exception as ex:
        print(ex)

def run_gui(pickled, gui):
    pickle.loads(pickled).run_gui(gui)

def unpickle(pickled):
    return pickle.loads(pickled)
    

class Command():
    def __init__(self, *args):
        self.args = args

    def run_robot(self, robot):
        pass

    def run_gui(self, gui):
        pass
        
    def dump(self):
        return pickle.dumps(self)

class Turn(Command):
    def __init__(self, angle):
        self.angle = angle
    
    def run_robot(self, robot):
        print("turning", str(self.angle))

    def run_gui(self, gui):
        print("outputting to gui: " + str(self.angle))

class Motor(Command):
    def __init__(self, number, speed):
        self.number = number
        self.speed = speed

    def run_robot(self, robot):
        #print("Setting " + str(self.number) + " to " + str(self.speed))
        robot.update_port(self.number, self.speed)
        robot.queue_out.put(self.dump())
        
    def run_gui(self, gui):
        #print("outputting to gui: " + str(self.number) + " at " + str(self.speed))
        if gui:
            gui.output("Motor " + str(self.number) + " set to " + "{0:.2f}".format(self.speed))
            gui.update_readout(self.number, self.speed)

#created by controller, sent to GUI to process and add slider multiplier
class RawMotor(Command):
    def __init__(self, type, button, slider, motor, speed):
        self.number = motor
        self.speed = speed
        self.type = type
        self.button = button
        self.slider = slider

    def run_robot(self, robot):
        assert False, "Robot should not be running this"

    def run_gui(self, gui):
        #print("outputting to gui, raw: " + str(self.number) + " at " + str(self.speed))
        if gui:
            gui.update_controller(self.type, self.button, self.speed)
            gui.queue_out.put(Motor(self.number, self.speed*gui.slider_of_motor[self.slider].get()).dump())
            gui.output("Controller reading: " + str(self.number) + " for " + "{0:.2f}".format(self.speed))

        
if __name__ == "__main__":
    # command is created from the user side
    a = Queue()
    b = Queue()
    a.put(Turn(100).dump())
    b.put(a.get())

    run_robot(b.get(), None)
