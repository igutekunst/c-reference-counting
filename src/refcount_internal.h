#include <refcount/refcount.h>
#include <stdbool.h>

typedef struct SmartPointer * SmartPointer;
typedef struct ListEntry ListEntry;

struct ListEntry {
    void* item;
    ListEntry* next;
};

typedef struct {
    size_t count;
    ListEntry* head;
} PointerList;


struct SmartPointer {
    size_t ref_count;
    ListEntry * children;
    void * data;
};

static SmartPointer x;

/*
 * Calculate a machine independent offset
 * between the start of a SmartPointer and the data
 * part
 */
#define POINTER_OFFSET ((void * ) &(x->data) - (void * )&(x))


ListEntry* SmartPointerList_append(ListEntry* list, SmartPointer pointer);


size_t SmartPointerList_len(ListEntry* list);

/*
 * Tries to remove pointer from list, returning true
 * on success
 */
bool SmartPointerList_remove(ListEntry** list, struct SmartPointer* pointer);

SmartPointer* SmartPointer_get(void * data);



