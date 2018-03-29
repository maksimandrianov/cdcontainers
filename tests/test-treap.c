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
#include "cdcontainers/treap.h"
#include "cdcontainers/casts.h"

static struct cdc_pair a = {CDC_INT_TO_PTR(0), CDC_INT_TO_PTR(0)};
static struct cdc_pair b = {CDC_INT_TO_PTR(1), CDC_INT_TO_PTR(1)};
static struct cdc_pair c = {CDC_INT_TO_PTR(2), CDC_INT_TO_PTR(2)};
static struct cdc_pair d = {CDC_INT_TO_PTR(3), CDC_INT_TO_PTR(3)};
static struct cdc_pair e = {CDC_INT_TO_PTR(4), CDC_INT_TO_PTR(4)};
static struct cdc_pair f = {CDC_INT_TO_PTR(5), CDC_INT_TO_PTR(5)};
static struct cdc_pair g = {CDC_INT_TO_PTR(6), CDC_INT_TO_PTR(6)};
static struct cdc_pair h = {CDC_INT_TO_PTR(7), CDC_INT_TO_PTR(7)};

static int lt_int(const void *l, const void *r)
{
        return l < r;
}

static bool treap_key_int_eq(struct cdc_treap *t, size_t count, ...)
{
        va_list args;
        int i;
        struct cdc_pair *val;
        void *tmp;

        va_start(args, count);
        for (i = 0; i < count; ++i) {
                val = va_arg(args, struct cdc_pair *);
                if (cdc_treap_get(t, val->first, &tmp) != CDC_STATUS_OK ||
                    tmp != val->second) {
                        va_end(args);
                        return false;
                }
        }

        va_end(args);
        return true;
}

static inline void treap_inorder_print_int(struct cdc_treap_node *node)
{
        if (node->left)
                treap_inorder_print_int(node->left);

        printf("%d ", CDC_PTR_TO_INT(node->key));

        if (node->right)
                treap_inorder_print_int(node->right);
}

void test_treap_ctor()
{
        struct cdc_treap *t;

        CU_ASSERT(cdc_treap_ctor(&t, NULL, lt_int, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_treap_size(t) == 0);

        cdc_treap_dtor(t);
}

void test_treap_ctorl()
{
        struct cdc_treap *t;

        CU_ASSERT(cdc_treap_ctorl(&t, NULL, lt_int, NULL, &a, &g, &h, &d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_treap_size(t) == 4);
        CU_ASSERT(treap_key_int_eq(t, 4, &a, &g, &h, &d));

        cdc_treap_dtor(t);
}

void test_treap_insert()
{
        struct cdc_treap *t;

        CU_ASSERT(cdc_treap_ctor(&t, NULL, lt_int, NULL) == CDC_STATUS_OK);

        CU_ASSERT(cdc_treap_insert(t, e.first, e.second, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_treap_insert(t, b.first, b.second, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_treap_insert(t, f.first, f.second, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_treap_insert(t, d.first, d.second, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_treap_insert(t, c.first, c.second, NULL) == CDC_STATUS_OK);

        CU_ASSERT(treap_key_int_eq(t, 5, &e, &b, &f, &d, &c));

        //        CU_ASSERT(cdc_treap_erase(t, val2.first) == 1);
        //        CU_ASSERT(cdc_treap_erase(t, val1.first) == 1);

        cdc_treap_dtor(t);
}

void test_treap_iterators()
{
        struct cdc_treap *t;
        struct cdc_treap_iter it1, it2;
        struct cdc_pair *arr[] = {&a, &b, &c, &d, &e, &f, &g, &h};
        bool check;
        int i;

        CU_ASSERT(cdc_treap_ctorl(&t, NULL, lt_int, NULL,
                                       &a, &b, &c, &d, &e, &f, &g, &h, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_treap_size(t) == 8);

        check = true;
        i = 0;
        cdc_treap_begin(t, &it1);
        cdc_treap_end(t, &it2);
        for ( ; !cdc_treap_iter_is_eq(&it1, &it2); cdc_treap_iter_next(&it1)) {
                if (cdc_treap_iter_key(&it1) != arr[i]->first) {
                        check = false;
                        break;
                }
                ++i;
        }
        CU_ASSERT(check == true);
        CU_ASSERT(i == cdc_treap_size(t));

        check = true;
        i = cdc_treap_size(t) - 1;
        cdc_treap_end(t, &it1);
        cdc_treap_iter_prev(&it1);
        cdc_treap_begin(t, &it2);
        for ( ; !cdc_treap_iter_is_eq(&it1, &it2); cdc_treap_iter_prev(&it1)) {
                if (cdc_treap_iter_key(&it1) != arr[i]->first) {
                        check = false;
                        break;
                }
                --i;
        }
        CU_ASSERT(check == true);
        CU_ASSERT(i == 0);

        check = true;
        i = 0;
        cdc_treap_begin(t, &it1);
        while (cdc_treap_iter_has_next(&it1)) {
                if (cdc_treap_iter_key(&it1) != arr[i]->first) {
                        check = false;
                        break;
                }
                ++i;
                cdc_treap_iter_next(&it1);
        }
        CU_ASSERT(check == true);
        CU_ASSERT(i == cdc_treap_size(t));

        check = true;
        i = cdc_treap_size(t) - 1;
        cdc_treap_end(t, &it1);
        cdc_treap_iter_prev(&it1);
        while (cdc_treap_iter_has_prev(&it1)) {
                if (cdc_treap_iter_key(&it1) != arr[i]->first) {
                        check = false;
                        break;
                }
                --i;
                cdc_treap_iter_prev(&it1);
        }
        CU_ASSERT(check == true);
        CU_ASSERT(i == 0);

        cdc_treap_dtor(t);
}

void test_treap_swap()
{

}
