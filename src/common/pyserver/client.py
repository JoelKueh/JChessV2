import socket

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect(("127.0.0.1", 9000))
    while True:
        data = s.recv(1024)
        message = int(data.decode())
        if message == -1:
            break
        message = str(message + 1).encode()
        s.sendall(message)
        

        


