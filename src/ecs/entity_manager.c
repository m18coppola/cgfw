#include <ecs/entity_manager.h>

struct EntityManager *
ecs_EntityManager_init(void)
{
	struct EntityManager *em = NULL;
	em = malloc(sizeof(struct EntityManager));
	em->entity_ID_queue = ds_ArrayQueue_init(MAX_ENTITIES, sizeof(EntityID));
	em->entity_count = 0;

	for (int i = 0; i < MAX_ENTITIES; i++) {
		ds_ArrayQueue_enqueue(em->entity_ID_queue);
		*((unsigned int *)ds_ArrayQueue_peekTail(em->entity_ID_queue)) = i;
	}

	return em;
}

void
ecs_EntityManager_free(struct EntityManager **em_p)
{
	struct EntityManager *em = *em_p;
	ds_ArrayQueue_free(&em->entity_ID_queue);
	free(em);
	*em_p = NULL;
}

EntityID
ecs_EntityManager_addEntity(struct EntityManager *em)
{
	EntityID newID = *((EntityID *)ds_ArrayQueue_dequeue(em->entity_ID_queue));
	em->entity_count++;
	return newID;
}

void
ecs_EntityManager_removeEntity(struct EntityManager *em, EntityID id)
{
	em->component_mask_table[id] = 0;
	ds_ArrayQueue_enqueue(em->entity_ID_queue);
	*((EntityID *)ds_ArrayQueue_peekTail(em->entity_ID_queue)) = id;
	em->entity_count--;
}

void
ecs_EntityManager_setComponentMask(struct EntityManager *em, EntityID id, unsigned int component_mask)
{
	em->component_mask_table[id] = component_mask;
}


unsigned int
ecs_EntityManager_getComponentMask(struct EntityManager *em, EntityID id)
{
	return em->component_mask_table[id];
}
