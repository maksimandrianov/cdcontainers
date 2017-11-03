#include <CUnit/Basic.h>
#include "test_common.h"


int main(int argc, char** argv) {

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
            CU_add_test(p_suite, "testr_data", test_vector_data) == NULL ||
            CU_add_test(p_suite, "test_getters", test_vector_getters) == NULL
                        ) {
                CU_cleanup_registry();
                return CU_get_error();
        }

        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();
        return CU_get_error();
}
