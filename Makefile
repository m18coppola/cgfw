# -*-Makefile-*-

CC=gcc
FLAGS=`sdl2-config --libs --cflags` -W -Wall -g
LIBS= -lsoil2 -lGLEW -lGL -lm

all: main

main: main.o
	$(CC) main.o $(LIBS) $(FLAGS) -o app.bin

main.o: main.c
	$(CC) main.c $(LIBS) $(FLAGS) -c -o main.o

clean:
	rm -f *.o *.bin
