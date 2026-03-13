#include "unity/unity.h"
#include "../includes/minishell.h"

t_minishell *g_minishell;

void setUp(void) {}
void tearDown(void) {}

void test_cmd_create_destroy(void) {
    t_cmd *cmd;
    char *args[] = {"ls", "-l", NULL};

    cmd = cmd_create("ls", args, NULL, NULL);
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("ls", cmd->command);
    TEST_ASSERT_EQUAL_INT(2, array_size(cmd->args));
    TEST_ASSERT_EQUAL_STRING("ls", cmd->args[0]);
    TEST_ASSERT_EQUAL_STRING("-l", cmd->args[1]);
    TEST_ASSERT_NULL(cmd->infile);
    TEST_ASSERT_NULL(cmd->outfile);

    cmd_destroy(&cmd);
    TEST_ASSERT_NULL(cmd);
}

void test_cmd_parse_str_simple(void) {
    t_cmd *cmd;

    cmd = cmd_parse_str("ls -l -a");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("ls", cmd->command);
    TEST_ASSERT_EQUAL_INT(3, array_size(cmd->args));
    TEST_ASSERT_EQUAL_STRING("ls", cmd->args[0]);
    TEST_ASSERT_EQUAL_STRING("-l", cmd->args[1]);
    TEST_ASSERT_EQUAL_STRING("-a", cmd->args[2]);
    
    cmd_destroy(&cmd);
}

void test_cmd_parse_str_redirections(void) {
    t_cmd *cmd;

    cmd = cmd_parse_str("cat < infile > outfile");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("cat", cmd->command);
    TEST_ASSERT_NOT_NULL(cmd->infile);
    TEST_ASSERT_EQUAL_STRING("infile", cmd->infile->path);
    TEST_ASSERT_EQUAL_INT(COMMON_FILE_IN, cmd->infile->type);
    TEST_ASSERT_NOT_NULL(cmd->outfile);
    TEST_ASSERT_EQUAL_STRING("outfile", cmd->outfile->path);
    TEST_ASSERT_EQUAL_INT(COMMON_FILE_OUT, cmd->outfile->type);

    cmd_destroy(&cmd);
}

void test_cmd_parse_str_append_heredoc(void) {
    t_cmd *cmd;

    cmd = cmd_parse_str("grep pattern << LIMITER >> append_file");
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_STRING("grep", cmd->command);
    TEST_ASSERT_NOT_NULL(cmd->infile);
    TEST_ASSERT_EQUAL_STRING("LIMITER", cmd->infile->path);
    TEST_ASSERT_EQUAL_INT(HEREDOC_FILE, cmd->infile->type);
    TEST_ASSERT_NOT_NULL(cmd->outfile);
    TEST_ASSERT_EQUAL_STRING("append_file", cmd->outfile->path);
    TEST_ASSERT_EQUAL_INT(APPEND_FILE, cmd->outfile->type);

    cmd_destroy(&cmd);
}

void cmd_destroy_helper(void *content) {
    t_cmd *cmd = (t_cmd *)content;
    cmd_destroy(&cmd);
}

void test_cmd_create_list(void) {
    char *cmds[] = {"ls -l", "grep foo", "wc -l", NULL};
    t_list *list;
    t_cmd *cmd;

    list = cmd_create_list(cmds);
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL_INT(3, ft_lstsize(list));

    cmd = (t_cmd *)list->content;
    TEST_ASSERT_EQUAL_STRING("ls", cmd->command);

    cmd = (t_cmd *)list->next->content;
    TEST_ASSERT_EQUAL_STRING("grep", cmd->command);

    cmd = (t_cmd *)list->next->next->content;
    TEST_ASSERT_EQUAL_STRING("wc", cmd->command);

    ft_lstclear(&list, cmd_destroy_helper);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_cmd_create_destroy);
    RUN_TEST(test_cmd_parse_str_simple);
    RUN_TEST(test_cmd_parse_str_redirections);
    RUN_TEST(test_cmd_parse_str_append_heredoc);
    RUN_TEST(test_cmd_create_list);
    return UNITY_END();
}
