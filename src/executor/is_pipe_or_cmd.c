/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_pipe_or_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:44:47 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:23:37 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap_pipes(int *fds)
{
	close(fds[0]);
	close(fds[1]);
	fds[0] = fds[2];
	fds[1] = fds[3];
	pipe(fds + 2);
}

void	set_pipe_args(t_mini *shell, int idx)
{
	char	**after;
	int		j;
	int		k;

	j = 0;
	while (shell->argv[idx + j] && ft_memcmp(shell->argv[idx + j], "|", 2))
		j++;
	after = (char **)ft_calloc(sizeof(char *), j + 1);
	if (!after)
		return ;
	k = -1;
	while (++k < j)
		after[k] = ft_strdup(shell->argv[idx + k]);
	shell->argv = after;
	shell->argc = j;
}

void	is_cmd_or_pipe(t_mini *shell)
{
	int	fds[4];
	int	i;

	i = 0;
	shell->fd_out = dup(STDIN_FILENO);
	while (shell->argv[i] && ft_memcmp(shell->argv[i], "|", 2))
		i++;
	if (!shell->argv[i])
		shell->envp = execute_commands(shell);
	else if (shell->argv[i])
	{
		if (pipe(fds) == -1 || pipe(fds + 2) == -1)
			return ;
		shell->child = execute_pipes(shell, fds);
		while (shell->child-- > 0)
			wait(&shell->e_status);
		shell->e_status /= 256;
		i = -1;
		while (++i < 4)
			close(fds[i]);
	}
	dup2(shell->fd_out, STDIN_FILENO);
	close(shell->fd_out);
}
