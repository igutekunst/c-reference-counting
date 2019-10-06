//
// Created by Isaac Gutekunst on 10/1/19.
//
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "linked_list.h"


struct ListEntry {
    void* item;
    struct ListEntry* next;
    struct ListEntry* prev;
    LinkedList* list;
};

typedef struct ListEntry ListEntry;

struct LinkedList {
    size_t len;
    struct ListEntry* head;
    struct ListEntry* tail;
};

void linked_list_init(LinkedList* list) {
    list->len = 0;
    list->tail = NULL;
    list->head = NULL;
}

LinkedList* linked_list_create() {
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    linked_list_init(list);
    return list;
}


size_t linked_list_len(LinkedList* list) {
    if (list == NULL) {
        fprintf(stderr, "linked_list_len error: list == NULL\n");
        exit(EXIT_FAILURE);
    }
    return list->len;
}


void linked_list_append(LinkedList* list, void* ptr) {
    if (list == NULL) {
        fprintf(stderr, "linked_list_append error: list == NULL\n");
        exit(EXIT_FAILURE);
    }

    struct ListEntry* item = (struct ListEntry*) malloc(sizeof(struct ListEntry));
    if (item == NULL) {
        fprintf(stderr, "linked_list_append error: allocating item failed. %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    item->item = ptr;
    item->next = NULL;

    item->prev = list->tail;

    if (list->head == NULL) {
        list->head = list->tail = item;
    } else {
        list->tail->next = item;
        list->tail = item;
    }

    list->len++;
}


bool linked_list_get(LinkedList* list, size_t index, void** ret) {
    if (list == NULL) {
        fprintf(stderr, "linked_list_get error: list == NULL\n");
        exit(EXIT_FAILURE);
    }

    if (ret == NULL) {
        fprintf(stderr, "linked_list_get error: ret == NULL\n");
        exit(EXIT_FAILURE);
    }

    ListEntry* list_entry = list->head;

    size_t i;
    for(i = 0; i < index; i++) {
        if (list_entry == NULL) {
            break;
        }
        list_entry = list_entry->next;
    }

    if (i != index) {
        return false;
    }

    if (list_entry) {
        *ret = list_entry->item;
        return true;
    }

    return false;
}


bool linked_list_pop(LinkedList* list, void** ret) {

    if (list == NULL) {
        fprintf(stderr, "linked_list_get error: list == NULL\n");
        exit(EXIT_FAILURE);
    }

    if (ret == NULL) {
        fprintf(stderr, "linked_list_get error: ret == NULL\n");
        exit(EXIT_FAILURE);
    }
    if (list->len == 0) {
        return false;
    }

    ListEntry* tail = list->tail;

    assert(tail);

    if (tail->prev) {
        tail->prev->next = NULL;
    }

    list->tail = tail->prev;
    *ret = tail->item;
    free(tail);
    list->len--;

    return true;
}

//TODO this is all fine and everything, except Linux does it better:
// https://kernelnewbies.org/FAQ/LinkedLists
