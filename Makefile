# -*-Makefile-*-

CC = gcc
INCLUDE = -I src/
FLAGS=`sdl2-config --libs --cflags` -W -Wall -g
LIBS= -lsoil2 -lGLEW -lGL -lm
OBJS_NAMES = data_structures.o ecs.o
OBJS=$(patsubst %, build/objects/%, $(OBJS_NAMES))

all: main

main: main.o $(OBJS_NAMES)
	$(CC) build/objects/main.o $(OBJS) $(LIBS) $(FLAGS) $(INCLUDE) -o build/app.bin

main.o: src/main.c
	$(CC) src/main.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/main.o

data_structures.o: src/util/data_structures.c
	$(CC) src/util/data_structures.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/data_structures.o

ecs.o: src/ecs.c
	$(CC) src/ecs.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/ecs.o

run: main
	$(info )
	$(info ___STARTING EXECUTION___)
	$(info ========================)
	$(info )
	./build/app.bin

checkmem: main
	$(info )
	$(info ___STARTING EXECUTION___)
	$(info ____CHECKMEM ENABLED____)
	$(info ========================)
	$(info )
	valgrind --leak-check=full --track-origins=yes ./build/app.bin

checkcache: main
	$(info )
	$(info ___STARTING EXECUTION___)
	$(info ___CHECKCACHE ENABLED___)
	$(info ========================)
	$(info )
	valgrind --tool=cachegrind ./build/app.bin
	

clean:
	rm build/objects/*.o
	rm build/app.bin
