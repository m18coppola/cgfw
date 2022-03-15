#ifndef COMPONENT_MANAGER_H_
#define COMPONENT_MANAGER_H_

#include <stdio.h>

#define MAX_COMPONENTS 32

typedef unsigned int CID;

struct ecs_ComponentManager {
	struct {
		int size;
	} CID_Queue;
};


#endif /* COMPONENT_MANAGER_H_ */
