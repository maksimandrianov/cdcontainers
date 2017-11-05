#include <CUnit/Basic.h>
#include "test_common.h"


int main(int argc, char** argv)
{
        CU_pSuite p_suite = NULL;

        if (CUE_SUCCESS != CU_initialize_registry())
                return CU_get_error();

        p_suite = CU_add_suite("VECTOR TESTS", NULL, NULL);
        if (p_suite == NULL) {
                CU_cleanup_registry();
                return CU_get_error();
        }

        if (CU_add_test(p_suite, "test_ctor", test_vector_ctor) == NULL ||
            CU_add_test(p_suite, "test_ctor_list", test_vector_ctor_list) == NULL ||
            CU_add_test(p_suite, "test_reserve", test_vector_reserve) == NULL ||
            CU_add_test(p_suite, "test_data", test_vector_data) == NULL ||
            CU_add_test(p_suite, "test_get", test_vector_get) == NULL ||
            CU_add_test(p_suite, "test_at", test_vector_at) == NULL ||
            CU_add_test(p_suite, "test_front", test_vector_front) == NULL ||
            CU_add_test(p_suite, "test_back", test_vector_back) == NULL ||
            CU_add_test(p_suite, "test_insert", test_vector_insert) == NULL ||
            CU_add_test(p_suite, "test_erase", test_vector_erase) == NULL ||
            CU_add_test(p_suite, "test_clear", test_vector_clear) == NULL ||
            CU_add_test(p_suite, "test_push_back", test_vector_push_back) == NULL ||
            CU_add_test(p_suite, "test_pop_back", test_vector_pop_back) == NULL ||
            CU_add_test(p_suite, "test_swap", test_vector_swap) == NULL) {
                CU_cleanup_registry();
                return CU_get_error();
        }

        p_suite = CU_add_suite("LIST TESTS", NULL, NULL);
        if (p_suite == NULL) {
                CU_cleanup_registry();
                return CU_get_error();
        }

        if (CU_add_test(p_suite, "test_ctor", test_list_ctor) == NULL ||
            CU_add_test(p_suite, "test_ctor_list", test_list_ctor_list) == NULL ||
            CU_add_test(p_suite, "test_push_back", test_list_push_back) == NULL ||
            CU_add_test(p_suite, "test_push_front", test_list_push_front) == NULL ||
            CU_add_test(p_suite, "test_at", test_list_at) == NULL ||
            CU_add_test(p_suite, "test_front", test_list_front) == NULL ||
            CU_add_test(p_suite, "test_back", test_list_back) == NULL ||
            CU_add_test(p_suite, "test_pop_back", test_list_pop_back) == NULL ||
            CU_add_test(p_suite, "test_pop_front", test_list_pop_front) == NULL ||
            CU_add_test(p_suite, "test_swap", test_list_swap) == NULL ||
            CU_add_test(p_suite, "test_insert", test_list_insert) == NULL ||
            CU_add_test(p_suite, "test_erase", test_list_erase) == NULL ||
            CU_add_test(p_suite, "test_clear", test_list_clear) == NULL) {
                CU_cleanup_registry();
                return CU_get_error();
        }

        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();

        return CU_get_error();
}
