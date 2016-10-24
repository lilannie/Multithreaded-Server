#include "linked_list.h"
#include "worker.h"

// Inits a new linked list
// Returns the new linked list
linked_list_t* init_list() {
  linked_list_t* list = malloc(sizeof(linked_list_t));
  list->size = 0;
  list->head = NULL;
  list->tail = NULL; // {0: requests, 1: workers}
  return list;
}

// Returns the size of the linked list
// Not sure why I have this when I can just do list->size, but I use it so
// whatever :/
int list_size(linked_list_t* list) {
  return list->size;
}

// Returns the data at the node with the given index
void* get_node_data(linked_list_t* list, int index) {
  int i;
  node_t* current_node = list->head;

  for (i = 0; i < index; i++) {
    current_node = current_node->next;
  }
  return current_node->data;
}

// Inserts a new node into the list
// Returns the new node
node_t* insert_node(linked_list_t* list, void* data) {
  node_t* node = (node_t*)malloc(sizeof(node_t));
  node->data = data;
  node->next = NULL;

  if (list->size == 0) {
    list->head = node;
    list->tail = node;
  }

  list->tail->next = node;
  list->tail = node;
  list->size++;

  return node;
}

// Removes the last node from the list
// Returns the removed node for use in free later
node_t* remove_node(linked_list_t* list) {
  // printf("List size: %d\n", list->size);
  if (list->size == 0) {
    return NULL;
  }

  node_t* node = list->head;

  // printf("List head request id: %d", ((request_t *)(node->data))->id);
  if (list->size == 1) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return node;
  }

  node_t* next = list->head->next;
  list->head = next;
  list->head->next = next->next;
  list->size--;

  // printf("Removed\n");
  return node;
}

// Removes the last node from the list
// Returns the removed node for use in free later
node_t* remove_node_tail(linked_list_t* list) {
  if (list->size == 0) {
    return NULL;
  }

  node_t* node = list->tail;
  int i;

  if (list->size == 1) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return node;
  }

  node_t* current_node = list->head;

  for (i = 1; i < list->size - 1; i++) {
    current_node = current_node->next;
  }

  list->tail = current_node;
  current_node->next = NULL;
  list->size--;

  return node;
}

// Destroy the list and frees the mem
// True on success
bool destroy_list(linked_list_t* list) {
  node_t* temp;

  while (list->size > 0) {
    temp = remove_node(list);
    free(temp->data);
    free(temp);
  }
  free(list);
  return true;
}
