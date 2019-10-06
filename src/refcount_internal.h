#include <refcount/refcount.h>
#include <stdbool.h>
#include "list.h"


typedef struct SmartPointer SmartPointer;


struct SmartPointer {
    size_t ref_count;
    struct list_head siblings;
    struct list_head children;
    void * data;
    // TODO this is a lot of overhead for a single pointer!
};

static struct SmartPointer x;

/*
 * Calculate a machine independent offset
 * between the start of a SmartPointer and the data
 * part
 */

SmartPointer* SmartPointer_get(void * data);





