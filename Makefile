all: server

server: number-server.c http-server.c
	gcc -std=c11 -Wall -Wno-unused-variable -fsanitize=address -g number-server.c http-server.c -o server

run: server
	./server 42042

clean:
	rm -f server
