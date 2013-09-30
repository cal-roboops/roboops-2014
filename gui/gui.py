from tkinter import *
from random import randint
from keyManager import *

WIDTH = 1600
HEIGHT = 900

class outputter(Frame):
    _rows = 0

    def __init__(self, root):

        Frame.__init__(self, root)
        self.canvas = Canvas(root, borderwidth=0, highlightthickness=0, background="#000000", width = 8*WIDTH//10, height = HEIGHT//4)
        self.frame = Frame(self.canvas, background="#000000", width = 8*WIDTH//10, height = HEIGHT//4)
        self.scroll = Scrollbar(root, orient="vertical", command=self.canvas.yview)
        self.scroll.config(bg="#000000")
        self.canvas.configure(yscrollcommand=self.scroll.set)

        self.scroll.grid(row=11, column=1, sticky=NS)
        self.canvas.grid(row=11, column=0)
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

class readouts(LabelFrame):

    _read_labels_outputs = []
    _read_labels = {}
    _read_labels_len = 0

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

    def update_label(self, name, new_readout):
        assert name in self._read_labels, name + " does not exist."
        i = self._read_labels[name]
        self._read_labels_outputs[i].set(new_readout)

class sliders(LabelFrame):
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

    def get_value(self, name):
        assert self._sliders[name], "Does not exist."
        return self._sliders[name].get()

class commandLine(Entry):

    def __init__(self, parent, queue_out, queue_in) :
        Entry.__init__(self, parent)

        self.key_man = keyManager()
        self.key_man.add_binding('\r', lambda : queue_out.put(self.get()), self.key_man.PRESS)
        self.key_man.add_binding('\r', lambda : queue_in.put("Outputting: " + self.get()), self.key_man.PRESS)
        self.key_man.add_binding('\r', lambda : self.delete(0, END), self.key_man.PRESS)

        self.__bind_keys()

    def set_grid(self, **kwargs) :
        self.grid(**kwargs)

    def __bind_keys(self) :
        press = lambda event : self.key_man.act_key_press(event.char)
        release = lambda event : self.key_man.act_key_release(event.char)

        self.bind('<Key>', press)
        self.bind('<KeyRelease>', release)

class gui():
    def __init__(self, queue_in, queue_out) :
        self.root = Tk()
        self.root.geometry(str(WIDTH) + "x" + str(HEIGHT) + "+0+0")

        self.active = True

        self.root.protocol("WM_DELETE_WINDOW", self.destroy)

        self.queue_out = queue_out
        self.queue_in = queue_in

        self.video = Label(self.root, text = "Picture")
        self.video.grid(row=0, column=0, rowspan = 10, columnspan=15,padx=(2*WIDTH//5,2*WIDTH//5), pady=(3*HEIGHT//10, 3*HEIGHT//10))

        self.output_display = outputter(self.root)
        self.output_display.set_grid_scroll(row=11,column=1,sticky=NS)
        self.output_display.set_grid_canvas(row=11,column=0)

        self.input_readout = readouts(self.root, "Inputs of a Varied Nature")
        self.input_readout.set_grid(row=11,column=16)

        self.robot_readout = readouts(self.root, "Motors and Various Readouts of Importance")
        self.robot_readout.set_grid(row=0,column=16)

        self.sliders = sliders(self.root, "Sliders of Utmost Functionality")
        self.sliders.set_grid(row=2,column=16)

        self.command_line = commandLine(self.root, self.queue_out, self.queue_in)
        self.command_line.set_grid(row=12,column=0,sticky=EW)

        self.active = True

        self.populate()

    def populate(self) :
        for i in range(0, 3):
            self.sliders.add_slider("Slider" + str(i), -100, 100)
        for i in range(0, 10):
            self.input_readout.add_label("???" + str(i), "100")
        for i in range(0, 13):
            self.robot_readout.add_label("Motor " + str(i), "100")
        for i in range(100):
            self.output_display.add_line("This is " + str(i))

    def gui_loop(self) :
        self.root.mainloop()

    def read_inputs(self) :
        while(self.is_active()) :
            string = self.queue_in.get()
            if not "Outputting: " in string :
                self.parse_commands(string)
            self.output_display.add_line(string)

    def parse_commands(self, string) :
        i, action, value, length = 0, "", "", len(string)
        while i < length :
            if string[i] == ':' :
                i += 1
                while i < length and string[i] != ';':
                    value += string[i]
                    i += 1
                self.update_readout(action, value)
                action, value = "", ""
            else :
                action = string[i]
            i += 1

    def update_readout(self, action, value):
        self.robot_readout.update_label("Motor " + str(action), str(value))

    def is_active(self) :
        return self.active

    def destroy(self) :
        self.root.destroy()
        self.active = False
