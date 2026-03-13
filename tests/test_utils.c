#include "unity/unity.h"
#include "../includes/minishell.h"

t_minishell *g_minishell;

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_ft_strcmp(void) {
    TEST_ASSERT_EQUAL_INT(1, ft_strcmp("hello", "hello"));
    TEST_ASSERT_EQUAL_INT(0, ft_strcmp("hello", "world"));
    TEST_ASSERT_EQUAL_INT(0, ft_strcmp("hello", ""));
    TEST_ASSERT_EQUAL_INT(0, ft_strcmp("", "hello"));
    TEST_ASSERT_EQUAL_INT(1, ft_strcmp("", ""));
}

void test_array_size(void) {
    char *arr1[] = {"one", "two", "three", NULL};
    char *arr2[] = {NULL};
    char *arr3[] = {"only", NULL};

    TEST_ASSERT_EQUAL_INT(3, array_size(arr1));
    TEST_ASSERT_EQUAL_INT(0, array_size(arr2));
    TEST_ASSERT_EQUAL_INT(1, array_size(arr3));
    TEST_ASSERT_EQUAL_INT(0, array_size(NULL));
}

void test_array_dup(void) {
    char *arr[] = {"one", "two", "three", NULL};
    char **dup;

    dup = array_dup(arr);
    TEST_ASSERT_NOT_NULL(dup);
    TEST_ASSERT_EQUAL_INT(3, array_size(dup));
    TEST_ASSERT_EQUAL_STRING("one", dup[0]);
    TEST_ASSERT_EQUAL_STRING("two", dup[1]);
    TEST_ASSERT_EQUAL_STRING("three", dup[2]);
    TEST_ASSERT_NULL(dup[3]);
    
    array_destroy(dup);

    dup = array_dup(NULL);
    TEST_ASSERT_NULL(dup);
}

void test_array_append(void) {
    char **arr = NULL;
    
    array_append(&arr, ft_strdup("one"));
    TEST_ASSERT_NOT_NULL(arr);
    TEST_ASSERT_EQUAL_INT(1, array_size(arr));
    TEST_ASSERT_EQUAL_STRING("one", arr[0]);
    TEST_ASSERT_NULL(arr[1]);

    array_append(&arr, ft_strdup("two"));
    TEST_ASSERT_EQUAL_INT(2, array_size(arr));
    TEST_ASSERT_EQUAL_STRING("one", arr[0]);
    TEST_ASSERT_EQUAL_STRING("two", arr[1]);
    TEST_ASSERT_NULL(arr[2]);

    array_destroy(arr);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ft_strcmp);
    RUN_TEST(test_array_size);
    RUN_TEST(test_array_dup);
    RUN_TEST(test_array_append);
    return UNITY_END();
}
