/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acesar-l <acesar-l@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 19:17:28 by acesar-l          #+#    #+#             */
/*   Updated: 2022/12/09 20:01:59 by acesar-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "types.h"
# include "utils.h"
# include "defines.h"
# include "lexical.h"
# include <limits.h>
# include <signal.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

void		prompt(void);
t_bool		syntatic_validations(char *prompt);
int			error(char *message, int exit_code);
char		*tokenization(char *prompt);
char		*get_env_value(const char *name, char *const *envp);
char		**parse_pipe(char *prompt);
char		*expand_vars(const char *prompt, char *const *envp);
void		handle_signal(void);
char		*command_creator(char *prompt);
t_list		*cmd_create_list(char *const *cmds);
void		handle_signal(void);
char		*get_var_name(const char *string);
char		*get_var_value(const char *string);
char		**unset_env_value(const char *name, char *const *envp);
void		file_manager(t_file	*file);
char		**set_env_value(char *name, char *value, char *const *envp);
void		list_clear(t_list **list);

// Builtins
int			execute_builtin(t_cmd *cmd);
int			echo(char **arguments);
int			pwd(void);
int			cd(char **args, int argc);
int			env(char *const *envp);
int			unset(const char *name, char ***envp);
int			export(char *const *args, char ***envp);
void		minishell_exit(const char *exit_code);
t_bool		is_a_builtin(char *command);

//execution
void		init_files(void);
char		**get_cmd_paths(void);
void		execution_process(void);
void		delete_temporary_files(void);
void		save_last_exit_code(int wstatus);
void		execution_exit(void);
void		execution_clear(int *pipe_fd);

//global
void		init_global_struct(void);
void		clear_global(void);

//memory
int			clear_memory(void);
void		clear_history(void);
void		dead_minihell(void);
void		handle_sigquit(int signal);

//minishell
void		minishell_init_files(void);
t_minishell	*minishell_create(char **envp);

//signal
void		handle_signal_fork(void);

#endif
