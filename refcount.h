#ifndef REFCOUNT_H
#define REFCOUNT_H
/**
 * Reference counting memory management library
 * ============================================
 *
 * Note: The implementation does not include 
 * hierarchical memory management
 *
 * This library provides functions to manage memory with 
 * reference counting.
 *
 * This allows the programmer to allocate memory, retaining 
 * multiple references to a single buffer, and freeing the
 * memory when all references are released.
 *
 * The library is not magic however. Care must be taken to 
 * retain and release memory in appropriate locations by calling 
 * the relevant functions.
 *
 * Hierarchical Memory Management
 * ==============================
 *
 * To facilitate complex data structures with multiple references  
 * to data and nested hierarchical data, the library provides
 * hierarchical memory management.
 *
 * It does this with the concept of nested memory allocation, 
 * where memory can be allocated "within" the scope of another 
 * block of memory.
 *
 * When a parent block of memory is freed, all of its children 
 * are released (and freed) as well.
 * 
 * Warnings
 * ========
 * The library incurs a space overhead of sizeof(size_t) per 
 * block of memory allocated + any overhead due to memory
 * alignment introduced by the underlying malloc implementation
 * OS.
 *
 * Examples
 * ========
 *
 *
 */

/**
 * Allocate a block of memory that can be refcounted.
 * Sets the initial internal refcount to 1
 *
 * @param bytes number of bytes of allocate
 *
 * @return pointer to allocated bytes, or NULL
 * on failure
 *
 */
void *  alloc     (size_t bytes);


/**
 * Retain a reference to a pointer allocated 
 * with alloc. Increases the internal reference count
 * and returns the new value.
 *
 * @param data pointer to memory allocated with alloc
 *
 * @return new refernce count, or -1 on failure
 */
int     retain    (void * data);


/**
 * Release memory allocated by alloc.
 * Decreses the internal reference count, returning it.
 * If the internal reference count reaches zero, the 
 * memory is freed.
 *
 * If data contains children and its reference count
 * reaches zero, release will be called on
 * each of them before data is freed.
 *
 * @param data pointer to memory allocated with alloc
 *
 * @return new reference count, or -1 on failure
 */
int     release   (void * data);


/**
 * Allocate a block of memory that can be refcounted,
 * and creates a reference within parent, and sets 
 * it's internal reference count to 1.
 *
 * When the parent is released, release will be called
 * on this pointer as well.
 *
 * @param parent pointer to memory allocated with
 * alloc that will retain a reference to the allocated memory
 *
 * @param bytes number of bytes of allocate
 *
 * @return pointer to allocated bytes, or NULL
 * on failure
 *
 */
void *  alloc_in      (void * parent, size_t bytes);


/**
 * Retain a reference to a pointer allocated 
 * with alloc within parent.
 *
 * Increases the internal reference count
 * and returns the new value.
 *
 * @param parent pointer to memory allocated with
 * alloc that will retain a reference to data
 *
 * @param data pointer to memory allocated with alloc
 *
 * @return new refernce count, or -1 on failure
 */
int     retain_in     (void * parent, void * data);


/**
 * Release memory allocated with alloc from parent, 
 * decrementing the reference count
 * 
 *
 * This function will attempt to releas data from 
 * parent. If parent does not contain a reference to data,
 * do nothing and return the unchanged reference count.
 *
 * @param parent pointer to memory allocated with
 * alloc containing a reference to data
 *
 * @param data pointer to memory allocated with alloc
 *
 * @return new refernce count, or -1 on failure
 *
 */
int     release_from  (void * parent, void * data);

#endif
