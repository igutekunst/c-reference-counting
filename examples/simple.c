#include <refcount/refcount.h>


void rc_init(void);


void rc_deinit(void);


void rc_enter_scope();


void rc_leave_scope();


void nested_function(void* input) {
    static void* persistent_pointer = NULL;

    rc_enter_scope();
    void* temp_var = rc_alloc(10); // refcount = 1

    persistent_pointer = input;
    rc_retain(input); // Refcount = 2

    rc_leave_scope(); // temp_var will be freed
}

int main(int argc, char* argv[]) {

    // Initialize the reference counting library. This must be done once per
    // thread of execution, as rc_init uses an internal static variable to keep track
    // of nested reference counted scopes

    // This creates a global scope that keeps track of all allocations
    rc_init();

    // Create a scope for tracking allocations
    // Any memory allocated and not retained will be released and freed
    // at the matching rc_exit_scope
    rc_enter_scope(); // Call this scope A

    // Allocate two blocks of memory. These variables will allocated in scope A
    void* foo = rc_alloc(100); // Refcount = 1
    void* bar = rc_alloc(100); // Refcount = 1

    // Call a function.
    nested_function(foo);

    // Leaving the scope decreases the refcount of all
    // contained pointers. This decreases the refcount of bar to 0, freeing its
    // memory.
    //
    // Since foo was retained by nested_function, its refcount is now 1
    rc_leave_scope();

    // At this point, we free any memory that wasn't freed already.
    //
    // With some debug printing, this could help find memory leaks that occur because
    // memory was retained too many times
    rc_deinit();
}
/*
 * Some interesting observations
 *
 * Memory can still leak. This occurs if some function calls rc_retain, and never rc_release.
 *
 * Therefore, it's very important to assume calling rc_retain is taking on a large responsibility. Typically,
 * this would be done inside of libraries that are extensively tested for memory leaks.
 *
 * Since this library is intended to be used by OOC, most user programs will not call rc_alloc directly, but
 * will call new, and subsequently store objects inside of collections that in turn handle memory responsibly.
 *
 * Of course, user programs will need to hold onto pointers that go out of scope, and that's what the global cleanup is for.
 *
 * A few real-world larger scale programs should illuminate the kind of patterns pitfalls that will emerge in real world
 * programs.
 *
 * Nested Pointers
 * (To live in examples/nested_pointeres.c)
 * The idea with nested allocations is that pointers can be allocated within the scope of another pointer, rather than
 * within a scope delimited by rc_enter_scope and rc_exit_scope. By allocating memory that belongs to other allocations,
 * it becomes easier to make dynamic data structures that need to make allocations in order to grow or otherwise
 * operate.
 *
 * The idea is that when a containing pointer's refcount goes to zero, all children_list pointers will have their refcount
 * decremented as well, freeing the memory if necessary
 *
 *
 * OOC usage
 * This library was written to support OOC. Most code in OOC should use OOC types, enabling dynamic typing,
 * polymorphism, generic functions and other features. Without refcounting, memory management is quit cumbersome, especially
 * when lots of temporary variables are need to build up data structures, or to pass arguments to functions
 *
 *  With refcounting, variables can be created easily, and behave almost as if they where stack allocated, although in
 *  reality, they are allocated on the heap, and freed by calls to rc_exit_scope.
 *
 *  The reference counting library supports custom destructor functions that will be called before/instead of calling
 *  free on the memory..
 *
 *  These allow complex objects to de-initialize resources gracefully. Of course objects are welcome and encouraged to
 *  use the refcounting features, especially nested-pointers to make this easy.
 *
 *  This needs to be investigated more.. I think as long as all memory allocated on behalf of an object instance is
 *  nested within the main allocation for the object's header, then the destructor never needs to manually release any
 *  memory, and the refcounting library can handle it, leaving the destructor with fewer responsibilities.
 *
 *  Of course, for container types, it's expected for them to release memory when items are removed from the collection.
 *  If they are wrapped internally, the wrapper object (ListNode for a linked list) can be released, and the refcounting
 *  library will in turn release the memory contained in the ListNode, since it was allocated with ListNode as a parent.
 *
 *  Overhead:
 *
 *  All of this creates memory and CPU overhead. Each allocation will contain five pointers and a reference count.
 *
 *  The pointers are: children_list, refcount, next, previous, (and a pointer to the data (optional))
 *
 *  The next/prev pointers are for storing the allocation in a linked list. This is used for nested allocations.
 *  The doubly linked nature makes it easy to perform O(1) deletions
 *
 *  The parent pointer points at the parent allocation, and is maybe not necessary.
 *
 *  The children_list pointer points at a linked list of nested allocations.
 *
 *  The refcount is the refcount :)
 *
 *
 */
