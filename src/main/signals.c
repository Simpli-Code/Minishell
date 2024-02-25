/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 22:19:08 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 18:03:27 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
sets up signal handlers and ensuring proper handling
of signals and managing the exit status.
*/
void	setup_signals(t_mini *shell)
{
	signal(SIGINT, handle_sig_parent);
	signal(SIGQUIT, SIG_IGN);
	if (shell && shell->e_status < 0)
		shell->e_status = 0;
	if (shell && g_state >= 0)
	{
		shell->e_status = g_state;
		g_state = -1;
	}
}

/*
sets signals for the program (parent process)
*/
void	handle_sig_parent(int signum)
{
	if (signum == SIGINT)
	{
		write (1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_state = 130;
	}
}

/*
Child proccess handles SIGINT and SIGQUIT
*/
void	handle_sig_child(int signum)
{
	if (signum == SIGINT)
	{
		write (1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		g_state = 130;
	}
	else if (signum == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
		rl_replace_line("", 0);
		rl_on_new_line();
		g_state = 131;
	}
}

/*
sets up signal handlers for heredocument
*/
void	handle_sig_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		write (1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		g_state = 130;
	}
}
