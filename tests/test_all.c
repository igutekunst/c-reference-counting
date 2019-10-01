#include <check.h>
#include "test_linked_list.h"
#include "test_refcount.h"
#include <stdlib.h>


int main(int argc, char * argv[]) {
    int number_failed;
    Suite* refcount_suite = refcount_suite_create();
    Suite* linked_list_suite = linked_list_suite_create();

    SRunner *sr = srunner_create (refcount_suite);
    srunner_add_suite(sr, linked_list_suite);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all (sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
