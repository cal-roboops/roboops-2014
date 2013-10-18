from Tkinter import *
from random import randint
from KeyManager import *

WIDTH = 1600
HEIGHT = 900

class Outputter(Frame):
    _rows = 0

    def __init__(self, root):

        Frame.__init__(self, root)
        self.canvas = Canvas(root, borderwidth=0, highlightthickness=0, background="#000000", width = WIDTH//10, height = 2*HEIGHT//4)
        self.frame = Frame(self.canvas, background="#000000", width = WIDTH//10, height = 2*HEIGHT//4)
        self.scroll = Scrollbar(root, orient="vertical", command=self.canvas.yview)
        self.scroll.config(bg="#000000")
        self.canvas.configure(yscrollcommand=self.scroll.set)

        self.canvas.create_window((1,1), window=self.frame, anchor="sw", tags="self.frame")

        self.frame.bind("<Configure>", self.OnFrameConfigure)


    def set_grid_canvas(self, **kwargs):
        self.canvas.grid(**kwargs)
    
    def set_grid_scroll(self, **kwargs):
        self.scroll.grid(**kwargs)

    def add_line(self, txt):
        Label(self.frame, text = txt, bg="#000000", fg="#ffffff").grid(row=self._rows, column=0, sticky=W)
        self._rows += 1

    def OnFrameConfigure(self, event):
        '''Reset the scroll region to encompass the inner frame'''
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))

class Readouts(LabelFrame):
    def __init__(self, parent, txt = "Undef, yo"):
        self._read_labels_outputs = []
        self._read_labels = {}
        self._read_labels_len = 0
        LabelFrame.__init__(self, parent, text=txt)

    def set_grid(self, **kwargs):
        self.grid(**kwargs)

    def add_label(self, name, readout):
        n_readout = StringVar()
        n_readout.set(readout)
        self._read_labels_outputs.append(n_readout)
        Label(self, text = name).grid(row = self._read_labels_len, column = 0, sticky=W)
        Label(self, textvariable = n_readout).grid(row = self._read_labels_len, column = 1, sticky=E)
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
        LabelFrame.__init__(self, parent, text=txt)

    def set_grid(self, **kwargs):
        self.grid(**kwargs)

    def add_slider(self, name, min, max):
        Label(self, text = name).grid(row = self._num_sliders*2, column = 0, sticky=EW, padx = WIDTH//14)
        slid = Scale(self, from_=min, to=max, orient=HORIZONTAL)
        slid.grid(row = self._num_sliders*2 + 1, column = 0, sticky=EW)
        self._sliders[name] = slid
        self._num_sliders += 1
        return slid

    def get_value(self, name):
        assert self._sliders[name], "Does not exist."
        return self._sliders[name].get()

class CommandLine(Entry):

    def __init__(self, parent, queue_out, queue_in) :
        Entry.__init__(self, parent)

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

        self.active = True

        self.root.protocol("WM_DELETE_WINDOW", self.destroy)

        self.queue_out = queue_out
        self.queue_in = queue_in

        #for motor referencing, for easier updating of values, everything has an id
        self.readouts = []

        #for slider referencing
        self.slider_of_motor = []

        self.output_display = Outputter(self.root) #set output box
        self.output_display.set_grid_canvas(row=0,column=0,sticky=N+W+S,rowspan=4)
        self.output_display.set_grid_scroll(row=0,column=1,sticky=N+W+S,rowspan=4)

        self.command_line = CommandLine(self.root, self.queue_out, self.queue_in)
        self.command_line.set_grid(row=4,column=0,sticky=E+N+W,columnspan=2)

        for i in range(10):
            self.root.rowconfigure(i, weight=1)

        for i in range(20):
            self.root.columnconfigure(i + 2, weight=1)

        self.input_readout = Readouts(self.root, "Controller Input")
        self.input_readout.set_grid(row=1,column=5,sticky=E+W+N+S)

        self.robot_readout_motors = Readouts(self.root, "Driving Motors")
        self.robot_readout_motors.set_grid(row=0,column=3,sticky=E+W+N+S)

        self.robot_readout_swerve = Readouts(self.root, "Swerve Motors")
        self.robot_readout_swerve.set_grid(row=1,column=3,sticky=E+W+N+S)

        self.robot_readout_arm = Readouts(self.root, "Arm Motors")
        self.robot_readout_arm.set_grid(row=0,column=4,sticky=E+W+N+S)

        self.robot_readout_cam = Readouts(self.root, "Camera Motors")
        self.robot_readout_cam.set_grid(row=1,column=4,sticky=E+W+N+S)

        self.sliders = Sliders(self.root, "Sensitivity Sliders")
        self.sliders.set_grid(row=0,column=5)

        self.active = True

        self.populate()

    def populate(self) :

        j = 0 #keep track of absolute motor ids, as opposed to local ones, usless right now; will replace with ports later

        #populate motor readouts for wheels
        for i in range(4):
            self.readouts[j] = self.robot_readout_motors.add_label("Motor " + str(i), "0")
            j += 1

        #populate motor readouts for servos
        for i in range(4):
            self.readouts[j] = self.robot_readout_swerve.add_label("Motor " + str(i), "0")
            j += 1

        #populate motor readouts for arm
        for i in range(3):
            self.readouts[j] = self.robot_readout_arm.add_label("Motor " + str(i), "0")
            j += 1

        #populate motor readouts for camera
        self.readouts[j] = self.robot_readout_cam.add_label("Motor X", "0")
            j += 1
        self.readouts[j] = self.robot_readout_cam.add_label("Motor Y", "0")
            j += 1

        #populate sliders
        self.slider_of_motor[0] = self.sliders.add_slider("Drive Sensitivity Left", 0, 100)
        self.slider_of_motor[1] = self.sliders.add_slider("Drive Sensitivity Right", 0, 100)

        #populate user inputs
        self.input_readout.add_label("A", "0")
        self.input_readout.add_label("B", "0")
        self.input_readout.add_label("X", "0")
        self.input_readout.add_label("Y", "0")

        self.input_readout.add_label("L Analog Y", "0")
        self.input_readout.add_label("L Analog X", "0")
        self.input_readout.add_label("R Analog Y", "0")
        self.input_readout.add_label("R Analog X", "0")

        #output line populate test
        for i in range(100):
            self.output_display.add_line("This is " + str(i))

    def gui_loop(self) :
        self.root.mainloop()

    def read_inputs(self) :
        while(self.is_active()) :
            self.queue_in.get().run_gui(self)

    def update_readout(self, motor, value):
        #self.readouts[motor].update_label(local_names[motor], str(value))
        self.readouts[motor].set(value)

    def output(self, string):
        self.output_display.add_line(string)

    def is_active(self) :
        return self.active

    def destroy(self) :
        self.root.destroy()
        self.active = False
