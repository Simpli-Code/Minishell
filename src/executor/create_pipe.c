/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:25:52 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 17:13:03 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
executing the child process in a pipeline
handle SIGINT for the child process
fork a new process for the curr cmd,
Set up input (0) and output (STDOUT_FILENO) redirection
if the cmd is not the first or last in the pipeline. close all file
descriptors in the original array fds to avoid resource leaks.
argc = shell->argc;
argv = shell->argv;
save the original values of argc and argv
set_pipe_args(shell, pos);
set the cmd arguments for the cmd within the pipeline.
execute the cmd within the pipeline using the execute_commands
free the memory allocated for the modified arguments,
restores the original argc and argv values
exit the child process with the exit code stored in shell->e_status.
*/
static void	handle_child(t_mini *shell, int *flag, int *fds, int pos)
{
	int		i;
	int		argc;
	char	**argv;

	signal(SIGINT, handle_sig_child);
	signal(SIGQUIT, SIG_IGN);
	if (!fork())
	{
		if (!flag[0])
			dup2(fds[0], STDIN_FILENO);
		if (!flag[1])
			dup2(fds[3], STDOUT_FILENO);
		i = 0;
		while (i < 4)
			close(fds[i++]);
		argc = shell->argc;
		argv = shell->argv;
		set_pipe_args(shell, pos);
		execute_commands(shell);
		ft_free_2d_array(shell->argv);
		shell->argc = argc;
		shell->argv = argv;
		close(shell->fd_out);
		exit(shell->e_status);
	}
}

/*
check the execution of cmds within a pipeline.

child_pid;	to keep track of the number of child PID
int *flag;	to count the nbr of cmds in pipeline, if it's the cmd in pipeline

flag[0] = 1;	the first flag, the first cmd is the last one in the pipeline
flag[1] = 0;	the 2. flag is initialized to 0,
iterates through the cmds
nested loop to find the end of curr cmd until a pipe (|) or end of args

sets flag[1] to 1 if there are no more arguments after the current set
(i.e.,at the end of the pipeline), else it's 0

execute child processes for curr cmd within pipeline,
and increments the count of child processes
update flag[0] and swap the file descriptors for the next set of cmds
in the pipeline using the swap_pipes

move the index j to the next set of arguments in the pipeline.
free the memory allocated for the flag array
return the number of child processes created
*/
int	helper_pipes(t_mini *shell, int *fds, int j, int *flag)
{
	int	i;

	j = 0;
	while (shell->argv[j])
	{
		i = 0;
		while (shell->argv[j + i] && ft_memcmp(shell->argv[j + i], "|", 2))
			i++;
		if (!shell->argv[i + j])
			flag[1] = 1;
		else
			flag[1] = 0;
		handle_child(shell, flag, fds, j);
		shell->child++;
		flag[0] = 0;
		swap_pipes(fds);
		if (!shell->argv[j + i])
			j += i;
		else
			j += i + 1;
	}
	free(flag);
	return (shell->child);
}

int	execute_pipes(t_mini *shell, int *fds)
{
	int	*flag;
	int	j;

	j = 0;
	flag = (int *)malloc(sizeof(int) * 2);
	if (!flag)
		return (0);
	flag[0] = 1;
	flag[1] = 0;
	return (helper_pipes(shell, fds, j, flag));
}
