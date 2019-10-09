//
// Created by Isaac Gutekunst on 10/1/19.
//

#include "refcount_internal.h"
#include "test_refcount.h"

START_TEST(test_smart_pointer)
    {
        int* foo = rc_alloc(sizeof(int));
        struct SmartPointer* sp = SmartPointer_get(foo);
        ck_assert_int_eq(1, sp->ref_count);

        rc_retain(foo);
        ck_assert_int_eq(2, sp->ref_count);
        rc_release(foo);
        ck_assert_int_eq(1, sp->ref_count);
        rc_release(foo);
        ck_assert_int_eq(0, rc_debug_num_allocations());
    }
END_TEST

START_TEST(test_retain_in_simple)
    {
        void* parent = rc_alloc(4);
        int* child = rc_alloc_in(parent, sizeof(int));

        ck_assert_int_eq(2, rc_debug_num_allocations());
        rc_retain(child);
        struct SmartPointer* child_sp = SmartPointer_get(child);
        ck_assert_int_eq(2, child_sp->ref_count);

        rc_release(parent);
        ck_assert_int_eq(1, child_sp->ref_count);
        rc_release(child);

        ck_assert_int_eq(0, rc_debug_num_allocations());
    }
END_TEST

START_TEST(test_retain_in_chain)
    {
        const size_t CHAIN_LEN = 10;
        void* top = rc_alloc(4);
        void* parent = top;
        void* child;
        for (size_t i = 0; i < CHAIN_LEN; i++) {
            child = rc_alloc_in(parent, sizeof(int));
            parent = child;
        }

        ck_assert_int_eq(11, rc_debug_num_allocations());

        rc_release(top);

        ck_assert_int_eq(0, rc_debug_num_allocations());
    }
END_TEST

START_TEST(test_two_parents_crashes)
    {
        void* parent_1 = rc_alloc(0);
        void* parent_2 = rc_alloc(0);

        void* child = rc_alloc_in(parent_1, 0);
        int status = rc_retain_in(parent_2, child);
        ck_assert_int_eq(-1, status);
        rc_release(parent_1);
        rc_release(parent_2);
        ck_assert_int_eq(0, rc_debug_num_allocations());

    }
END_TEST


Suite* refcount_suite_create(void) {
    Suite* s = suite_create("Reference Counting");
    TCase* tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_smart_pointer);
    tcase_add_test(tc_core, test_retain_in_simple);
    tcase_add_test(tc_core, test_retain_in_chain);
    tcase_add_test(tc_core, test_two_parents_crashes);

    suite_add_tcase(s, tc_core);
    return s;
}
