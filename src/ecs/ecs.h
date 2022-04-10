#ifndef ECS_H_
#define ECS_H_

#include <stdio.h>
#include <util/data_structures.h>

#define MAX_ENTITIES 10
#define MAX_COMPONENTS 32

typedef unsigned int CID;
typedef unsigned int EID;
typedef unsigned int SID;
typedef unsigned int Signature;

struct ComponentArray
{
	void *array;
	int *entity_index_map;
	int *index_entity_map;
	size_t component_size;
	unsigned int component_count;
};

void ecs_init(void);
void ecs_exit(void);
EID ecs_makeEntity(void);
void ecs_removeEntity(EID eid);
CID ecs_registerComponent(size_t component_size);
void *ecs_addComponentInstance(EID eid, CID cid);
void ecs_removeComponentInstance(EID eid, CID cid);
void *ecs_getComponentInstance(EID eid, CID cid);

/* private */
struct ComponentArray *ecs_ComponentArray_init(size_t component_size);
void ecs_ComponentArray_free(struct ComponentArray **ca_p);
void ecs_ComponentArray_insert(struct ComponentArray *ca, EID eid);
void *ecs_ComponentArray_get(struct ComponentArray *ca, EID eid);
void ecs_ComponentArray_remove(struct ComponentArray *ca, EID eid);

#endif /* ECS_H_ */
