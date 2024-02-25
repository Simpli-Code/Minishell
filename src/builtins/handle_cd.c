/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 09:58:36 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/01 21:19:05 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
The set_oldpwd_export and set_pwd_export are respponsible for
setting the environment variable OLDPWD and PWd for exporting
*	argc is set to 4 indicating 4 arguments
*	argv is freed by calling free matrix
*	new argv is allocated with size 4 char *
*	argv[0] is set to "export"
*	argv[1] is set to "OLDPWD="
*	argv[2] is set to oldpwd
*	the envp is updated by calling export_cmd
the shell->argv array is freed again
a new shell is allocated with size 4 char *
*/
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

/*
changes the current directory,
updates the environment variables OLDPWD and PWD,
and handles error msg.

getcwd(oldpwd, BUF_SIZE)
To store the current working directory to the old working directory.
if	chdir(path) returns 0 (change dir to the path) indicating success
if changing dir is successful go inside if statement and
updates the environment variables OLDPWD and PWD
else if changing dir fails chdir is non zero
an error message
*/
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

/*
handling the "cd" (change directory) cmd
if argc is <= 2 proceeds with the directory change.
If there are more than two arguments, it prints an error message
if two or fewer arguments:
check if argv[1] is NULL starts with "--" or "~", or is "-"
Depending on these conditions,
it sets the path variable to the home directory ("HOME" environment variable),
the previous directory ("OLDPWD" environment variable)
chang_dir function with the calculated path to change the current directory.
If errno is greater than 0,
it means that an error occurred during the directory change.
In this case, prints an error message
*/
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
