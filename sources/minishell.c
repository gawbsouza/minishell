/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gnuncio- <gnuncio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 19:18:49 by acesar-l          #+#    #+#             */
/*   Updated: 2022/12/05 10:13:04 by gnuncio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	g_data;

int	main(int argc, char **argv, char **envp)
{
	if (argc != 1 || ft_strcmp(argv[0], "minishell"))
		exit(EXIT_FAILURE);
	g_data.env = array_dup(envp);
	handle_signal();
	while (true)
	{
		prompt(&g_data);
	}
}
