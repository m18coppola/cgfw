#ifndef COMPONENT_MANAGER_H_
#define COMPONENT_MANAGER_H_

#include <stdio.h>
#include <stdlib.h>

#define MAX_COMPONENTS 32

typedef unsigned int CID;

struct ComponentArray
{
	void *array;
	int *entity_index_map;
	int *index_entity_map;
	size_t component_size;
	unsigned int component_count;
	unsigned int capacity;
	CID component_id;
};

struct ComponentManager {
	unsigned int current_id;
	struct ComponentArray *registered_components[MAX_COMPONENTS];
};

struct ComponentArray *ecs_ComponentArray_init(CID cid, size_t component_size, int capacity);
void ecs_ComponentArray_free(struct ComponentArray **ca_p);
void ecs_ComponentArray_insert(struct ComponentArray *ca, unsigned int eid);
void *ecs_ComponentArray_get(struct ComponentArray *ca, unsigned int eid);
void ecs_ComponentArray_remove(struct ComponentArray *ca, unsigned int eid);


struct ComponentManager *ecs_ComponentManager_init(void);
void ecs_ComponentManager_free(struct ComponentManager **cm_p);
CID ecs_ComponentManager_registerComponent(struct ComponentManager *cm, size_t component_size, int capacity);
void ecs_ComponentManager_addComponentInstance(struct ComponentManager *cm, unsigned int eid, CID cid);
void ecs_ComponentManager_removeComponentInstance(struct ComponentManager *cm, unsigned int eid, CID cid);
void *ecs_ComponentManager_getComponentInstance(struct ComponentManager *cm, unsigned int eid, CID cid);
void ecs_ComponentManager_destroyEntity(struct ComponentManager *cm, unsigned int eid);

#endif /* COMPONENT_MANAGER_H_ */
