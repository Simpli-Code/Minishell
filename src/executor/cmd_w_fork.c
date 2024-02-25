/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_w_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:34:47 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/04 09:55:31 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
to execute a command with fork, wait for the child process to complete,
and then set the exit status

Store the original argv[0] for later restoration
adjust the pointer from the cmd if "./" exists
Get the current working directory
Set the path in the argv[0] if needed
Create a child process
handle signals
if execve fails, handle the cmd path
Exit the child process with exit status

Parent process waits for the child to finish
and collect it's exit status
to get the actual exit status /= 256
Free allocated memory
Restore the original argv[0]
*/
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
