import socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind(("127.0.0.1", 9000))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f"connected by {addr}")
        while True:
            # message = input()
            # conn.sendall(message.encode())
            data = conn.recv(1024).decode()
            if data:
                print(data)


