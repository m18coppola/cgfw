# -*-Makefile-*-

CC = gcc
INCLUDE = -I src/
FLAGS=`sdl2-config --libs --cflags` -W -Wall -g
LIBS= -lsoil2 -lGLEW -lGL -lm
OBJS = datastructures.o

all: main

main: main.o $(OBJS)
	$(CC) build/objects/main.o build/objects/datastructures.o $(LIBS) $(FLAGS) $(INCLUDE) -o build/app.bin

main.o: src/main.c
	$(CC) src/main.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/main.o

datastructures.o: src/util/datastructures.c
	$(CC) src/util/datastructures.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/datastructures.o

run: main
	./build/app.bin

clean:
	rm build/objects/*.o
	rm build/app.bin
