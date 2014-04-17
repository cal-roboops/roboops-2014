import socket
import select
from threading import Thread
from datetime import datetime, timedelta
from time import sleep

def defaultOut(message):
	print(message)

def defaultIn():
	message = input()
	return message

def defaultError():
    print("Socket disconnected!")

def defaultShouldEnd():
    return True

class Endpoint():
    heartbeat_interval = 1
    heartbeat_msg = "HEARTBEAT\0"
    last_update = None
    wait_time = 10

    def __init__(self, fnSend, fnReceive, fnError) :
        self.fnSend = fnSend
        self.fnReceive = fnReceive
        self.fnError = fnError
        self.isOn = True
        self.stored = ""

    def update_time(self):
        self.last_update = datetime.today()

    def heartbeat_routine(self):
        while self.isOn:
            try:
                sendstring = self.heartbeat_msg.encode(encoding='UTF-8')
                success = self.sc.sendall(sendstring)
            except:
                pass
            if self.connection_expired():
                print "EXPIRED!"
                self.close()
            sleep(self.heartbeat_interval)


    def connection_expired(self):
        if self.last_update != None:
            return (datetime.today() - self.last_update).total_seconds() > self.wait_time
        else:
            return False

    def send(self, function):
        while self.isOn:
            try:
                sendstring = function() + "\0"
                if type(sendstring) == type("hi") :
                    sendstring = sendstring.encode(encoding='UTF-8')
                success = self.sc.sendall(sendstring)

            except:
                pass
        print("Sending socket closed!")

    def receive(self, function):
        while self.isOn:
            str_recvd = ""
            try:
                str_recvd = self.sc.recv(4028).decode(encoding='UTF-8')
                if str_recvd == self.heartbeat_msg:
                    self.update_time()
                else:
                    self.stored += str_recvd
            except Exception as e:
                self.close()


            if(str_recvd == ""):
                self.close()


            while(self.stored.find("\0") > -1):
                null_ptr = self.stored.find("\0")
                temp_msg = self.stored[:null_ptr]
                self.stored = self.stored[null_ptr + 1:]
                function(temp_msg)
        print("Receiving socket closed!")

    def close(self):
        self.isOn = False
        self.sc.shutdown(2)
        self.fnError()

    def start(self):
        Thread(None, self.send, None, (self.fnSend,)).start()
        Thread(None, self.receive, None, (self.fnReceive,)).start()
        Thread(None, self.heartbeat_routine, None, ()).start()

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
        print('Socket connects '+repr(self.sc.getsockname())+' and '+repr(self.sc.getpeername()))

class Client(Endpoint):
    def __init__(self, host='localhost', port=800, fnSend=defaultIn, fnReceive=defaultOut, fnError=defaultError):
        Endpoint.__init__(self, fnSend, fnReceive, fnError)

        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.s.connect((host, port))
        self.sc = self.s
        print('Client has been assigned socket name'+repr(self.s.getsockname()))
