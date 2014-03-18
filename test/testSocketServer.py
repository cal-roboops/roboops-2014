import socket
from threading import Thread

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

HOST = '0.0.0.0'
PORT = 1061

s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(5)

print('Listening at '+repr(s.getsockname()))
sc, sockname = s.accept()
print('We have accepted a connection from'+repr(sockname))
print('Socket connects'+repr(sc.getsockname())+'and '+repr(sc.getpeername()))
def send():
    while True:
        sendString = input()
        sc.sendall(sendString)
def recieve():
    while True:
        message = sc.recv(1024)
        print('The client said '+repr(message))
Thread(target=send).start()
Thread(target=recieve).start() 
