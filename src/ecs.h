#ifndef ECS_H_
#define ECS_H_

#include <stdio.h>
#include <util/data_structures.h>

#define MAX_ENTITIES 1000
#define MAX_COMPONENTS 32
#define MAX_SYSTEMS 32

#define GET_COMPONENT(T, eid, cid) ((T *)ecs_getComponentInstance(eid, cid))
#define ADD_COMPONENT(T, eid, cid) ((T *)ecs_addComponentInstance(eid, cid))

typedef unsigned int CID;
typedef unsigned int EID;
typedef unsigned int SID;
typedef unsigned int Signature;

enum CacheHint {SINGLE_COLUMN, LEDGER};

struct System
{
	Signature system_signature;
	EID *query_results;
	CID *query;
	int entity_count;
	void (*procedure)(EID, CID *);
	enum CacheHint hint;
};


void ecs_init(void);
void ecs_exit(void);
EID ecs_makeEntity(void);
void ecs_removeEntity(EID eid);
CID ecs_registerComponent(size_t component_size);
void *ecs_addComponentInstance(EID eid, CID cid);
void ecs_removeComponentInstance(EID eid, CID cid);
void *ecs_getComponentInstance(EID eid, CID cid);
Signature ecs_getEntitySignature(EID eid);
CID ecs_getComponentID(Signature signature);
SID ecs_registerSystem(CID *query, int query_size, void (*procedure)(EID, CID *));
void ecs_reindexSystems(Signature dirty_components);
void ecs_indexSystem(struct System *s);
void ecs_callSystem(SID sid);
Signature ecs_getComponentSignature(CID cid);

#endif /* ECS_H_ */
