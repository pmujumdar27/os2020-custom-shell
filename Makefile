all: build

build:
	gcc shell.c globals.c pwd.c ls.c mkdir.c cat.c grep.c -o shell -Wall -Werror  

clean:
	rm shell