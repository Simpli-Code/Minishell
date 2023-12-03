/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 10:02:40 by chruhin           #+#    #+#             */
/*   Updated: 2023/12/03 10:29:59 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	token is an integer is either 1 or 0,
	1 indicating parent (main proccess) and 0 child (child proccess)
*/
void	setup_signals(int token)
{
	if (token == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (token == 1)
	{
		signal(SIGINT, handle_sig_child);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (token == -1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
	}
	else
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
	}
}

/*
	Child proccess handles SIGINT and SIGQUIT
*/
void	handle_sig_child(int signum)
{
	if (signum == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		g_var = 130;
	}
	else if (signum == SIGQUIT)
	{
		ft_putstr_fd("Quit\n", 2);
		rl_replace_line("", 0);
		rl_on_new_line();
		g_var = 131;
	}
}
