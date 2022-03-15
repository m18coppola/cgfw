#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct node_ {
	int key;
	void *data;

	struct node_ *next;
} node;

/* SINGLY LINKED LIST */
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

/* ARRAY QUEUE */
struct ArrayQueue {
	void *array;
	int head;
	int tail;
	int size;
	int capacity;
	size_t element_size;
};

struct ArrayQueue *ds_ArrayQueue_init(int capacity, size_t element_size);
void ds_ArrayQueue_free(struct ArrayQueue **queue_p);
int ds_ArrayQueue_isEmpty(struct ArrayQueue *queue);
int ds_ArrayQueue_isFull(struct ArrayQueue *queue);
void ds_ArrayQueue_enqueue(struct ArrayQueue *queue);
void *ds_ArrayQueue_peekTail(struct ArrayQueue *queue);
void *ds_ArrayQueue_peekHead(struct ArrayQueue *queue);
void *ds_ArrayQueue_dequeue(struct ArrayQueue *queue);

#endif /* DATASTRUCTURES_H_ */