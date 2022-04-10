#include <ecs/ecs.h>

static Signature entity_signatures[MAX_ENTITIES];
static int entity_count;
static unsigned int component_count;
static struct ComponentArray *registered_components[MAX_COMPONENTS];
static EID entity_ID_queue[MAX_ENTITIES];
static int next_eid_index;
static int last_eid_index;
static int available_eid_count;

void
ecs_init(void)
{
	int i;

	/* entity system init */
	entity_count = 0;
	available_eid_count = 0;
	next_eid_index = 0;
	last_eid_index = MAX_ENTITIES - 1;

	for (i = 0; i < MAX_ENTITIES; i++) {
		last_eid_index = (last_eid_index + 1) % MAX_ENTITIES;
		entity_ID_queue[last_eid_index] = i;
		available_eid_count++;
	}

	/* component system init */
	component_count = 0;
	for (i = 0; i < MAX_COMPONENTS; i++) {
		registered_components[i] = NULL;
	}
}

void
ecs_exit(void)
{
	int i;

	/* component system free */
	for (i = 0; i < MAX_COMPONENTS; i++) {
		if (registered_components[i] != NULL)
			ecs_ComponentArray_free(&(registered_components[i]));
	}
}

EID
ecs_makeEntity(void)
{
	EID new_eid = entity_ID_queue[next_eid_index];
	next_eid_index = (next_eid_index + 1) % MAX_ENTITIES;
	available_eid_count--;
	entity_count++;
	return new_eid;
}

void
ecs_removeEntity(EID eid)
{
	unsigned int i;

	/* remove relevant components */
	for (i = 0; i < component_count; i++) {
		ecs_ComponentArray_remove(registered_components[i], eid);
	}

	/* remove from systems' lists */
	//TODO

	/* reset entity and place into ID queue */
	entity_signatures[eid] = 0;

	last_eid_index = (last_eid_index + 1) % MAX_ENTITIES;
	entity_ID_queue[last_eid_index] = eid;
	available_eid_count++;

	entity_count--;
}

CID
ecs_registerComponent(size_t component_size)
{
	CID new_cid;

	new_cid = component_count++;
	registered_components[new_cid] = ecs_ComponentArray_init(component_size);

	return new_cid;
}

void *
ecs_addComponentInstance(EID eid, CID cid)
{
	/* allocate memory for component */
	ecs_ComponentArray_insert(registered_components[cid], eid);

	/* update signature for entity */
	entity_signatures[eid] |= (1 << cid);

	/* update systems' entity lists */
	//TODO
	
	return ecs_ComponentArray_get(registered_components[cid], eid);
}

void
ecs_removeComponentInstance(EID eid, CID cid)
{
	/* remove component from memory */
	ecs_ComponentArray_remove(registered_components[cid], eid);

	/* update signature for entity */
	entity_signatures[eid] &= ~(1 << cid);

	/* remove EID from systems' lists */
	//TODO
}

void *
ecs_getComponentInstance(EID eid, CID cid)
{
	return ecs_ComponentArray_get(registered_components[cid], eid);
}


struct ComponentArray *
ecs_ComponentArray_init(size_t component_size)
{
	struct ComponentArray *ca;
	int i;

	ca = malloc(sizeof(struct ComponentArray));
	ca->array = malloc(component_size * MAX_ENTITIES);
	ca->entity_index_map = malloc(sizeof(int) * MAX_ENTITIES);
	ca->index_entity_map = malloc(sizeof(int) * MAX_ENTITIES);
	for (i = 0; i < MAX_ENTITIES; i++) {
		ca->entity_index_map[i] = -1;
		ca->index_entity_map[i] = -1;
	}
	ca->component_size = component_size;
	ca->component_count = 0;

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
ecs_ComponentArray_insert(struct ComponentArray *ca, EID eid)
{
	ca->entity_index_map[eid] = ca->component_count;
	ca->index_entity_map[ca->component_count] = eid;
	ca->component_count++;
}

void *
ecs_ComponentArray_get(struct ComponentArray *ca, EID eid)
{
	return ca->array + ca->component_size * ca->entity_index_map[eid];
}

void
ecs_ComponentArray_remove(struct ComponentArray *ca, EID eid)
{
	if (ca->entity_index_map[eid] == -1) return;
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

