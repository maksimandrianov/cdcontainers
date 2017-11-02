#include "CUnit/Basic.h" 
#include "cdstructures.h"

void simpleTest(void) {

        struct cds_vector *v;
        CU_ASSERT(!cds_vector_ctor(v));
}

int main(int argc, char** argv) {

        CU_pSuite pSuite = NULL;

        /* initialize the CUnit test registry */
        if (CUE_SUCCESS != CU_initialize_registry())
                return CU_get_error();

        /* add a suite to the registry */
        pSuite = CU_add_suite("Suite_1", NULL, NULL);
        if (NULL == pSuite) {
                CU_cleanup_registry();
                return CU_get_error();
        }

        /* add the tests to the suite */
        if (NULL == CU_add_test(pSuite, "Simple Addition Test", simpleTest)) {
                CU_cleanup_registry();
                return CU_get_error();
        }

        /* Run all tests using the CUnit Basic interface */
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();
        return CU_get_error();
}
