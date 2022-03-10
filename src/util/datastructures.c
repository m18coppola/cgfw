#include <util/datastructures.h>

struct LinkedList *
ds_LinkedList_init(void)
{
	struct LinkedList *l;

	l = malloc(sizeof(struct LinkedList));
	l->size = 0;
	l->head = NULL;

	return l;
}

void
ds_LinkedList_free(struct LinkedList **list_p)
{
	node *finger;
	node *next;
	struct LinkedList *list;

	list = *list_p;

	finger = list->head;
	next = finger;
	while (finger != NULL) {
		next = finger->next;
		free(finger->data);
		free(finger);
		finger = next;
	}

	free(list);
	*list_p = NULL;
}

int
ds_LinkedList_add(struct LinkedList *list, int key, size_t data_size)
{
	/* 
	 * `p` will traverse the linked list.
	 * It is a pointer to a node* (`node*` being a fundamental unit of the linked list)
	 */
	node **p;
	node *new_node;

	new_node = malloc(sizeof(node));
	if (new_node == NULL) {
		fprintf(stderr, "malloc() returned NULL while adding to LinkedList");
		return 1;
	}
	new_node->key = key;
	new_node->data = malloc(data_size);
	new_node->next = NULL;

	p = &(list->head);
	/* (*p) is a node*, which `->` can operate upon */
	while ((*p) != NULL) {
		//node *finger = (*p);
		//finger = finger->next;
		//p = &(finger);
		p = &(*p)->next;
	}
	(*p) = new_node;

	return 0;
}

int
ds_LinkedList_remove(struct LinkedList *list, int key)
{
	node **p;
	node *old_node;

	p = &(list->head);
	while ((*p) && (*p)->key != key) {
		p = &(*p)->next;
	}
	if ((*p) == NULL) {
		fprintf(stderr, "linked list cannot find key during removal.\n");
		return 1;
	}
	old_node = (*p);
	(*p) = (*p)->next;

	free(old_node->data);
	free(old_node);
	return 0;
}

void *
ds_LinkedList_get(struct LinkedList *list, int key)
{
	node **p;

	p = &(list->head);
	while ((*p) && (*p)->key != key) {
		p = &(*p)->next;
	}
	if ((*p) == NULL) {
		return NULL;
	}
	return (*p)->data;
}

void
ds_LinkedList_print(struct LinkedList *list)
{
	node **p;

	p = &(list->head);
	while ((*p) != NULL) {
		printf("%c ", *(char *)((*p)->data));
		p = &(*p)->next;
	}
	printf("\n");
}
