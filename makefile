all: build link

build:
	gcc -c server.c

link:
	gcc server.o -o server
	
	
	