"""Command pattern implementation for robot actions"""

import pickle

class Command():
    def __init__(self, *args):
        self.args = args
        
    def dump(self):
        return pickle.dumps(self)

def run(command):
    cmd_object = pickle.loads(command)
    cmd_object.run()

class Turn(Command):
    def __init__(self, angle):
        self.angle = angle
    
    def run(self):
        print("turning", self.angle)
        
if __name__ == "__main__":
    # command is created from the user side
    turn = Turn(10)
    s = turn.dump()
    
    # s is sent over the network to the robot
    # the robot runs
    run(s)
