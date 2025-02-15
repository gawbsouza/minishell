/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 22:00:03 by acesar-l          #+#    #+#             */
/*   Updated: 2022/12/10 18:09:39 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

typedef enum e_bool
{
	false,
	true
}	t_bool;

typedef struct s_stack
{
	char			c;
	struct s_stack	*next;
}	t_stack;

typedef enum e_ftype {
	COMMON_FILE_IN,
	COMMON_FILE_OUT,
	APPEND_FILE,
	HEREDOC_FILE
}	t_ftype;

typedef struct s_file {
	char	*path;
	int		fd;
	t_ftype	type;
}	t_file;

typedef struct s_cmd
{
	char	*command;
	char	**args;
	t_file	*infile;
	t_file	*outfile;
}	t_cmd;

typedef struct s_minishell
{
	char	*current_path;
	char	**envp;
	int		last_exit_code;
	t_list	*command_list;
	t_file	*temp_infile;
	t_file	*temp_outfile;
}	t_minishell;

t_stack	*stack_new(char c);
void	stack_push(t_stack **stack, t_stack *new);
void	stack_pop(t_stack **stack);
void	clear_stack(t_stack **stack);

t_cmd	*cmd_create(const char *cmd, char *const *args, t_file *i, t_file *o);
void	cmd_destroy(t_cmd **cmd);
t_cmd	*cmd_parse_str(const char *str);
t_file	*file_create(const char *path, int fd, t_ftype type);
void	file_destroy(t_file **file);

#endif
