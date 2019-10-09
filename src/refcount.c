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
// TODO use thread local storage, or some other mechanism for this to be thread safe
LIST_HEAD(allocation_list);

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

    INIT_LIST_HEAD(&p->children_list);
    INIT_LIST_HEAD(&p->sibling_list);


    p->ref_count = 1;
    p->data = ((void*) p) + offsetof(struct SmartPointer, data) + sizeof(void*);

    list_add(&p->allocation_list_head, &allocation_list);
    return p->data;
}

void* rc_alloc_in(void* parent, size_t bytes) {
    void* child = rc_alloc(bytes);
    if (child == NULL) {
        fprintf(stderr, "rc_alloc_in error: %s\n", strerror(errno));
        exit(errno);
    }
    rc_retain_in(parent, child);

    // retain_in increases refcount, though we only
    // want the refcount to stay as 1
    // TODO make this faster by not calling rc_release.
    // Maybe this can be done by making alloc_in do all of the allocating
    rc_release(child);//
    return child;
}


int rc_retain(void* data) {
    if (!data) { return -1; }

    SmartPointer* p = SmartPointer_get(data);
    p->ref_count++;
}


int rc_retain_in(void* _parent, void* _child) {
    if (!_child) { return -1; }
    if (!_parent) { return -1; }

    SmartPointer* parent = SmartPointer_get(_parent);
    SmartPointer* child = SmartPointer_get(_child);
    if (!list_empty(&child->sibling_list)) {
        fprintf(stderr, "Attempt to assign 2nd parent (%p) to pointer %p\n",
                parent, child);
        return -1;
    }
    list_add(&child->sibling_list, &parent->children_list);
    child->ref_count++;

    return child->ref_count;
}


static int internal_rc_release(SmartPointer* p) {

    if (p->ref_count) {
        p->ref_count--;
    }

    if (p->ref_count == 0) {

        SmartPointer* child = NULL;
        if (!list_empty(&p->children_list)) {
            list_for_each_entry(child, &p->children_list, sibling_list) {
                if (child) {
                    internal_rc_release(child);
                }
            }
        }

        list_del(&p->allocation_list_head);
        free(p);
    }
}

int rc_release(void* data) {
    if (!data) { return -1; }
    SmartPointer* p = SmartPointer_get(data);
    internal_rc_release(p);
    return 0;
}

void rc_debug_print_allocations(void) {
    SmartPointer* entry;
    list_for_each_entry(entry, &allocation_list, allocation_list_head) {
        printf("%p - rc %zu", entry, entry->ref_count);
    }
}

size_t rc_debug_num_allocations(void) {
    SmartPointer* entry;
    size_t count = 0;
    list_for_each_entry(entry, &allocation_list, allocation_list_head) {
        count++;
    }
    return count;
}
/**
 *
 * Notes:
 *
 * There will be a top level list/vector of all allocations. This will be
 * used at the end of the execution to discover memory leaks. It will optionally be
 * created.
 *
 * Each scope will have an allocation of zero bytes (only header), that will contain
 * a list of all allocations made in a scope.
 *
 * A scope is entered with rc_enter_scope, and exited with rc_exit_scope.
 *
 * Nested scopes are possible. This is done by making multiple calls to rc_enter_scope without
 * calling rc_exit_scope. Allocations will occur in the scope associated with the most
 * recent call to rc_enter_scope.
 *
 * Allocations can only be allocated in a scope (default), or within a parent allocation.
 *
 */


