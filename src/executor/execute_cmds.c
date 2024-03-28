/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 09:49:23 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:23:12 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**execute_commands(t_mini *shell)
{
	int	fd;
	int	i;

	i = 0;
	if (shell->argv[i] && *(shell->argv[i]))
	{
		if (!is_heredoc_sign(shell))
			fd = open_fd_heredoc(shell);
		else
		{
			fd = init_fd(shell);
			filter_redirection(shell);
		}
		if (is_builtins(shell, fd) == 127 && is_executable(shell, fd) == 127)
		{
			ft_putstrs_fd(PMT1, shell->argv[i], ": command not found\n", 2);
			shell->e_status = 127;
		}
		if (fd != 1)
			close(fd);
	}
	return (shell->envp);
}

int	is_infile_sign(t_mini *shell)
{
	int	i;

	i = 0;
	while (shell->argv[i] && ft_memcmp(shell->argv[i], "<", 2))
	{
		i++;
	}
	if (shell->argv[i])
	{
		return (SUCCESS);
	}
	else
	{
		return (FAILURE);
	}
}
