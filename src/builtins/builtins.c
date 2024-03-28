/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 20:02:18 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:21:13 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit(t_mini *shell)
{
	int	i;

	i = 0;
	while (shell->argc > 1 && ft_isdigit(shell->argv[1][i]))
	{
		i++;
	}
	if (shell->argc > 1 && shell->argv[1][i])
	{
		ft_putstrs_fd("exit\nbash: exit: ", shell->argv[1],
			": numeric argument required\n", 2);
		shell->e_status = 2;
	}
	else
	{
		ft_putstr_fd("exit\n", 2);
	}
	if (shell->argc > 1 && shell->e_status != 2)
		i = ft_atoi(shell->argv[1]);
	else
		i = shell->e_status;
	free_shell(shell);
	exit(i);
}


static void	handle_env(t_mini *shell, int fd)
{
	int	i;

	i = 0;
	if (shell->argc != 1)
	{
		ft_putstrs_fd("env: ‘", shell->argv[1],
			"’: No such file or directory\n", 2);
		shell->e_status = 126;
		return ;
	}
	while (shell->envp[i])
		ft_putstrs_fd(shell->envp[i++], "\n", 0, fd);
}

static int	builtins_helper(t_mini *shell, int fd)
{
	char	*path;
	char	cwd[BUF_SIZE];

	path = NULL;
	if (!ft_memcmp(shell->argv[0], "echo", 5))
		handle_echo(shell, fd);
	else if (!ft_memcmp(shell->argv[0], "pwd", 4))
		ft_putstrs_fd(getcwd(cwd, BUF_SIZE), "\n", 0, fd);
	else if (!ft_memcmp(shell->argv[0], "cd", 3))
	{
		path = ft_strdup(shell->argv[0]);
		if (!path)
		{
			free(path);
			return (1);
		}
		handle_cd(shell);
		if (path && !ft_strncmp(path, "-", 2))
			ft_putstrs_fd(getcwd(cwd, BUF_SIZE), "\n", 0, fd);
		free(path);
	}
	else
		return (FAILURE);
	return (SUCCESS);
}

int	is_builtins(t_mini *shell, int fd)
{
	shell->e_status = 0;
	if (!builtins_helper(shell, fd))
		return (shell->e_status);
	if (!ft_memcmp(shell->argv[0], "env", 4))
		handle_env(shell, fd);
	else if (!ft_memcmp(shell->argv[0], "./", 2) || !ft_memcmp(shell->argv[0],
			"../", 3) || !ft_memcmp(shell->argv[0], "/", 1))
		return (handle_cmd_w_fork(shell));
	else if (!ft_memcmp(shell->argv[0], "export", 7)
		|| !ft_memcmp(shell->argv[0], "unset", 6))
		shell->envp = export_unset(shell, fd);
	else if (!ft_memcmp(shell->argv[0], "exit", 5))
		handle_exit(shell);
	else
		return (127);
	return (shell->e_status);
}
