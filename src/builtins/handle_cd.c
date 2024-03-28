/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 09:58:36 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:21:26 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_oldpwd_export(t_mini *shell, char *oldpwd)
{
	shell->argc = 4;
	ft_free_2d_array(shell->argv);
	shell->argv = (char **)ft_calloc(sizeof(char *), 4);
	shell->argv[0] = ft_strdup("export");
	if (!shell->argv[0])
		return ;
	shell->argv[1] = ft_strdup("OLDPWD=");
	if (!shell->argv[1])
		return ;
	shell->argv[2] = ft_strdup(oldpwd);
	if (!shell->argv[2])
		return ;
	shell->envp = export_cmd(shell, 1);
	ft_free_2d_array(shell->argv);
}

static void	set_pwd_export(t_mini *shell, char *cwd)
{
	shell->argv = (char **)ft_calloc(sizeof(char *), 4);
	shell->argv[0] = ft_strdup("export");
	if (!shell->argv[0])
		return ;
	shell->argv[1] = ft_strdup("PWD=");
	if (!shell->argv[1])
		return ;
	shell->argv[2] = ft_strdup(getcwd(cwd, BUF_SIZE));
	if (!shell->argv[2])
		return ;
	shell->envp = export_cmd(shell, 1);
}

static void	chang_dir(t_mini *shell, char *path)
{
	char	cwd[BUF_SIZE];
	char	oldpwd[BUF_SIZE];

	getcwd(oldpwd, BUF_SIZE);
	if (chdir(path) == 0)
	{
		set_oldpwd_export(shell, oldpwd);
		set_pwd_export(shell, cwd);
	}
	else
	{
		ft_putstrs_fd("bash: cd: ", shell->argv[1], ": ", 2);
	}
}

void	handle_cd(t_mini *shell)
{
	char	*path;

	path = NULL;
	errno = 0;
	if (shell->argc <= 2)
	{
		if (!shell->argv[1] || !ft_memcmp(shell->argv[1], "--", 3)
			|| !ft_memcmp(shell->argv[1], "~", 2))
			path = get_env_path(shell->envp, "HOME");
		else if (!ft_memcmp(shell->argv[1], "-", 2))
			path = get_env_path(shell->envp, "OLDPWD");
		else
			path = shell->argv[1];
		chang_dir(shell, path);
		if (errno > 0)
		{
			ft_putstrs_fd(strerror(errno), "\n", 0, 2);
			shell->e_status = 1;
		}
	}
	else
	{
		ft_putstrs_fd(PMT1, shell->argv[0], ": too many arguments\n", 2);
		shell->e_status = 1;
	}
}
