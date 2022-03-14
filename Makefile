# -*-Makefile-*-

CC = gcc
INCLUDE = -I src/
FLAGS=`sdl2-config --libs --cflags` -W -Wall -g
LIBS= -lsoil2 -lGLEW -lGL -lm
OBJS_NAMES = datastructures.o entity_manager.o
OBJS=$(patsubst %, build/objects/%, $(OBJS_NAMES))

all: main

main: main.o $(OBJS_NAMES)
	$(CC) build/objects/main.o $(OBJS) $(LIBS) $(FLAGS) $(INCLUDE) -o build/app.bin

main.o: src/main.c
	$(CC) src/main.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/main.o

datastructures.o: src/util/datastructures.c
	$(CC) src/util/datastructures.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/datastructures.o

entity_manager.o: src/ecs/entity_manager.c
	$(CC) src/ecs/entity_manager.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/entity_manager.o

run: main
	./build/app.bin

clean:
	rm build/objects/*.o
	rm build/app.bin
