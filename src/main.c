#include <stdio.h>
#include <util/data_structures.h>
#include <ecs/entity_manager.h>
#include <ecs/component_manager.h>

int
main (int argc, char *argv[])
{
	printf("Allocating a Component Array...\n");
	struct ComponentArray *component_list;
	component_list = ecs_ComponentArray_init((unsigned int)0, sizeof(long), 10);

	ecs_ComponentArray_insert(component_list, (unsigned int) 1);
	ecs_ComponentArray_insert(component_list, (unsigned int) 2);
	ecs_ComponentArray_insert(component_list, (unsigned int) 3);
	*(long *)ecs_ComponentArray_get(component_list, (unsigned int) 1) = 111;
	*(long *)ecs_ComponentArray_get(component_list, (unsigned int) 2) = 222;
	*(long *)ecs_ComponentArray_get(component_list, (unsigned int) 3) = 333;

	for(int i = 0; i <= 10; i++) {
		printf("%ld, ", *(long *)(component_list->array + (i * sizeof(long))));
	}
	printf("\n");

	printf("removing key 2...\n");
	ecs_ComponentArray_remove(component_list, (unsigned int)2);

	for(int i = 0; i <= 10; i++) {
		printf("%ld, ", *(long *)(component_list->array + (i * sizeof(long))));
	}
	printf("\n");


	printf("inserting 2 back in...\n");
	ecs_ComponentArray_insert(component_list, (unsigned int) 2);
	CASTED_DEREF(long, ecs_ComponentArray_get(component_list, (unsigned int) 2)) = 222;


	for(int i = 0; i <= 10; i++) {
		printf("%ld, ", *(long *)(component_list->array + (i * sizeof(long))));
	}
	printf("\n");
	printf("ensuring keys are mapped correctly...\n");
	for(int i = 1; i <= 3; i++) {
		printf("%ld, ", CASTED_DEREF(long, ecs_ComponentArray_get(component_list, (unsigned int) i)));
	}
	printf("\n");




	printf("Releasing Component Array...\n");
	ecs_ComponentArray_free(&component_list);
}
