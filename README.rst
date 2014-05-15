c-reference-counting
====================

Simple library for managing memory with reference counting


Reference counting memory management library
============================================

Note: The implementation does not include 
hierarchical memory management yet. 

See refcount.h: https://github.com/igutekunst/c-reference-counting/blob/master/refcount.h

This library provides functions to manage memory with 
reference counting.

This allows the programmer to allocate memory, retaining 
multiple references to a single buffer, and freeing the
memory when all references are released.

The library is not magic however. Care must be taken to 
retain and release memory in appropriate locations by calling 
the relevant functions.

Hierarchical Memory Management
==============================

To facilitate complex data structures with multiple references
to data and nested hierarchical data, the library provides
hierarchical memory management.

It does this with the concept of nested memory allocation, 
where memory can be allocated "within" the scope of another 
block of memory.

When a parent block of memory is freed, all of its children 
are released (and freed) as well.

Warnings
========
The library incurs a space overhead of sizeof(size_t) per 
block of memory allocated + any overhead due to memory
alignment introduced by the underlying malloc implementation
OS.

Examples
========


