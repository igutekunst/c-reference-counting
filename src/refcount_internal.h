#include <refcount/refcount.h>
#include <stdbool.h>
#include "list.h"


typedef struct SmartPointer SmartPointer;


struct SmartPointer {
    size_t ref_count;
    /**
     * Keep track of all allocations for debug purposes
     */
    struct list_head allocation_list_head;
    struct list_head sibling_list;
    struct list_head children_list;
    void * data;
    // TODO add de-allocator pointer too!
};

static struct SmartPointer x;

/*
 * Calculate a machine independent offset
 * between the start of a SmartPointer and the data
 * part
 */

SmartPointer* SmartPointer_get(void * data);





