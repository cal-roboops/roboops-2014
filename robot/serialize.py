"""Command pattern implementation for robot actions"""

import pickle

def run_robot(pickle, robot):
    pickle.loads(pickle).run_robot(robot)

def run_gui(pickle, gui):
    pickle.loads(pickle).run_gui(gui)
    

class Command():
    def __init__(self, *args):
        self.args = args
        
    def dump(self):
        return pickle.dumps(self)

class Turn(Command):
    def __init__(self, angle):
        self.angle = angle
    
    def run_robot(self, robot):
        print("turning", self.angle)

    def run_gui(self, gui):
        print("outputting to gui: " + self.angle)

class Motor(Command):
    def __init__(self, number, speed):
        self.number = number
        self.speed = speed

    def robot_run(self, robot):
        print("Setting " + self.number + " to " + speed)

    def run_gui(self, gui):
        print("outputting to gui: " + self.number + " at " + self.speed)
        if gui:
            gui.output("Motor " + self.number + " set to " + self.speed)
            gui.update_readout(self.number, self.speed)

class RawMotor(Command):
    def __init__(self, number, speed):
        self.number = number
        self.speed = speed

    def robot_run(self, robot):
        assert False, "Robot should not be running this"

    def run_gui(self, gui):
        if gui:
            gui.queue_out.put(Motor(self.number, self.speed*gui.slider_of_motor[self.number].get()))

        
if __name__ == "__main__":
    # command is created from the user side
    turn = serialize.Turn(100)
    
    # s is sent over the network to the robot
    # the robot runs
    Command.run_robot(turn, None)
