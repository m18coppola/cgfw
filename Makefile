# -*-Makefile-*-

CC = gcc
INCLUDE = -I src/
FLAGS=`sdl2-config --libs --cflags` -W -Wall -g
LIBS= -lsoil2 -lGLEW -lGL -lm
OBJS_NAMES = data_structures.o entity_manager.o component_manager.o
OBJS=$(patsubst %, build/objects/%, $(OBJS_NAMES))

all: main

main: main.o $(OBJS_NAMES)
	$(CC) build/objects/main.o $(OBJS) $(LIBS) $(FLAGS) $(INCLUDE) -o build/app.bin

main.o: src/main.c
	$(CC) src/main.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/main.o

data_structures.o: src/util/data_structures.c
	$(CC) src/util/data_structures.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/data_structures.o

entity_manager.o: src/ecs/entity_manager.c
	$(CC) src/ecs/entity_manager.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/entity_manager.o

component_manager.o: src/ecs/component_manager.c
	$(CC) src/ecs/component_manager.c $(LIBS) $(FLAGS) $(INCLUDE) -c -o build/objects/component_manager.o

run: main
	$(info )
	$(info ___STARTING EXECUTION___)
	$(info ========================)
	$(info )
	./build/app.bin

checkleaks: main
	$(info )
	$(info ___STARTING EXECUTION___)
	$(info ____VALGRIND ENABLED____)
	$(info ========================)
	$(info )
	valgrind ./build/app.bin --leak-check=full --track-origin=yes
	

clean:
	rm build/objects/*.o
	rm build/app.bin
