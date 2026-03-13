#include "unity/unity.h"
#include "../includes/minishell.h"

t_minishell *g_minishell;

void setUp(void) {}
void tearDown(void) {}

void test_is_a_builtin(void) {
    TEST_ASSERT_TRUE(is_a_builtin("echo"));
    TEST_ASSERT_TRUE(is_a_builtin("cd"));
    TEST_ASSERT_TRUE(is_a_builtin("pwd"));
    TEST_ASSERT_TRUE(is_a_builtin("export"));
    TEST_ASSERT_TRUE(is_a_builtin("unset"));
    TEST_ASSERT_TRUE(is_a_builtin("env"));
    TEST_ASSERT_TRUE(is_a_builtin("exit"));
    TEST_ASSERT_FALSE(is_a_builtin("ls"));
    TEST_ASSERT_FALSE(is_a_builtin("grep"));
    TEST_ASSERT_FALSE(is_a_builtin(NULL));
}

void test_echo(void) {
    char *args[] = {"hello", "world", NULL};
    TEST_ASSERT_EQUAL_INT(0, echo(args));

    char *args_n[] = {"-n", "no", "newline", NULL};
    TEST_ASSERT_EQUAL_INT(0, echo(args_n));
}

void test_pwd(void) {
    TEST_ASSERT_EQUAL_INT(0, pwd());
}

void test_export_unset(void) {
    char *export_args[] = {"export", "NEW_VAR=value", NULL};
    char **current_env;
    char *initial_env[] = {"EXISTING=stay", NULL};
    char *val;

    current_env = array_dup(initial_env);

    // Test export
    export(export_args, &current_env);
    val = get_env_value("NEW_VAR", current_env);
    TEST_ASSERT_EQUAL_STRING("value", val);
    free(val);

    // Test unset
    // In this project, unset is: int unset(const char *name, char ***envp);
    unset("NEW_VAR", &current_env);
    val = get_env_value("NEW_VAR", current_env);
    TEST_ASSERT_NULL(val);

    array_destroy(current_env);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_is_a_builtin);
    RUN_TEST(test_echo);
    RUN_TEST(test_pwd);
    RUN_TEST(test_export_unset);
    return UNITY_END();
}
