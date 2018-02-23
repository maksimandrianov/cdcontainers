// The MIT License (MIT)
// Copyright (c) 2017 Maksim Andrianov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
#include "test-common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>

static int lt_int(const void *l, const void *r)
{
        return l < r;
}

static void inorder_print_int(struct cdc_treap_node *node) {
        if (node->left)
                inorder_print_int(node->left);

        printf("%p ", node->key);

        if (node->right)
                inorder_print_int(node->right);
}

void test_treap_ctor()
{
}

void test_treap_ctorl()
{

}

void test_treap_insert()
{
        struct cdc_treap *t;
        void *a = (void *)1, *b = (void *)2, *c = (void *)3,
                        *d = (void *)4, *e = (void *)5;

        CU_ASSERT(cdc_treap_ctor(&t, NULL, lt_int, NULL) == CDC_STATUS_OK);
        printf("\n");
        CU_ASSERT(cdc_treap_insert(t, d, NULL) == CDC_STATUS_OK);
        inorder_print_int(t->root);
        printf("\n");
        CU_ASSERT(cdc_treap_insert(t, a, NULL) == CDC_STATUS_OK);
        inorder_print_int(t->root);
        printf("\n");
        CU_ASSERT(cdc_treap_insert(t, e, NULL) == CDC_STATUS_OK);
        inorder_print_int(t->root);
        printf("\n");
        CU_ASSERT(cdc_treap_insert(t, c, NULL) == CDC_STATUS_OK);
        inorder_print_int(t->root);
        printf("\n");
        CU_ASSERT(cdc_treap_insert(t, b, NULL) == CDC_STATUS_OK);
        inorder_print_int(t->root);
        printf("\n");


        CU_ASSERT(cdc_treap_erase(t, b) == 1);
        inorder_print_int(t->root);
        printf("\n");

        CU_ASSERT(cdc_treap_erase(t, a) == 1);
        inorder_print_int(t->root);
        printf("\n");

}

void test_treap_swap()
{

}
