//
// Created by Isaac Gutekunst on 10/1/19.
//

#include "test_linked_list.h"
#include "linked_list.h"

START_TEST(test_linked_list_create) {
    LinkedList* list = linked_list_create();
    ck_assert_ptr_nonnull(list);

    ck_assert_int_eq(0, linked_list_len(list));


}
END_TEST

START_TEST(test_linked_list_append_pop) {
        LinkedList* list = linked_list_create();
        int a, b, c;
        linked_list_append(list, &a);
        linked_list_append(list, &b);
        linked_list_append(list, &c);

        ck_assert_int_eq(3, linked_list_len(list));
        void* ret;

        ck_assert(linked_list_pop(list, &ret));
        ck_assert_ptr_eq(ret, &c);
        ck_assert(linked_list_pop(list, &ret));
        ck_assert_ptr_eq(ret, &b);
        ck_assert(linked_list_pop(list, &ret));
        ck_assert_ptr_eq(ret, &a);

        ck_assert_int_eq(0, linked_list_len(list));
        ck_assert(linked_list_pop(list, &ret) == false);


}
END_TEST


Suite* linked_list_suite_create(void) {
    Suite* s = suite_create("Linked List");
    TCase* tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_linked_list_create);
    tcase_add_test(tc_core, test_linked_list_append_pop);

    suite_add_tcase(s, tc_core);
    return s;
}
