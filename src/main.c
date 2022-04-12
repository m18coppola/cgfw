#include <stdio.h>
#include <ecs/ecs.h>

struct IntPair {
	int x;
	int y;
};

struct LeftInt {
	int val;
};

struct RightInt {
	int val;
};

void
printSumImplementation(EID eid, CID *cids) {
	struct LeftInt *li;
	struct RightInt *ri;

	li = GET_COMPONENT(struct LeftInt, eid, cids[0]);
	ri = GET_COMPONENT(struct RightInt, eid, cids[1]);

	printf("EID%u:\t%d + %d = %d\n", eid, li->val, ri->val, li->val + ri->val);
}

void
doublePosImplementation(EID eid, CID *cids) {
	struct IntPair *currIP;

	currIP = (struct IntPair *)ecs_getComponentInstance(eid, cids[0]);

	currIP->x *= 2;
	currIP->y *= 2;
}


void
printIPImplementation(EID eid, CID *cids) {
	struct IntPair *currIP;

	currIP = (struct IntPair *)ecs_getComponentInstance(eid, cids[0]);

	printf("ID: %u: %d, %d\n", eid, currIP->x, currIP->y);
}

int
main (int argc, char *argv[])
{
	printf("starting ECS...\n");
	ecs_init();
	printf("\tsuccess!\n");

	printf("registering int pair component...\n");
	CID position = ecs_registerComponent(sizeof(struct IntPair));
	CID left = ecs_registerComponent(sizeof(struct LeftInt));
	CID right = ecs_registerComponent(sizeof(struct RightInt));
	printf("\tposition CID: %u\n", position);




	printf("making 100 entities...\n");
	EID my_entities[100];
	for (int i = 0; i < 100; i++) {
		my_entities[i] = ecs_makeEntity();
		printf("\tregistered id: %u\n", my_entities[i]);
	}


	printf("giving each entity a int pair...\n");
	for(int i = 0; i < 100; i++) {
		struct IntPair *currIP;
		currIP = (struct IntPair *)ecs_addComponentInstance(my_entities[i], position);
		currIP->x = i + 1;
		currIP->y = (i+1) * 2;
		struct LeftInt *l = ADD_COMPONENT(struct LeftInt, my_entities[i], left);
		struct RightInt *r = ADD_COMPONENT(struct RightInt, my_entities[i], right);
		r->val = i*2 +100;
		l->val = i*3 +100;
	}

	printf("print content...\n");
	for(int i = 0; i < 100; i++) {
		struct IntPair *currIP;
		currIP = (struct IntPair *)ecs_getComponentInstance(my_entities[i], position);
		printf("ID: %u: %d, %d\n", my_entities[i], currIP->x, currIP->y);
	}

	printf("registering systems...\n");
	SID doubleSys = ecs_registerSystem(&position, 1, doublePosImplementation);
	SID printIP = ecs_registerSystem(&position, 1, printIPImplementation);
	CID q[2] = {
		left,
		right
	};
	SID sum = ecs_registerSystem(q, 2, printSumImplementation);

	printf("calling system...\n");
	//ecs_callSystem(doubleSys);
	//ecs_callSystem(printIP);
	ecs_callSystem(sum);


	//printf("print content...\n");
	//ecs_callSystem(printIP);


	printf("remove even EIDS...\n");
	for(int i = 0; i < 100; i += 2) {
		ecs_removeEntity(my_entities[i]);
	}
	ecs_removeComponentInstance(my_entities[1], position);

	printf("print content...\n");
	ecs_callSystem(printIP);

	printf("calling system...\n");
	ecs_callSystem(doubleSys);

	printf("print content...\n");
	ecs_callSystem(printIP);

	printf("exiting ECS...\n");
	ecs_exit();

	return 0;
}

