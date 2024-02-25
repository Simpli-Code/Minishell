/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_pipe_or_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:44:47 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 16:40:53 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
for setting up and switching between pipes in a series of cmds
close the read (fds[0]) and write (fds[1]) ends of the current pipe.
update the read (fds[0]) and write (fds[1]) ends of the pipe to be the same as
the new pipe's read (fds[2]) and write (fds[3]) ends,

creates a new pipe and updates the array fds to store the read
and write ends of the new pipe at indices 2 and 3,
*/
void	swap_pipes(int *fds)
{
	close(fds[0]);
	close(fds[1]);
	fds[0] = fds[2];
	fds[1] = fds[3];
	pipe(fds + 2);
}

/*
extracts the arguments of a cmd form the pipeline
iterate through the args starting from the specified index i until
the end of the args or a pipe (|) allocate memory for the array of strings
(after) to store the arguments within the pipeline. copie the args from the
original array shell->argv to the newly allocated array after.
shell->argv = after;
assign the new array after to shell->argv
shell->argc = j;
set shell->argc to the number of args within the pipeline
*/
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

/*
check whether to execute cmd or pipe
fds[4];		array of fds for creating pipes
duplicate the STDIN and stores it in std_out.
to restore the standard input after command or pipeline execution.
loop through argv until you find a pipe (|)
if (!shell->argv[i]) -> if no pipe found, it means there is a single command,
run Execute Command function and update the envp variables.
else if a pipe and there are commands in the pipeline:
	create two pipes (fds and fds + 2) to set up the pipeline.
	check if creating pipes failed
	execute_pipes and get the PID of the last child
	while (child-- > 0)
	wait(&shell->e_status);
	wait for all child processes to finish and get their exit status.
	shell->e_status /= 256;
	divide the exit status by 256 to get the actual exit code.
	close all file descriptors.
dup2(std_out, 0);
restore the std_in by duplicating the original std_in fd stored in std_out.
*/
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
