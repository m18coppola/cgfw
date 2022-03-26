#include <ecs/component_manager.h>

struct ComponentArray *
ecs_ComponentArray_init(CID cid, size_t component_size, int capacity)
{
	struct ComponentArray *ca;
	int i;

	ca = malloc(sizeof(struct ComponentArray));
	ca->array = malloc(component_size * capacity);
	ca->entity_index_map = malloc(sizeof(int) * capacity);
	ca->index_entity_map = malloc(sizeof(int) * capacity);
	for (i = 0; i < capacity; i++) {
		ca->entity_index_map[i] = -1;
		ca->index_entity_map[i] = -1;
	}
	ca->component_size = component_size;
	ca->component_count = 0;
	ca->capacity = capacity;
	ca->component_id = cid;

	return ca;
}

void
ecs_ComponentArray_free(struct ComponentArray **ca_p)
{
	free((*ca_p)->array);
	free((*ca_p)->entity_index_map);
	free((*ca_p)->index_entity_map);
	free((*ca_p));
	*ca_p = NULL;
}

void
ecs_ComponentArray_insert(struct ComponentArray *ca, unsigned int eid)
{
	ca->entity_index_map[eid] = ca->component_count;
	ca->index_entity_map[ca->component_count] = eid;
	ca->component_count++;
}

void *
ecs_ComponentArray_get(struct ComponentArray *ca, unsigned int eid)
{
	return ca->array + ca->component_size * ca->entity_index_map[eid];
}

void
ecs_ComponentArray_remove(struct ComponentArray *ca, unsigned int eid)
{
	//if (eid >= ca->capacity || ca->entity_index_map[eid] == -1) return;
	char *origin = (char *)ca->array + ca->component_size * (ca->component_count - 1);
	char *dest = (char *)ca->array + ca->component_size * (ca->entity_index_map[eid]);
	size_t steps = ca->component_size / sizeof(char);
	for (size_t i = 0; i < steps; i += sizeof(char)) {
		*(dest + i) = *(origin + i);
	}

	unsigned int eid_to_move;
	int index_to_move_to;
	eid_to_move = ca->index_entity_map[ca->component_count - 1];
	index_to_move_to = ca->entity_index_map[eid];

	ca->entity_index_map[eid_to_move] = index_to_move_to;
	ca->entity_index_map[ca->component_count - 1] = -1;
	ca->index_entity_map[index_to_move_to] = eid_to_move;
	ca->index_entity_map[ca->component_count - 1] = -1;
	ca->component_count--;
}

struct ComponentManager *
ecs_ComponentManager_init(void)
{
	struct ComponentManager *cm;
	int i;
	cm = malloc(sizeof(struct ComponentManager));
	cm->current_id = 0;
	for (i = 0; i < MAX_COMPONENTS; i++) {
		cm->registered_components[i] = NULL;
	}
	return cm;
}

void
ecs_ComponentManager_free(struct ComponentManager **cm_p)
{
	int i;
	struct ComponentManager *cm;
	cm = *cm_p;
	for (i = 0; i < MAX_COMPONENTS; i++) {
		if (cm->registered_components[i] != NULL) ecs_ComponentArray_free(&(cm->registered_components[i]));
	}
	free(*cm_p);
	cm_p = NULL;
}

CID
ecs_ComponentManager_registerComponent(struct ComponentManager *cm, size_t component_size, int capacity)
{
	CID newCID;
	struct ComponentArray *ca;
	newCID = cm->current_id++;
	ca = ecs_ComponentArray_init(newCID, component_size, capacity);
	cm->registered_components[newCID] = ca;
	return newCID;
}

void
ecs_ComponentManager_addComponentInstance(struct ComponentManager *cm, unsigned int eid, CID cid)
{
	ecs_ComponentArray_insert(cm->registered_components[cid], eid);
}

void
ecs_ComponentManager_removeComponentInstance(struct ComponentManager *cm, unsigned int eid, CID cid)
{
	ecs_ComponentArray_remove(cm->registered_components[cid], eid);
}

void *
ecs_ComponentManager_getComponentInstance(struct ComponentManager *cm, unsigned int eid, CID cid)
{
	return ecs_ComponentArray_get(cm->registered_components[cid], eid);
}

void
ecs_ComponentManager_destroyEntity(struct ComponentManager *cm, unsigned int eid)
{
	struct ComponentArray **columns;
	int i;

	columns = cm->registered_components;
	for (i = 0; i < MAX_COMPONENTS; i++) {
		if (columns[i] != NULL)
			ecs_ComponentArray_remove(columns[i], eid);
	}
}
//TODO: just realized we have to update the component_mask_table when we
// add/remove a component to a particular EID
// maybe just do that in the coordinator. front end for mask handling on entity manager?
