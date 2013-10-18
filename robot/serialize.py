"""Command pattern implementation for robot actions"""

import pickle

class Command():
    def __init__(self, *args):
        self.args = args

    def run_robot(self, robot):
        pass

    def run_gui(self, gui):
        pass
        
    def dump(self):
        temp_pickle = pickle.dumps(self)
        temp_pickle.run_robot = self.run_robot
        temp_pickle.run_gui = self.run_gui
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

#created by controller, sent to GUI to process and add slider multiplier
class RawMotor(Command):
    def __init__(self, number, speed):
        self.number = number
        self.speed = speed

    def robot_run(self, robot):
        assert 1 = 0, "Robot should not be running this"

    def run_gui(self, gui):
        if gui:
            gui.queue_out.put(Motor(self.number, self.speed*gui.slider_of_motor[self.number].get()))

        
if __name__ == "__main__":
    # command is created from the user side
    turn = Turn(100)
    
    # turn is sent over to robot
    # the robot runs
    turn.run_robot(None)
