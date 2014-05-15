#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "refcount.h"

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

/*
 * Create a new SmartPointerList with list
 * as the tail.
 */
SmartPointerList append_SmartPointer(SmartPointerList list, SmartPointer pointer) {
  SmartPointerList newList = (SmartPointerList)  malloc(sizeof(struct SmartPointerList));
  newList->pointer = pointer;
  if (list) {
    newList->next = NULL;
  } else {
    newList->next = list;
  }

  return newList;
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
  SmartPointer p =  (SmartPointer) (data - sizeof(size_t));
  return p;
}


void * alloc(size_t bytes) {
  SmartPointer p = malloc(sizeof(SmartPointer) + bytes);

  if (! p) {
    return NULL;
  }

  p->ref_count = 1;
  p->data = ((void *) p ) + sizeof(size_t);
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



int main() {
  void * t = alloc(10);
  SmartPointer p = get_smart_pointer(t);
  printf("refcount %d \n", p->ref_count);

  retain(t);
  printf("refcount %d \n", p->ref_count);

  release(t);
  printf("refcount %d \n", p->ref_count);

  release(t);

}
