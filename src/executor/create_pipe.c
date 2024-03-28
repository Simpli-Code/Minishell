/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:25:52 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:23:07 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child(t_mini *shell, int *flag, int *fds, int pos)
{
	int		i;
	int		argc;
	char	**argv;

	signal(SIGINT, handle_sig_child);
	signal(SIGQUIT, SIG_IGN);
	if (!fork())
	{
		if (!flag[0])
			dup2(fds[0], STDIN_FILENO);
		if (!flag[1])
			dup2(fds[3], STDOUT_FILENO);
		i = 0;
		while (i < 4)
			close(fds[i++]);
		argc = shell->argc;
		argv = shell->argv;
		set_pipe_args(shell, pos);
		execute_commands(shell);
		ft_free_2d_array(shell->argv);
		shell->argc = argc;
		shell->argv = argv;
		close(shell->fd_out);
		exit(shell->e_status);
	}
}

int	helper_pipes(t_mini *shell, int *fds, int j, int *flag)
{
	int	i;

	j = 0;
	while (shell->argv[j])
	{
		i = 0;
		while (shell->argv[j + i] && ft_memcmp(shell->argv[j + i], "|", 2))
			i++;
		if (!shell->argv[i + j])
			flag[1] = 1;
		else
			flag[1] = 0;
		handle_child(shell, flag, fds, j);
		shell->child++;
		flag[0] = 0;
		swap_pipes(fds);
		if (!shell->argv[j + i])
			j += i;
		else
			j += i + 1;
	}
	free(flag);
	return (shell->child);
}

int	execute_pipes(t_mini *shell, int *fds)
{
	int	*flag;
	int	j;

	j = 0;
	flag = (int *)malloc(sizeof(int) * 2);
	if (!flag)
		return (0);
	flag[0] = 1;
	flag[1] = 0;
	return (helper_pipes(shell, fds, j, flag));
}
