/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 19:42:16 by gasouza           #+#    #+#             */
/*   Updated: 2022/12/10 17:42:37 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_minishell	*g_minishell;

static void		handle_sigint(int signal);
void			handle_sigquit(int signal);
static void		handle_sigint_fork(int signal);

void	handle_signal(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_signal_fork(void)
{
	signal(SIGINT, handle_sigint_fork);
	signal(SIGQUIT, SIG_IGN);
}

static void	handle_sigint_fork(int signal)
{
	(void) signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	handle_sigint(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	printf(CYAN);
	rl_on_new_line();
	rl_redisplay();
}
