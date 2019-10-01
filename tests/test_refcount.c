//
// Created by Isaac Gutekunst on 10/1/19.
//

#include "refcount_internal.h"
#include "test_refcount.h"

START_TEST(test_smart_pointer) {
        int* foo = rc_alloc(sizeof(int));
        struct SmartPointer* sp = SmartPointer_get(foo);
        ck_assert_int_eq(1, sp->ref_count);

        rc_retain(foo);
        ck_assert_int_eq(2, sp->ref_count);
        rc_release(foo);
        ck_assert_int_eq(1, sp->ref_count);
        rc_release(foo);
}
END_TEST


Suite* refcount_suite_create(void) {
    Suite* s = suite_create("Reference Counting");
    TCase* tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_smart_pointer);

    suite_add_tcase(s, tc_core);
    return s;
}
