#include <stdio.h>
#include <ecs/ecs.h>

int
main (int argc, char *argv[])
{
	printf("starting ECS...\n");
	ecs_init();
	printf("\tsuccess!\n");

	printf("registering int pair component...\n");
	struct IntPair {
		int x;
		int y;
	};
	CID position = ecs_registerComponent(sizeof(struct IntPair));
	printf("\tposition CID: %u\n", position);




	printf("making 3 entites...\n");
	EID my_entities[3];
	for (int i = 0; i < 3; i++) {
		my_entities[i] = ecs_makeEntity();
		printf("\tregistered id: %u\n", my_entities[i]);
	}


	printf("giving each entity a int pair...\n");
	for(int i = 0; i < 3; i++) {
		struct IntPair *currIP;
		currIP = (struct IntPair *)ecs_addComponentInstance(my_entities[i], position);
		currIP->x = i + 1;
		currIP->y = (i+1) * 2;
	}

	printf("print content...\n");
	for(int i = 0; i < 3; i++) {
		struct IntPair *currIP;
		currIP = (struct IntPair *)ecs_getComponentInstance(my_entities[i], position);
		printf("ID: %u: %d, %d\n", my_entities[i], currIP->x, currIP->y);
	}


	ecs_removeComponentInstance(my_entities[0], position);


	printf("freeing entites...\n");
	for (int i = 0; i < 3; i++) {
		ecs_removeEntity(my_entities[i]);
	}

	printf("exiting ECS...\n");
	ecs_exit();

	return 0;
}
