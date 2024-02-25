/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 09:49:23 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/04 12:53:20 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
handle a cmd by setting up output redirection,
create a new array of cmd arguments,
check if the cmd is a built-in function or an external binary,
and printing an error message if the cmd is not found.

check if the first argument in the cmd (the cmd itself) is not NULL
and not an empty string.
call the init_fd function to set up
the file descriptor for output redirection.
filter_redirection func will create a new array of cmd arguments
without considering output redirection symbols.
is_builtins function will check if the cmd corresponds
to a built-in function. The result is stored in shell->e_status.
checks if the return value is 127
(indicating that the cmd is not a built-in function)
is_executable function will check if the cmd corresponds to an external binary.
If the return value is still 127, it means the cmd is not found.
print an error message
If the file descriptor is not the default standard output (1),
close the file descriptor.
return the pointer to the environment variables (shell->envp).
*/
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
