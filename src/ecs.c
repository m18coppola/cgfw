#include <ecs/ecs.h>

static Signature entity_signatures[MAX_ENTITIES];
static int entity_count;
static unsigned int component_count;
static struct PackedArray *registered_components[MAX_COMPONENTS];
static EID entity_ID_queue[MAX_ENTITIES];
static int next_eid_index;
static int last_eid_index;
static int available_eid_count;
static struct System *registered_systems[MAX_SYSTEMS];
static unsigned int system_count;
static struct PackedArray *signature_ledger;

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
	signature_ledger = ds_PackedArray_init(MAX_ENTITIES, sizeof(Signature));

	/* component system init */
	component_count = 0;
	for (i = 0; i < MAX_COMPONENTS; i++) {
		registered_components[i] = NULL;
	}
}

void
ecs_exit(void)
{
	unsigned int i;
	struct System *s;

	/* entity system free */
	ds_PackedArray_free(&signature_ledger);

	/* component system free */
	for (i = 0; i < MAX_COMPONENTS; i++) {
		if (registered_components[i] != NULL)
			ds_PackedArray_free(&(registered_components[i]));
	}

	/* free systems */
	for (i = 0; i < system_count; i++) {
		s = registered_systems[i];
		switch (s->hint) {
		case LEDGER:
			free(s->query_results);
			break;
		case SINGLE_COLUMN:
		default:
			break;
		}
		free(s);
	}
}

EID
ecs_makeEntity(void)
{
	EID new_eid = entity_ID_queue[next_eid_index];
	next_eid_index = (next_eid_index + 1) % MAX_ENTITIES;
	available_eid_count--;
	entity_count++;
	*(SID *)ds_PackedArray_addKey(signature_ledger, new_eid) = 0;
	return new_eid;
}

void
ecs_removeEntity(EID eid)
{
	unsigned int i;

	/* remove relevant components */
	for (i = 0; i < component_count; i++) {
		ds_PackedArray_removeKey(registered_components[i], eid);
	}

	/* remove from systems' lists */
	ecs_reindexSystems(entity_signatures[eid]);

	/* reset entity and place into ID queue */
	entity_signatures[eid] = 0;

	last_eid_index = (last_eid_index + 1) % MAX_ENTITIES;
	entity_ID_queue[last_eid_index] = eid;
	available_eid_count++;

	ds_PackedArray_removeKey(signature_ledger, eid);

	entity_count--;
}

CID
ecs_registerComponent(size_t component_size)
{
	CID new_cid;

	new_cid = component_count++;
	registered_components[new_cid] = ds_PackedArray_init(MAX_ENTITIES, component_size);

	return new_cid;
}

void *
ecs_addComponentInstance(EID eid, CID cid)
{
	/* allocate memory for component */
	ds_PackedArray_addKey(registered_components[cid], eid);

	/* update signature for entity */
	entity_signatures[eid] |= (1 << cid);
	*(EID *)ds_PackedArray_getElement(signature_ledger, eid) = entity_signatures[eid];

	/* update systems' entity lists */
	ecs_reindexSystems(ecs_getComponentSignature(cid));
	
	return ds_PackedArray_getElement(registered_components[cid], eid);
}

void
ecs_removeComponentInstance(EID eid, CID cid)
{
	/* remove component from memory */
	ds_PackedArray_removeKey(registered_components[cid], eid);

	/* update signature for entity */
	entity_signatures[eid] &= ~(1 << cid);

	/* remove EID from systems' lists */
	ecs_reindexSystems(ecs_getComponentSignature(cid));
}

void *
ecs_getComponentInstance(EID eid, CID cid)
{
	return ds_PackedArray_getElement(registered_components[cid], eid);
}

Signature
ecs_getEntitySignature(EID eid)
{
	return entity_signatures[eid];
}

Signature
ecs_getComponentSignature(CID cid)
{
	return 1 << cid;
}

CID
ecs_getComponentID(Signature signature)
{
	CID cid = 0;
	while(signature != 1) {
		signature = signature >> 1;
		cid++;
	}
	return cid;
}

SID
ecs_registerSystem(CID *query, int query_size, void (*procedure)(EID, CID *))
{
	struct System* s;
	Signature new_signature = 0;
	SID new_sid = system_count++;

	/* generate signature */
	for (int i = 0; i < query_size; i++) {
		new_signature |= (1 << query[i]);
	}

	s = malloc(sizeof(struct System));
	s->system_signature = new_signature;
	s->entity_count = 0;
	s->procedure = procedure;
	s->query = query;

	registered_systems[new_sid] = s;

	if (query_size == 1) {
		s->hint = SINGLE_COLUMN;
	} else {
		s->hint = LEDGER;
		s->query_results = malloc(sizeof(EID) * MAX_ENTITIES);
	}

	ecs_indexSystem(s);

	return new_sid;
}

void
ecs_reindexSystems(Signature dirty_components) {
	SID sid;
	struct System *s;

	for (sid = 0; sid < system_count; sid++) {
		s = registered_systems[sid];
		if ((s->system_signature & dirty_components) == dirty_components) {
			ecs_indexSystem(s);
		}
	}
}

void
ecs_indexSystem(struct System *s)
{
	int i;
	struct PackedArray *component;
	Signature entity_signature;

	switch (s->hint) {
	case SINGLE_COLUMN:
		component = registered_components[ecs_getComponentID(s->system_signature)];
		s->query_results = (EID *)component->index_key_map;
		s->entity_count = component->element_count;
		break;
	case LEDGER:
	default:
		s->entity_count = 0;
		for (i = 0; i < entity_count; i++) {
			entity_signature = ((Signature *)signature_ledger->array)[i];
			if ((entity_signature & s->system_signature) == s->system_signature) {
				s->query_results[s->entity_count++] = signature_ledger->index_key_map[i];
			}
		}
		break;
	}
}

void
ecs_callSystem(SID sid)
{
	int i;
	struct System *s;

	s = registered_systems[sid];
	for(i = 0; i < s->entity_count; i++) {
		s->procedure(s->query_results[i], s->query);
	}
}
