import socket
from threading import Thread

def defaultOut(message):
	print(message)

def defaultIn():
	message = input()
	return message

class Endpoint:
    def send(function):
        while True:
            sendString = function()
            sc.sendall(sendString)
    def recieve(function):
        while True:
            message = sc.recv(1024)
            function(message)

class Server(Endpoint):
    def __init__(self, host='0.0.0.0', port=1061, fnSend=defaultIn, fnRecieve=defaultOut):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.s.bind((host, port))
        self.s.listen(5)
        
        print('Listening at '+repr(self.s.getsockname()))
        self.sc, self.sockname = self.s.accept()
        print('We have accepted a connection from'+repr(self.sockname))
        print('Socket connects'+repr(self.sc.getsockname())+'and '+repr(self.sc.getpeername()))

        Thread(target=self.send, function=fnSend).start()
        Thread(target=self.recieve, function=fnRecieve).start()

class Client(Endpoint):
    def __init__(self, host='localhost', port=1061, fnSend=defaultIn, fnRecieve=defaultOut):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.s.connect((host, port))
        print('Client has been assigned socket name'+repr(self.s.getsockname()))

        Thread(target=self.send, function=fnSend).start()
        Thread(target=self.recieve, function=fnRecieve).start()
