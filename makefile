all: client server

client: client.c
	gcc -o client client.c

server: server.c
	gcc -g -o server server.c