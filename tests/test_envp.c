#include "unity/unity.h"
#include "../includes/minishell.h"

t_minishell *g_minishell;

void setUp(void) {}
void tearDown(void) {}

void test_get_var_name(void) {
    char *name;

    name = get_var_name("VAR=VALUE");
    TEST_ASSERT_EQUAL_STRING("VAR", name);
    free(name);

    name = get_var_name("PATH=/usr/bin");
    TEST_ASSERT_EQUAL_STRING("PATH", name);
    free(name);

    name = get_var_name("EMPTY=");
    TEST_ASSERT_EQUAL_STRING("EMPTY", name);
    free(name);

    name = get_var_name("NO_EQUAL");
    TEST_ASSERT_NULL(name);
}

void test_get_var_value(void) {
    char *value;

    value = get_var_value("VAR=VALUE");
    TEST_ASSERT_EQUAL_STRING("VALUE", value);
    free(value);

    value = get_var_value("PATH=/usr/bin");
    TEST_ASSERT_EQUAL_STRING("/usr/bin", value);
    free(value);

    value = get_var_value("EMPTY=");
    TEST_ASSERT_EQUAL_STRING("", value);
    free(value);

    value = get_var_value("NO_EQUAL");
    TEST_ASSERT_NULL(value);
}

void test_expand_vars(void) {
    char *envp[] = {"VAR=value", "PATH=/usr/bin", NULL};
    char *result;

    result = expand_vars("$VAR", envp);
    TEST_ASSERT_EQUAL_STRING("value", result);
    free(result);

    result = expand_vars("echo $VAR", envp);
    TEST_ASSERT_EQUAL_STRING("echo value", result);
    free(result);

    result = expand_vars("'$VAR'", envp);
    TEST_ASSERT_EQUAL_STRING("'$VAR'", result);
    free(result);

    result = expand_vars("\"$VAR\"", envp);
    TEST_ASSERT_EQUAL_STRING("\"value\"", result);
    free(result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_var_name);
    RUN_TEST(test_get_var_value);
    RUN_TEST(test_expand_vars);
    return UNITY_END();
}
