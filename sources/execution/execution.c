/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acesar-l <acesar-l@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 17:32:23 by acesar-l          #+#    #+#             */
/*   Updated: 2022/12/09 20:00:55 by acesar-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_minishell	*g_minishell;

static void	execute_single_cmd(t_cmd *command, int fd_pipe_in);
static int	execute_cmd_to_pipe(t_cmd *command, int fd_pipe_in);
static int	execute(t_cmd *command);
static void	command_not_found(char *command, t_list *list);

void	execution_process(void)
{
	t_cmd		*command;
	t_list		*node;
	int			fd_pipe_in;

	node = g_minishell->command_list;
	fd_pipe_in = STDIN_FILENO;
	while (node)
	{
		command = (t_cmd *) node->content;
		if (ft_strcmp("exit", command->command))
			minishell_exit(command->args[1]);
		if ((ft_strcmp("cd", command->command))
			|| (ft_strcmp("export", command->command))
			|| (ft_strcmp("unset", command->command)))
			execute_builtin(command);
		else if ((!node->next) || (command->outfile))
		{
			execute_single_cmd(command, fd_pipe_in);
		}
		else
			fd_pipe_in = execute_cmd_to_pipe(command, fd_pipe_in);
		node = node->next;
	}
}

static void	execute_single_cmd(t_cmd *command, int fd_pipe_in)
{
	int	pid;
	int	wstatus;

	pid = fork();
	handle_signal_fork();
	if (pid == 0)
	{
		clear_history();
		if (command->infile)
			dup2(command->infile->fd, STDIN_FILENO);
		else
			dup2(fd_pipe_in, STDIN_FILENO);
		if (command->outfile)
			dup2(command->outfile->fd, STDOUT_FILENO);
		if (execute(command) == -1)
			command_not_found(command->command, g_minishell->command_list);
		execution_exit();
	}
	waitpid(pid, &wstatus, 0);
	handle_signal();
	save_last_exit_code(wstatus);
}

static int	execute_cmd_to_pipe(t_cmd *command, int fd_pipe_in)
{
	int		fd_new_pipe[2];
	int		wstatus;
	int		pid;

	pipe(fd_new_pipe);
	pid = fork();
	handle_signal_fork();
	if (pid == 0)
	{
		execution_clear(&fd_new_pipe[INPUT]);
		handle_signal_fork();
		if (command->infile)
			dup2(command->infile->fd, STDIN_FILENO);
		else
			dup2(fd_pipe_in, STDIN_FILENO);
		dup2(fd_new_pipe[OUTPUT], STDOUT_FILENO);
		if (execute(command) == -1)
			command_not_found(command->command, g_minishell->command_list);
		execution_exit();
	}
	waitpid(pid, &wstatus, 0);
	handle_signal();
	save_last_exit_code(wstatus);
	close(fd_new_pipe[OUTPUT]);
	return (fd_new_pipe[INPUT]);
}

static int	execute(t_cmd *command)
{
	char	*cmd_and_path;
	char	**paths;
	int		i;

	i = 0;
	if ((ft_strcmp("echo", command->command))
		|| (ft_strcmp("pwd", command->command))
		|| ((ft_strcmp("env", command->command))))
	{
		execute_builtin(command);
		return (0);
	}
	if (ft_count_occurrences(command->command, '/'))
		execve(command->command, command->args, g_minishell->envp);
	paths = get_cmd_paths();
	while (paths && paths[i])
	{
		cmd_and_path = ft_strjoin(paths[i], command->command);
		execve(cmd_and_path, command->args, g_minishell->envp);
		free(cmd_and_path);
		i++;
	}
	array_destroy(paths);
	return (-1);
}

static void	command_not_found(char *command, t_list *list)
{
	t_cmd	*cmd;
	t_list	*node;
	int		exit_code;

	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf(GREY"minishell: %s : command not found\n"RESET, command);
	g_minishell->last_exit_code = COMMAND_NOT_FOUND;
	node = list;
	clear_global();
	delete_temporary_files();
	while (node)
	{
		cmd = (t_cmd *)node->content;
		cmd_destroy(&cmd);
		node = node->next;
	}
	array_destroy(g_minishell->envp);
	list_clear(&list);
	exit_code = g_minishell->last_exit_code;
	free(g_minishell);
	exit(exit_code);
}
