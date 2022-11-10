/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 19:21:29 by gasouza           #+#    #+#             */
/*   Updated: 2022/11/10 06:52:23 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexical.h"

t_token	*token_create(char *value, t_tokentype type)
{
	t_token	*token;

	token = (t_token *) malloc(sizeof(t_token));
	if (token)
	{
		if (!value)
			token->value = ft_strdup("");
		if (value)
			token->value = value;
		token->type = type;
	}
	return (token);
}
