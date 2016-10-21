#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <limits.h>

typedef struct node {
	int request
	char *args[21];
	struct node *next; 
}node_t;

typedef struct list {
	struct node *head;
	int length;
}list_t;

node_t* create_node(int request, char *args[21], node_t *next){
	node_t *node = malloc(sizeof(node_t));

}

list_t* init_list(node_t *nodes[]){
	node_t *head = malloc(sizeof(node_t));
	head->request = NULL;
	head->args = NULL;
	head->next = NULL;

	list_t *list = malloc(sizeof(list_t));
	list->head = head;

	node_t *current = list->head; 
	int i = 0;
	for (i = 0; i < sizeof(nodes)/sizeof(node_t); i++) {
		if (nodes[i] != NULL) {
			if (current->request == NULL) {
				current->request = nodes[i].request;
				current->args = nodes[i].args;
				current->next = nodes[i].next;
			}
			else {
				current->next = &nodes[i];
				current = current->next;
			}
		}
	}
	return list;
}

void push(list_t *list, node_t *node) {
	node_t* current = list->head;

	while (current ->next != NULL) {
		current = current->next;
	}

	node->next = NULL;
	current->next = node;
	list->size++;
}

int pop(list_t *list) {
	node_t *next_node = NULL;

	if (list->head == NULL) {
		return -1; 
	}

	next_node = list->head->next;
	int retval = list->head->transaction;
	free(list->head);
	list->head = next_node;

	return retval;
}

void free(list_t *list) {
	node_t *current = list_t->head;
	node_t *next = list_t->head;

	while (next != NULL) {
		current = next;
		next = current->next;
		free(current);
	}
}