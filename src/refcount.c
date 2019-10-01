#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "refcount_internal.h"


SmartPointer* SmartPointer_get(void* data) {
    SmartPointer* p = (SmartPointer*) (data - POINTER_OFFSET);
    return p;
}


void* rc_alloc(size_t bytes) {
    SmartPointer p = malloc(sizeof(SmartPointer) + bytes);

    if (!p) {
        return NULL;
    }

    p->ref_count = 1;
    p->children = NULL;
    p->data = ((void*) p) + POINTER_OFFSET;
    return p->data;
}


int rc_retain(void* data) {
    if (!data) { return -1; }

    SmartPointer p = SmartPointer_get(data);
    p->ref_count++;
}


int rc_release(void* data) {
    if (!data) { return -1; }

    SmartPointer p = SmartPointer_get(data);
    if (p->ref_count) {
        p->ref_count--;
    }

    if (p->ref_count == 0) {
        free(p);
    }
    return 0;
}



