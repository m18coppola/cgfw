#ifndef ENTITY_MANAGER_H_
#define ENTITY_MANAGER_H_

#include <stdio.h>
#include <util/data_structures.h>

#define MAX_ENTITIES 5000

typedef unsigned int EntityID;

struct EntityManager {
	struct ArrayQueue *entity_ID_queue;
	unsigned int component_mask_table[MAX_ENTITIES];
	int entity_count;
};

struct EntityManager *ecs_EntityManager_init(void);
void ecs_EntityManager_free(struct EntityManager** em_p);
EntityID ecs_EntityManager_addEntity(struct EntityManager *em);
void ecs_EntityManager_removeEntity(struct EntityManager *em, EntityID id);
void ecs_EntityManager_setComponentMask(struct EntityManager *em, EntityID id, unsigned int component_mask);
unsigned int ecs_EntityManager_getComponentMask(struct EntityManager *em, EntityID id);

#endif /* ENTITY_MANAGER_H_ */
