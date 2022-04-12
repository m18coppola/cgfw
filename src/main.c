#include <stdio.h>
#include <ecs/ecs.h>

CID position;

struct IntPair {
	int x;
	int y;
};

void
doublePosImplementation(EID eid) {
	struct IntPair *currIP;
	printf("\tlocal sys: iter on eid:%u\n", eid);
	currIP = (struct IntPair *)ecs_getComponentInstance(eid, position);
	currIP->x *= 2;
	currIP->y *= 2;
}

int
main (int argc, char *argv[])
{
	printf("starting ECS...\n");
	ecs_init();
	printf("\tsuccess!\n");

	printf("registering int pair component...\n");
	position = ecs_registerComponent(sizeof(struct IntPair));
	printf("\tposition CID: %u\n", position);




	printf("making 3 entities...\n");
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

	printf("registering system...\n");
	SID doubleSys = ecs_registerSystem(&position, 1, doublePosImplementation);

	printf("calling system...\n");
	ecs_callSystem(doubleSys);


	printf("print content...\n");
	for(int i = 0; i < 3; i++) {
		struct IntPair *currIP;
		currIP = (struct IntPair *)ecs_getComponentInstance(my_entities[i], position);
		printf("ID: %u: %d, %d\n", my_entities[i], currIP->x, currIP->y);
	}


	printf("remove eid:1 component...\n");
	ecs_removeComponentInstance(my_entities[1], position);

	printf("print content...\n");
	for(int i = 0; i < 3; i++) {
			struct IntPair *currIP;
			currIP = (struct IntPair *)ecs_getComponentInstance(my_entities[i], position);
			if(currIP != NULL)
			printf("ID: %u: %d, %d\n", my_entities[i], currIP->x, currIP->y);
	}

	printf("calling system...\n");
	ecs_callSystem(doubleSys);

	printf("print content...\n");
	for(int i = 0; i < 3; i++) {
		if(i != 1) {
			struct IntPair *currIP;
			currIP = (struct IntPair *)ecs_getComponentInstance(my_entities[i], position);
			if(currIP != NULL)
			printf("ID: %u: %d, %d\n", my_entities[i], currIP->x, currIP->y);
		}
	}

	printf("exiting ECS...\n");
	ecs_exit();

	return 0;
}

