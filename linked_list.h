#ifndef LINKED_LIST
#define LINKED_LIST

#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0

typedef struct node_t {
  void* data;
  struct node_t* next;
} node_t;

typedef struct {
  node_t* head;
  node_t* tail;
  int size;
} linked_list_t;

linked_list_t* init_list();
int list_size(linked_list_t* list);
void* get_node_data(linked_list_t* list, int index);
node_t* insert_node(linked_list_t* list, void* data);
node_t* remove_node(linked_list_t* list);
bool destroy_list(linked_list_t* list);

#endif
