/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 20:02:18 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 22:34:37 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
handles the exit cmd by checking the number of arguments.
If there are too many arguments or the second argument is not a numeric value,
it prints an error message, it frees the allocated memory and exits
the program with the specified exit code.
checks if there are more than two arguments
if true, it prints an error message
else if there are two or fewer arguments:
set i to the integer value of the second argument using ft_atoi
if it is present and does not contain non-digit characters.
Otherwise, it set i to shell->e_status
free the memory allocated for the t_mini structure and its fields
exit the program with the exit code i.
*/
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

/*
To Handle the "env" cmd. If the cmd has no additional arguments
Check if the number of arguments is not equal to 1,
if there's more than one arg
Print an error message indicating permission denied and
set the return value to 126
If there's only one argument,
which is "env," enters a loop to iterate thorough the
environment variables
inside the loop
print each environment variables followed by newline
to the specified file descriptor
*/
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

/*
check if the given cmd is a built-in cmd,
execute the corresponding function.
If the cmd is not recognized as a built-in,
return 1 to indicate that it'str an external cmd.
handle built-in cmds for echoing, printing the current working directory,
and changing the directory.
*/
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

/*
checks various built-in cmds and call the functions or return an exit code.
if argv[0] is env, handle_env func
if the argv[0] starts with "./", "../", or "/",
indicating that it's a cmd to execute
a local file. If any of these conditions are true,
it calls the handle_cmd_w_fork
handles as well export, unset and exit
*/
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
