import socket
from threading import Thread

def defaultOut(message):
	print(message)

def defaultIn():
	message = input()
	return message

def defaultError():
    print("Socket disconnected!")

class Endpoint():
    def __init__(self, fnSend, fnReceive, fnError) :
        self.fnSend = fnSend
        self.fnReceive = fnReceive
        self.fnError = fnError
        self.isOn = True
        self.stored = ""

    def send(self, function):
        while self.isOn:
            sendstring = function() + "\0"
            #print(sendstring)
            if type(sendstring) == type("hi") :
                sendstring = sendstring.encode(encoding='UTF-8')
            #print("sent!\n")
            self.sc.sendall(sendstring)
    def receive(self, function):
        while self.isOn:
            try:
                str_recvd = self.sc.recv(4028).decode(encoding='UTF-8')
                self.stored += str_recvd
            except Exception as e:
                self.isOn = False
                fnError()

            if(str_recvd == ""):
                self.isOn = False
                fnError()

            while(self.stored.find("\0") > -1):
                null_ptr = self.stored.find("\0")
                temp_msg = self.stored[:null_ptr]
                self.stored = self.stored[null_ptr + 1:]
                function(temp_msg)

    def start(self):
        Thread(None, self.send, None, (self.fnSend,)).start()
        Thread(None, self.receive, None, (self.fnReceive,)).start()

class Server(Endpoint):
    def __init__(self, host='0.0.0.0', port=800, fnSend=defaultIn, fnReceive=defaultOut, fnError=defaultError):
        Endpoint.__init__(self, fnSend, fnReceive, fnError)

        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.s.bind((host, port))
        self.s.listen(5)
        
        print('Listening at '+repr(self.s.getsockname()))
        self.sc, self.sockname = self.s.accept()
        print('We have accepted a connection from'+repr(self.sockname))
        print('Socket connects'+repr(self.sc.getsockname())+'and '+repr(self.sc.getpeername()))

class Client(Endpoint):
    def __init__(self, host='localhost', port=800, fnSend=defaultIn, fnReceive=defaultOut, fnError=defaultError):
        Endpoint.__init__(self, fnSend, fnReceive, fnError)

        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.s.connect((host, port))
        self.sc = self.s
        print('Client has been assigned socket name'+repr(self.s.getsockname()))
