#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct node_ {
	int key;
	void *data;
	size_t data_size;
	struct node_ *next;
} node;

struct LinkedList {
	int size;
	node *head;
};

struct LinkedList *ds_LinkedList_init(void);
void ds_LinkedList_free(struct LinkedList **list_p);
int ds_LinkedList_add(struct LinkedList *list, int key, size_t data_size);
int ds_LinkedList_remove(struct LinkedList *list, int key);
void *ds_LinkedList_get(struct LinkedList *list, int key);
void ds_LinkedList_print(struct LinkedList *list);

#endif /* DATASTRUCTURES_H_ */
