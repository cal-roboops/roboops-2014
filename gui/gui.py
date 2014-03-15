from Tkinter import *
from random import randint
from KeyManager import *
from constants import *

from Queue import Queue

from serialize import Serialize

import pickle

WIDTH = 1600
HEIGHT = 900

COLOR_0 = "#000044"
COLOR_1 = "#000022"
COLOR_3 = "#000022"
COLOR_4 = "#6666ff"
COLOR_5 = "#4444dd"
COLOR_6 = "#000000"

class Outputter(Frame):
    _rows = 0

    def __init__(self, root):

        Frame.__init__(self, root)
        self.canvas = Canvas(root, borderwidth=0, highlightthickness=0, background=COLOR_1, width = WIDTH//10, height = 2*HEIGHT//4)
        self.frame = Frame(self.canvas, background=COLOR_1, width = WIDTH//10, height = 2*HEIGHT//4)
        self.scroll = Scrollbar(root, orient="vertical", command=self.canvas.yview)
        self.scroll.config(bg=COLOR_1,troughcolor=COLOR_5,bd=0,elementborderwidth=0)
        self.canvas.configure(yscrollcommand=self.scroll.set)

        self.canvas.create_window((1,1), window=self.frame, anchor="sw", tags="self.frame")

        self.frame.bind("<Configure>", self.OnFrameConfigure)

        self.txt = StringVar()
        self.txt.set("Ohai")

        self.textBox = Label(self.frame, textvariable = self.txt,bg=COLOR_1,fg=COLOR_4,justify=LEFT).grid(row=0,column=0, sticky=W)


    def set_grid_canvas(self, **kwargs):
        self.canvas.grid(**kwargs)
    
    def set_grid_scroll(self, **kwargs):
        self.scroll.grid(**kwargs)

    def add_line(self, txt):
        #Label(self.frame, text = txt, bg=COLOR_1, fg=COLOR_4).grid(row=self._rows, column=0, sticky=W)
        self.txt.set(self.txt.get() + "\n" + txt);
        self._rows += 1

        if(self._rows > 100) :
            self._rows = 0
            self.txt.set("")

    def OnFrameConfigure(self, event):
        '''Reset the scroll region to encompass the inner frame'''
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))

class Readouts(LabelFrame):
    def __init__(self, parent, txt = "Undef, yo"):
        LabelFrame.__init__(self, parent, text="",width=WIDTH//10)
        self._read_labels_outputs = []
        self._read_labels = {}
        self._read_labels_len = 0
        self.config(bg=COLOR_0, bd=0,highlightbackground=COLOR_6,highlightthickness=2)

        Label(self, text = txt, bg = COLOR_0, fg=COLOR_5).grid(row = 0, column = 0, columnspan = 2, sticky=EW)

    def set_grid(self, **kwargs):
        self.grid(**kwargs)

    def add_label(self, name, readout):
        n_readout = StringVar()
        n_readout.set(readout)
        self._read_labels_outputs.append(n_readout)
        Label(self, text = name, bg = COLOR_0, fg=COLOR_5, width = WIDTH//200, justify=LEFT).grid(row = self._read_labels_len + 1, column = 0)
        Label(self, textvariable = n_readout, bg = COLOR_0, fg=COLOR_5, width = WIDTH//200, justify=RIGHT).grid(row = self._read_labels_len + 1, column = 1)
        self._read_labels[name] = self._read_labels_len
        self._read_labels_len += 1
        return n_readout

    def update_label(self, name, new_readout):
        assert name in self._read_labels, name + " does not exist."
        i = self._read_labels[name]
        self._read_labels_outputs[i].set(new_readout)

    def get_value(self, name) :
        assert name in self._read_labels, name + " does not exist."
        return self._read_labels[name]

class Sliders(LabelFrame):
    _num_sliders = 0
    _sliders = {}

    def __init__(self, parent, txt = "Undef, yo"):
        LabelFrame.__init__(self, parent, text="")
        self.config(bg=COLOR_0, bd=0,highlightbackground=COLOR_6,highlightthickness=2)
        Label(self, text = txt, bg = COLOR_0, fg=COLOR_5).grid(row = 0, column = 0)

    def set_grid(self, **kwargs):
        self.grid(**kwargs)

    def add_slider(self, name, min, max):
        Label(self, text = name, bg=COLOR_0, fg=COLOR_5).grid(row = self._num_sliders*2 + 1, column = 0, sticky=EW)
        slid = Scale(self, from_=min, to=max, orient=HORIZONTAL,length=WIDTH//5, borderwidth=0, bg=COLOR_0, fg=COLOR_5, highlightthickness=0,sliderrelief=FLAT,activebackground=COLOR_4,troughcolor=COLOR_6,tickinterval=int((max-min)/4))
        slid.grid(row = self._num_sliders*2 + 2, column = 0,sticky=E)
        self._sliders[name] = slid
        self._num_sliders += 1
        return slid

    def get_value(self, name):
        assert self._sliders[name], "Does not exist."
        return self._sliders[name].get()

class CommandLine(Entry):

    def __init__(self, parent, queue_out, queue_in) :
        Entry.__init__(self, parent, bg=COLOR_6, bd=0, fg=COLOR_4)

        self.key_man = KeyManager()
        #self.key_man.add_binding('\r', lambda : queue_out.put(self.get()), self.key_man.PRESS)
        #self.key_man.add_binding('\r', lambda : queue_in.put("Outputting: " + self.get()), self.key_man.PRESS)
        self.key_man.add_binding('\r', lambda : self.delete(0, END), self.key_man.PRESS)

        self.__bind_keys()

    def set_grid(self, **kwargs) :
        self.grid(**kwargs)

    def __bind_keys(self) :
        press = lambda event : self.key_man.act_key_press(event.char)
        release = lambda event : self.key_man.act_key_release(event.char)

        self.bind('<Key>', press)
        self.bind('<KeyRelease>', release)

class Gui():
    def __init__(self, queue_in, queue_out) :
        self.root = Tk()
        self.root.geometry(str(WIDTH) + "x" + str(HEIGHT) + "+0+0")

        self.root.attributes("-alpha", .9)
        self.root.config(bg=COLOR_3)

        self.active = True

        self.root.protocol("WM_DELETE_WINDOW", self.destroy)

        self.queue_out = queue_out
        self.queue_in = queue_in

        #for motor referencing, for easier updating of values, everything has an id
        self.readouts = {}

        #for controller referencing, for easier updating of values, akin to motor referencing
        self.controller_readouts = {}
        
        self.controller_readouts[BUTTON] = {}
        self.controller_readouts[AXIS] = {}
        self.controller_readouts[HAT] = {}

        #for slider referencing
        self.slider_of_motor = {}

        self.output_display = Outputter(self.root) #set output box
        self.output_display.set_grid_canvas(row=0,column=1,sticky=N+W+S,rowspan=5)
        self.output_display.set_grid_scroll(row=0,column=0,sticky=N+W+S,rowspan=5)

        self.command_line = CommandLine(self.root, self.queue_out, self.queue_in)
        self.command_line.set_grid(row=4,column=1,sticky=E+S+W,columnspan=1)

        for i in range(10):
            self.root.rowconfigure(i, weight=1)

        for i in range(20):
            self.root.columnconfigure(i + 2, weight=1)

        self.input_readout = Readouts(self.root, "Controller Input")
        self.input_readout.set_grid(row=0,column=3,sticky=E+W+N+S,rowspan=2)

        self.robot_readout_motors = Readouts(self.root, "Driving Motors")
        self.robot_readout_motors.set_grid(row=0,column=4,sticky=E+W+N+S)

        self.robot_readout_swerve = Readouts(self.root, "Swerve Motors")
        self.robot_readout_swerve.set_grid(row=1,column=4,sticky=E+W+N+S)

        self.robot_readout_arm = Readouts(self.root, "Arm Motors")
        self.robot_readout_arm.set_grid(row=0,column=5,sticky=E+W+N+S)

        self.robot_readout_cam = Readouts(self.root, "Camera Motors")
        self.robot_readout_cam.set_grid(row=1,column=5,sticky=E+W+N+S)

        self.sliders = Sliders(self.root, "Sensitivity Sliders")
        self.sliders.set_grid(row=0,column=6,rowspan=2,sticky=E+W+N+S)

        self.active = True

        self.populate()

    def populate(self) :
        #populate motor readouts for wheels
        self.readouts[FRONT_LEFT_WHEEL] = self.robot_readout_motors.add_label("Front Left", "0")
        self.readouts[FRONT_RIGHT_WHEEL] = self.robot_readout_motors.add_label("Front Right", "0")
        self.readouts[BACK_LEFT_WHEEL] = self.robot_readout_motors.add_label("Back Left", "0")
        self.readouts[BACK_RIGHT_WHEEL] = self.robot_readout_motors.add_label("Back Right", "0")

        #populate motor readouts for servos
        self.readouts[FRONT_LEFT_SWERVE] = self.robot_readout_swerve.add_label("Front Left", "0")
        self.readouts[FRONT_RIGHT_SWERVE] = self.robot_readout_swerve.add_label("Front Right", "0")
        self.readouts[BACK_LEFT_SWERVE] = self.robot_readout_swerve.add_label("Back Left", "0")
        self.readouts[BACK_RIGHT_SWERVE] = self.robot_readout_swerve.add_label("Back Right", "0")

        #populate motor readouts for arm
        self.readouts[ARM_0] = self.robot_readout_arm.add_label("Arm 0", "0")
        self.readouts[ARM_1] = self.robot_readout_arm.add_label("Arm 1", "0")
        self.readouts[ARM_2] = self.robot_readout_arm.add_label("Arm 2", "0")
        self.readouts[CLAW] = self.robot_readout_arm.add_label("Claw", "0")

        #populate motor readouts for camera
        self.readouts[CAM_X] = self.robot_readout_cam.add_label("Cam X", "0")
        self.readouts[CAM_Y] = self.robot_readout_cam.add_label("Cam Y", "0")

        #populate sliders
        self.slider_of_motor[LEFT_SENSITIVITY] = self.sliders.add_slider("Left Sensitivity", 0, 100)
        self.slider_of_motor[RIGHT_SENSITIVITY] = self.sliders.add_slider("Right Sensitivity", 0, 100)
        self.slider_of_motor[ARM_0_SENSITIVITY] = self.sliders.add_slider("Arm 0 Sensitivity", 0, 100)
        self.slider_of_motor[ARM_1_SENSITIVITY] = self.sliders.add_slider("Arm 1 Sensitivity", 0, 900)
        self.slider_of_motor[ARM_2_SENSITIVITY] = self.sliders.add_slider("Arm 2 Sensitivity", 0, 600)
        self.slider_of_motor[CLAW_SENSITIVITY] = self.sliders.add_slider("Claw Sensitivity", 0, 600)

        #populate user inputs
        self.controller_readouts[BUTTON][A_BUTTON] = self.input_readout.add_label("A", "0")
        self.controller_readouts[BUTTON][B_BUTTON] = self.input_readout.add_label("B", "0")
        self.controller_readouts[BUTTON][X_BUTTON] = self.input_readout.add_label("X", "0")
        self.controller_readouts[BUTTON][Y_BUTTON] = self.input_readout.add_label("Y", "0")

        self.controller_readouts[AXIS][L_ANALOG_Y] = self.input_readout.add_label("L Analog Y", "0")
        self.controller_readouts[AXIS][L_ANALOG_X] = self.input_readout.add_label("L Analog X", "0")
        self.controller_readouts[AXIS][R_ANALOG_Y] = self.input_readout.add_label("R Analog Y", "0")
        self.controller_readouts[AXIS][R_ANALOG_X] = self.input_readout.add_label("R Analog X", "0")
        self.controller_readouts[AXIS][TRIGGER] = self.input_readout.add_label("Trigger", "0")

        #output line populate test
        for i in range(100):
            self.output_display.add_line("This is " + str(i))

    def gui_loop(self) :
        self.root.mainloop()

    def read_inputs(self) :
        while(self.is_active()) :
            a = self.queue_in.get()
            try:
                Serialize.run_gui(a, self)
            except Exception as ex:
                print(type(ex).__name__, ex.args)

    def update_readout(self, motor, value):
        #self.readouts[motor].update_label(local_names[motor], str(value))]
        self.readouts[motor].set("{0:.2f}".format(value))

    def update_controller(self, type, button, value):
        self.controller_readouts[type][button].set("{0:.2f}".format(value))

    def output(self, string):
        self.output_display.add_line(string)

    def is_active(self) :
        return self.active

    def destroy(self) :
        self.root.destroy()
        self.active = False
