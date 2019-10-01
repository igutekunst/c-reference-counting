cmake_minimum_required(VERSION 3.12)

set(CMAKE_MACOSX_RPATH 1)
project(refcount C CXX)

set(CMAKE_C_STANDARD 99)

link_directories(/usr/local/lib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(check REQUIRED)

add_library(refcount
        src/refcount.c
        src/refcount_internal.h
        include/refcount/refcount.h
        src/linked_list.c src/linked_list.h)

target_include_directories(refcount PRIVATE include)

add_executable(test_all
        tests/test_all.c
        tests/test_linked_list.c tests/test_linked_list.h tests/test_refcount.c tests/test_refcount.h)

LINK_DIRECTORIES(/usr/local/lib)

target_link_libraries(test_all ${CHECK_LIBRARIES} refcount)
target_include_directories(test_all PRIVATE ${CHECK_INCLUDE_DIRS})
target_include_directories(test_all PRIVATE include)
target_include_directories(test_all PRIVATE src)



