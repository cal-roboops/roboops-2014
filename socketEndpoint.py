import socket
from threading import Thread

def defaultOut(message):
	print(message)

def defaultIn():
	message = input()
	return message

class Endpoint():
    def __init__(self) :
        pass

    def send(self, function):
        while True:
            sendstring = function()
            if type(sendstring) == type("hi") :
                sendstring = function().encode(encoding='UTF-8')
            self.s.sendall(sendString)
    def receive(self, function):
        while True:
            message = self.s.recv(1024).decode(encoding='UTF-8')
            function(message)

    def start(self):
        Thread(None, self.send, None, (fnSend,)).start()
        Thread(None, self.receive, None, (fnReceive,)).start()

class Server(Endpoint):
    def __init__(self, host='0.0.0.0', port=800, fnSend=defaultIn, fnReceive=defaultOut):
        Endpoint.__init__(self)

        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.s.bind((host, port))
        self.s.listen(5)
        
        print('Listening at '+repr(self.s.getsockname()))
        self.sc, self.sockname = self.s.accept()
        print('We have accepted a connection from'+repr(self.sockname))
        print('Socket connects'+repr(self.sc.getsockname())+'and '+repr(self.sc.getpeername()))

class Client(Endpoint):
    def __init__(self, host='localhost', port=800, fnSend=defaultIn, fnReceive=defaultOut):
        Endpoint.__init__(self)

        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.s.connect((host, port))
        print('Client has been assigned socket name'+repr(self.s.getsockname()))
