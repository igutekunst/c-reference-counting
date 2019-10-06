#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include "refcount_internal.h"

// See https://medium.com/@414apache/kernel-data-structures-linkedlist-b13e4f8de4bf
// for information on using the Linux linked list implementation

SmartPointer* SmartPointer_get(void* data) {
    SmartPointer* p = (SmartPointer*) (data - offsetof(struct SmartPointer, data) - sizeof(void*));
    return p;
}


void* rc_alloc(size_t bytes) {
    SmartPointer* p = malloc(sizeof(SmartPointer) + bytes);

    if (!p) {
        fprintf(stderr, "rc_alloc error: %s\n", strerror(errno));
        exit(errno);
    }

     INIT_LIST_HEAD(&p->children);
     INIT_LIST_HEAD(&p->siblings);


    p->ref_count = 1;
    p->data = ((void*) p) + offsetof(struct SmartPointer, data) + sizeof(void*);
    return p->data;
}


int rc_retain(void* data) {
    if (!data) { return -1; }

    SmartPointer* p = SmartPointer_get(data);
    p->ref_count++;
}


int rc_release(void* data) {
    if (!data) { return -1; }

    SmartPointer* p = SmartPointer_get(data);
    if (p->ref_count) {
        p->ref_count--;
    }

    if (p->ref_count == 0) {
        free(p);
    }
    return 0;
}



