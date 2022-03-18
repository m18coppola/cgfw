#ifndef COMPONENT_MANAGER_H_
#define COMPONENT_MANAGER_H_

#include <stdio.h>
#include <stdlib.h>

#define MAX_COMPONENTS 32

typedef unsigned int CID;

struct ComponentManager {
	int current_id;

};

struct ComponentArray
{
	void *array;
	int *entity_index_map;
	int *index_entity_map;
	size_t component_size;
	int component_count;
	int capacity;
	CID component_id;
};

struct ComponentArray *ecs_ComponentArray_init(CID cid, size_t component_size, int capacity);
void ecs_ComponentArray_free(struct ComponentArray **ca_p);
void ecs_ComponentArray_insert(struct ComponentArray *ca, unsigned int eid);
void *ecs_ComponentArray_get(struct ComponentArray *ca, unsigned int eid);
void ecs_ComponentArray_remove(struct ComponentArray *ca, unsigned int eid);

#endif /* COMPONENT_MANAGER_H_ */
