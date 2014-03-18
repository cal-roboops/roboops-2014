import socket, sys
from threading import Thread
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

HOST = 'localhost'
PORT = 1061

s.connect((HOST,PORT))
print('Client has been assigned socket name'+repr(s.getsockname()))
def recieve():
    while True:
        data = s.recv(1024)
        if not data: sys.exit(0)
        print('\nThe server said '+ data)
def send():
	while True:
	    sendString = input()
	    s.sendall(sendString)
Thread(target=recieve).start()
Thread(target=send).start()
