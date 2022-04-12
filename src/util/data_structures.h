#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define CASTED_DEREF(TYPE, POINTER)	(*((TYPE *)POINTER))

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
	unsigned int *array;
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

/* PACKED ARRAY */
struct PackedArray {
	void *array;
	int *key_index_map;
	int *index_key_map;
	size_t element_size;
	unsigned int element_count;
	unsigned int capacity;
};

struct PackedArray *ds_PackedArray_init(int capacity, size_t element_size);
void ds_PackedArray_free(struct PackedArray **pa_p);
void *ds_PackedArray_addKey(struct PackedArray *pa, unsigned int key);
void *ds_PackedArray_getElement(struct PackedArray *pa, unsigned int key);
void ds_PackedArray_removeKey(struct PackedArray *pa, unsigned int key);

#endif /* DATASTRUCTURES_H_ */
