#include <stdio.h>
#include <util/datastructures.h>

int
main (int argc, char *argv[])
{
	printf("Adding 1, 2, 3...\n");
	struct LinkedList *myList = ds_LinkedList_init();
	ds_LinkedList_add(myList, 1, sizeof(int));
	ds_LinkedList_add(myList, 2, sizeof(int));
	ds_LinkedList_add(myList, 3, sizeof(int));

	*(char *)ds_LinkedList_get(myList, 1) = 'a';
	*(char *)ds_LinkedList_get(myList, 2) = 'b';
	*(char *)ds_LinkedList_get(myList, 3) = 'c';

	ds_LinkedList_print(myList);
	printf("Removing 2...\n");
	ds_LinkedList_remove(myList, 2);
	ds_LinkedList_print(myList);
	printf("Removing 4...\n");
	ds_LinkedList_remove(myList, 4);
	ds_LinkedList_print(myList);
	printf("freeing list...\n");
	ds_LinkedList_free(&myList);
	return 0;
}
