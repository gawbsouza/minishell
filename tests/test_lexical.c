#include "unity/unity.h"
#include "../includes/minishell.h"

t_minishell *g_minishell;

void setUp(void) {}
void tearDown(void) {}

void test_is_string_token(void) {
    t_token token;

    token.type = STRING_UNQUOTED;
    TEST_ASSERT_TRUE(is_string_token(&token));

    token.type = STRING_DQ_CLOSED;
    TEST_ASSERT_TRUE(is_string_token(&token));

    token.type = STRING_SQ_CLOSED;
    TEST_ASSERT_TRUE(is_string_token(&token));

    token.type = INFILE;
    TEST_ASSERT_FALSE(is_string_token(&token));

    token.type = PIPE;
    TEST_ASSERT_FALSE(is_string_token(&token));

    TEST_ASSERT_FALSE(is_string_token(NULL));
}

void test_is_infile_token(void) {
    t_token token;

    token.type = INFILE;
    TEST_ASSERT_TRUE(is_infile_token(&token));

    token.type = INFILE_HEREDOC;
    TEST_ASSERT_TRUE(is_infile_token(&token));

    token.type = OUTFILE;
    TEST_ASSERT_FALSE(is_infile_token(&token));

    TEST_ASSERT_FALSE(is_infile_token(NULL));
}

void test_is_outfile_token(void) {
    t_token token;

    token.type = OUTFILE;
    TEST_ASSERT_TRUE(is_outfile_token(&token));

    token.type = OUTFILE_APPEND;
    TEST_ASSERT_TRUE(is_outfile_token(&token));

    token.type = INFILE;
    TEST_ASSERT_FALSE(is_outfile_token(&token));

    TEST_ASSERT_FALSE(is_outfile_token(NULL));
}

void test_is_valid_expand_vars_token(void) {
    TEST_ASSERT_TRUE(is_valid_expand_vars_token(STRING_DQ_CLOSED));
    TEST_ASSERT_TRUE(is_valid_expand_vars_token(STRING_DQ_UNCLOSED));
    TEST_ASSERT_TRUE(is_valid_expand_vars_token(STRING_UNQUOTED));
    TEST_ASSERT_FALSE(is_valid_expand_vars_token(STRING_SQ_CLOSED));
    TEST_ASSERT_FALSE(is_valid_expand_vars_token(PIPE));
}

void test_token_create_destroy(void) {
    t_token *token;

    token = token_create("value", STRING_UNQUOTED);
    TEST_ASSERT_NOT_NULL(token);
    TEST_ASSERT_EQUAL_STRING("value", token->value);
    TEST_ASSERT_EQUAL_INT(STRING_UNQUOTED, token->type);
    token_destroy(token);

    token = token_create(NULL, PIPE);
    TEST_ASSERT_NOT_NULL(token);
    TEST_ASSERT_EQUAL_STRING("", token->value);
    TEST_ASSERT_EQUAL_INT(PIPE, token->type);
    token_destroy(token);
}

void test_token_to_str(void) {
    t_token *token;
    char *str;

    token = token_create("cmd", STRING_UNQUOTED);
    str = token_to_str(token);
    TEST_ASSERT_EQUAL_STRING("cmd", str);
    free(str); token_destroy(token);

    token = token_create("hello", STRING_DQ_CLOSED);
    str = token_to_str(token);
    TEST_ASSERT_EQUAL_STRING("\"hello\"", str);
    free(str); token_destroy(token);

    token = token_create("world", STRING_SQ_CLOSED);
    str = token_to_str(token);
    TEST_ASSERT_EQUAL_STRING("'world'", str);
    free(str); token_destroy(token);

    token = token_create(NULL, PIPE);
    str = token_to_str(token);
    TEST_ASSERT_EQUAL_STRING("|", str);
    free(str); token_destroy(token);

    token = token_create(NULL, INFILE_HEREDOC);
    str = token_to_str(token);
    TEST_ASSERT_EQUAL_STRING("<<", str);
    free(str); token_destroy(token);
}

void test_get_next_token(void) {
    char *input = "ls -l | grep \"foo bar\" > outfile";
    char *prompt = input;
    t_token *token;

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_STRING("ls", token->value);
    TEST_ASSERT_EQUAL_INT(STRING_UNQUOTED, token->type);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_STRING("-l", token->value);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_INT(PIPE, token->type);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_STRING("grep", token->value);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_STRING("foo bar", token->value);
    TEST_ASSERT_EQUAL_INT(STRING_DQ_CLOSED, token->type);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_INT(OUTFILE, token->type);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_STRING("outfile", token->value);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_NULL(token);
}
void test_get_next_token_complex(void) {
    char *input = "cat<<EOF|grep 'quoted \"nested\" string' >>outfile";
    char *prompt = input;
    t_token *token;

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_STRING("cat", token->value);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_INT(INFILE_HEREDOC, token->type);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_STRING("EOF", token->value);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_INT(PIPE, token->type);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_STRING("grep", token->value);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_STRING("quoted \"nested\" string", token->value);
    TEST_ASSERT_EQUAL_INT(STRING_SQ_CLOSED, token->type);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_INT(OUTFILE_APPEND, token->type);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_STRING("outfile", token->value);
    token_destroy(token);

    TEST_ASSERT_NULL(get_next_token(&prompt));
}

void test_get_next_token_edge_cases(void) {
    char *input = "   >file   ";
    char *prompt = input;
    t_token *token;

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_INT(OUTFILE, token->type);
    token_destroy(token);

    token = get_next_token(&prompt);
    TEST_ASSERT_EQUAL_STRING("file", token->value);
    token_destroy(token);

    TEST_ASSERT_NULL(get_next_token(&prompt));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_is_string_token);
    RUN_TEST(test_is_infile_token);
    RUN_TEST(test_is_outfile_token);
    RUN_TEST(test_is_valid_expand_vars_token);
    RUN_TEST(test_token_create_destroy);
    RUN_TEST(test_token_to_str);
    RUN_TEST(test_get_next_token);
    RUN_TEST(test_get_next_token_complex);
    RUN_TEST(test_get_next_token_edge_cases);
    return UNITY_END();
}
