//
// Created by Isaac Gutekunst on 10/1/19.
//

#ifndef REFCOUNT_LINKED_LIST_H
#define REFCOUNT_LINKED_LIST_H
#include <stdint.h>
#include <stdbool.h>

typedef struct LinkedList LinkedList;

LinkedList* linked_list_create();
size_t linked_list_len(LinkedList* list);
void linked_list_append(LinkedList* list, void* item);
bool linked_list_get(LinkedList* list, size_t index, void** ret);
bool linked_list_pop(LinkedList* list, void** ret);

#endif //REFCOUNT_LINKED_LIST_H
