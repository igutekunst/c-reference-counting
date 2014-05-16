#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <refcount.h>

typedef struct SmartPointer * SmartPointer;
typedef struct SmartPointerList * SmartPointerList;
struct SmartPointerList {
  SmartPointer pointer;
  SmartPointerList next;
};


struct SmartPointer {
  size_t ref_count;
  SmartPointerList * children;
  void * data;
};

static SmartPointer x;

/*
 * Calculate a machine indepentent offset
 * between the start of a SmartPointer and the data 
 * part
 */
#define POINTER_OFFSET ((void * ) &(x->data) - (void * )&(x))


/*
 * Create a new SmartPointerList with list
 * as the tail.
 */
SmartPointerList append_SmartPointer(SmartPointerList list, SmartPointer pointer) {
  SmartPointerList newList = (SmartPointerList)  malloc(sizeof(struct SmartPointerList));
  newList->pointer = pointer;
  newList->next = list;
  return newList;
}


size_t len_SmartPointerList(SmartPointerList list) {
  SmartPointerList current  = list;
  size_t i = 0;
  while (current) {
    i++;
    current = current->next;
  }
  return i;
}


/*
 * Tries to emove pointer from list, returning true
 * on success
 */
bool remove_SmartPointer(SmartPointerList * list, SmartPointer pointer) {
  SmartPointerList current  = *list;
  SmartPointerList prev     = NULL;

  while (current) {
    if (current->pointer == pointer) {
      if (prev) {

        // If there was a previous item, we need to 
        // unlink the current item from the list
        // and then free it.
        
        // If the item to be freed is the last entry in the list
        // setting prev->next = current->next
        // is equivilent to setting it to NULL
        prev->next = current->next;
        free(current);

        return true;
      } else {

        prev = current->next;
        free(current);
        
        // If there is not a previous
        // list entry, then we are modifying 
        // the head of the list.
        // Since this modifies the list in place,
        // we need to point the list at the new head
        *list = prev; 

        return true;
      }
    }
    prev = current;
    current = current->next;
  }

  return false;
}


SmartPointer get_smart_pointer(void * data) {
  SmartPointer p =  (SmartPointer) (data - POINTER_OFFSET);
  return p;
}


void * alloc(size_t bytes) {
  SmartPointer p = malloc(sizeof(SmartPointer) + bytes);

  if (! p) {
    return NULL;
  }

  p->ref_count = 1;
  p->children = NULL;
  p->data = ((void *) p ) + POINTER_OFFSET;
  return p->data;
}


int retain(void * data) {
  if (!data) { return -1; }

  SmartPointer p = get_smart_pointer(data);
  p->ref_count++;
}


int release(void * data) {
  if (!data) { return -1; }

  SmartPointer p = get_smart_pointer(data);
  if (p->ref_count) {
      p->ref_count--;
  }

  if (p->ref_count == 0) {
      printf("Refcount 0. freeing\n");
      free(p) ;
  }
}



