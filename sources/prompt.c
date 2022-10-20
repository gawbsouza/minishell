/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acesar-l <acesar-l@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 19:26:52 by acesar-l          #+#    #+#             */
/*   Updated: 2022/10/19 06:00:45 by acesar-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prompt(void);
char	*current_path(void);

void	prompt(void)
{
	char	*prompt;

	prompt = readline(current_path());
	if (there_is_a_valid_char(prompt, "\0") == false)
		return	;
	if (are_the_quotation_marks_closed(prompt) == false)
	{
		error("syntax error: unclosed quotation marks", 2);
		return	;
	}
	add_history(prompt);
	if (there_is_a_valid_char(prompt, "#") == false)
		return	;
	if (is_syntax_valid(prompt) == false)
		error("syntax error or syntax not suported", 2);
	if (is_prompt_valid(prompt) == false)
		exit(EXIT_FAILURE);
	if (ft_strnstr(prompt, "cd", ft_strlen(prompt)))
		cd(&prompt[3]);
	if (ft_strnstr(prompt, "echo", ft_strlen(prompt)))
		ft_printf("%s\n", &prompt[5]);
	if (ft_strnstr(prompt, "pwd", ft_strlen(prompt)))
		pwd(STDOUT_FILENO);
}

char	*current_path(void)
{
	char	*path;

	path = ft_calloc(sizeof(char), PATH_MAX);
	if (getcwd(path, PATH_MAX) == NULL)
		return (NULL);
	else
	{
		ft_printf(PURPLE);
		path = ft_strappend(&path, ">");
		path = ft_strappend(&path, " ");
		path = ft_strappend(&path, RESET);
		return (path);
	}
}
