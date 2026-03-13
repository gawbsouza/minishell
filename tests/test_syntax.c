#include "unity/unity.h"
#include "../includes/minishell.h"

t_minishell *g_minishell;

void setUp(void) {
    g_minishell = malloc(sizeof(t_minishell));
    g_minishell->last_exit_code = 0;
}

void tearDown(void) {
    free(g_minishell);
}

void test_tokenization(void) {
    char *res;

    res = tokenization("ls -l");
    TEST_ASSERT_EQUAL_STRING("ls -l", res);
    free(res);

    res = tokenization("echo \"quoted string\"");
    TEST_ASSERT_EQUAL_STRING("echo \"*************\"", res);
    free(res);

    res = tokenization("echo 'single quoted'");
    TEST_ASSERT_EQUAL_STRING("echo '*************'", res);
    free(res);

    res = tokenization("echo \"'quoted'\"");
    TEST_ASSERT_EQUAL_STRING("echo \"'******'\"", res);
    free(res);
}

void test_parse_pipe(void) {
    char **res;

    res = parse_pipe("ls -l | grep foo | wc -l");
    TEST_ASSERT_NOT_NULL(res);
    TEST_ASSERT_EQUAL_INT(3, array_size(res));
    TEST_ASSERT_EQUAL_STRING("ls -l ", res[0]);
    TEST_ASSERT_EQUAL_STRING(" grep foo ", res[1]);
    TEST_ASSERT_EQUAL_STRING(" wc -l", res[2]);
    array_destroy(res);

    res = parse_pipe("echo \"pipe | inside\" | grep pipe");
    TEST_ASSERT_NOT_NULL(res);
    TEST_ASSERT_EQUAL_INT(2, array_size(res));
    TEST_ASSERT_EQUAL_STRING("echo \"pipe | inside\" ", res[0]);
    TEST_ASSERT_EQUAL_STRING(" grep pipe", res[1]);
    array_destroy(res);
}

void test_syntatic_validations_valid(void) {
    TEST_ASSERT_TRUE(syntatic_validations("ls -l"));
    TEST_ASSERT_TRUE(syntatic_validations("cat < infile | grep foo > outfile"));
    TEST_ASSERT_TRUE(syntatic_validations("echo \"unclosed quotes handled separately\""));
}

void test_syntatic_validations_invalid(void) {
    // Note: syntatic_validations calls error() which prints to stdout.
    // In a real unit test we might want to capture stdout, but here we check return value.
    
    TEST_ASSERT_FALSE(syntatic_validations("ls | | grep"));
    TEST_ASSERT_EQUAL_INT(2, g_minishell->last_exit_code);

    TEST_ASSERT_FALSE(syntatic_validations("> > outfile"));
    TEST_ASSERT_FALSE(syntatic_validations("cat < < infile"));
    TEST_ASSERT_FALSE(syntatic_validations("ls |"));
    TEST_ASSERT_FALSE(syntatic_validations("| ls"));
    TEST_ASSERT_FALSE(syntatic_validations("echo \"unclosed quote"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_tokenization);
    RUN_TEST(test_parse_pipe);
    RUN_TEST(test_syntatic_validations_valid);
    RUN_TEST(test_syntatic_validations_invalid);
    return UNITY_END();
}
