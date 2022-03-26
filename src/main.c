#include <stdio.h>
#include <util/data_structures.h>
#include <ecs/entity_manager.h>
#include <ecs/component_manager.h>

int
main (int argc, char *argv[])
{
	struct ComponentManager *cm;

	printf("init component manager...\n");
	cm = ecs_ComponentManager_init();

	printf("creating 2 components (one is int, other is float pair)...\n");
	CID floatpairCID;
	struct FloatPair {
		float x, y;
	};

	CID singleintCID;
	struct SingleInt {
		int num;
	};

	floatpairCID = ecs_ComponentManager_registerComponent(cm, sizeof(struct FloatPair), 10);
	singleintCID = ecs_ComponentManager_registerComponent(cm, sizeof(struct SingleInt), 10);

	unsigned int eid1 = 0;
	unsigned int eid2 = 1;
	unsigned int eid3 = 2;

	printf("giving eid1 and eid2 a float pair...\n");
	ecs_ComponentManager_addComponentInstance(cm, eid1, floatpairCID);
	ecs_ComponentManager_addComponentInstance(cm, eid2, floatpairCID);

	CASTED_DEREF(struct FloatPair, ecs_ComponentManager_getComponentInstance(cm, eid1, floatpairCID)).x = 1.1;
	CASTED_DEREF(struct FloatPair, ecs_ComponentManager_getComponentInstance(cm, eid1, floatpairCID)).y = 1.2;

	CASTED_DEREF(struct FloatPair, ecs_ComponentManager_getComponentInstance(cm, eid2, floatpairCID)).x = 2.1;
	CASTED_DEREF(struct FloatPair, ecs_ComponentManager_getComponentInstance(cm, eid2, floatpairCID)).y = 2.2;

	printf("giving eid2 and eid3 a singleint...\n");
	ecs_ComponentManager_addComponentInstance(cm, eid2, singleintCID);
	ecs_ComponentManager_addComponentInstance(cm, eid3, singleintCID);

	CASTED_DEREF(struct SingleInt, ecs_ComponentManager_getComponentInstance(cm, eid2, singleintCID)).num = 2;
	CASTED_DEREF(struct SingleInt, ecs_ComponentManager_getComponentInstance(cm, eid3, singleintCID)).num = 3;

	printf("detroying all components belonging to eid2...\n");
	ecs_ComponentManager_destroyEntity(cm, eid2);
	
	printf("free component manager...\n");
	ecs_ComponentManager_free(&cm);

	return 0;

}
