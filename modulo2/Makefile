all: server client

server: server.cpp socket.cpp
	g++ server.cpp socket.cpp -o server -pthread

client: client.cpp socket.cpp
	g++ client.cpp socket.cpp -o client -pthread
