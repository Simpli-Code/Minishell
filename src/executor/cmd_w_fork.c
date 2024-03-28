/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_w_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:34:47 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:22:57 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_cmd_w_fork(t_mini *shell)
{
	char	buff[BUF_SIZE];
	char	*path;
	char	*argv_zero;

	shell->e_status = 0;
	argv_zero = shell->argv[0];
	if (shell->argv[0][0] == '.' && shell->argv[0][1] == '/')
		shell->argv[0] += 2;
	path = getcwd(buff, BUF_SIZE);
	modify_path(shell->argv[0], &path);
	if (!fork())
	{
		signal(SIGINT, handle_sig_child);
		if (execve(path, shell->argv, shell->envp))
			handle_path_type(shell, argv_zero, path);
		exit(shell->e_status);
	}
	else
		wait(&shell->e_status);
	shell->e_status /= 256;
	free(path);
	shell->argv[0] = argv_zero;
	return (shell->e_status);
}
