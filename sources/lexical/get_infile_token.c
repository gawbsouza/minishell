/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_infile_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 06:47:47 by gasouza           #+#    #+#             */
/*   Updated: 2022/11/10 06:48:05 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexical.h"

t_token	*get_infile_token(char *start, char **str)
{
	if (start && str && *str && *start == '<')
	{
		if (*(start + 1) == '<')
		{
			*str = start + 2;
			return (token_create(ft_strdup("<<"), INFILE_HEREDOC));
		}
		*str = start + 1;
		return (token_create(ft_strdup("<"), INFILE));
	}
	return (NULL);
}
