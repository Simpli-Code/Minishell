/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_executable.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:15:40 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 17:14:55 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
execute external binaries from the path,
handle input and output redirection,
manage error, including permission issues.
fork-exec-wait to run the external program in a separate
child process, and provides an exit status based on the success
or failure of the execution

make a copy of cmd arg'str
handle SIGINT and keyboard interrupts in the child proccess
create a child proccess to execute the external binary in the child proccess
in_file(); to handle redirections
check fd value to duplicate fd into stdout (handle output redirection)
execute the external binary located at path with the provided args and envp
if exec fails it returns a non zero,
and the error related to permission (error code 'EACCES')
it sets 'shell->e_status' to 126 indicating permissin issue.
print an error message to the stderr (file descriptor 2)
that includes the cmd name, the error msg (obtained from strerror(errno)),
and sets shell->e_status accordingly. exit(shell->e_status);
It exits the child process with the value of shell->e_status.

In the parent proccess wait for the child process to complete
and collect its exit status, which is stored in shell->e_status.
divide shell->e_status by 256. to convert the exit status to the format
typically used in shell scripts, an exit status of 0 indicates success.
*/
static void	execute_child_process(t_mini *shell, int fd, char *path,
		char **args)
{
	if (is_infile_sign(shell) == SUCCESS && handle_infile(shell) != SUCCESS)
		exit(1);
	if (fd > 1)
		dup2(fd, STDOUT_FILENO);
	if ((execve(path, args, shell->envp)) && errno == EACCES)
	{
		shell->e_status = 126;
		ft_putstrs_fd(PMT1, shell->argv[0], ": ", 2);
		ft_putstrs_fd(strerror(errno), "\n", 0, 2);
		exit(shell->e_status);
	}
	close(fd);
	exit(shell->e_status);
}

int	handle_binaries(t_mini *shell, int fd, char *path)
{
	char	**args;

	signal(SIGINT, handle_sig_child);
	signal(SIGQUIT, handle_sig_child);
	if (!is_heredoc_sign(shell))
		exec_heredocument(shell, fd, path);
	else
	{
		args = ft_cpy_argvs(shell);
		if (!args)
			return (-1);
		if (!fork())
		{
			execute_child_process(shell, fd, path, args);
		}
		wait(&shell->e_status);
		shell->e_status /= 256;
		free(path);
		ft_free_2d_array(args);
	}
	return (SUCCESS);
}

int	exec_heredocument(t_mini *shell, int fd, char *path)
{
	char	**args;

	args = (char *[]){shell->argv[0], TMP_FILE, NULL};
	if (!fork())
	{
		if ((execve(path, args, shell->envp)))
		{
			shell->e_status = 126;
			ft_putstrs_fd(PMT1, shell->argv[0], ": ", 2);
			ft_putstrs_fd(strerror(errno), "\n", 0, 2);
			return (FAILURE);
		}
		exit(shell->e_status);
	}
	wait(&shell->e_status);
	shell->e_status /= 256;
	free(path);
	unlink(TMP_FILE);
	close(fd);
	return (SUCCESS);
}

/*
to check for an executable binary file specified by shell->argv[0].
If the binary is found, execute it using the handle_binaries function.

*dir		a directory stream
*d			a directory entry structure
*pre_path	a prefix path
*path		the full path of the executable

initialize the return value shell->e_status to 127.
This value suggests that the binary file was not found.
call the filter_path function to find the executable file.
The filter_path function returns a prefix path (pre_path)
if the executable is found.
if pre_path is not NULL,
if the executable was found in one of the directories.
strjoin to concatenate the prefix path with the name of the executable
to create the full path of the executable (path).
call the handle_binaries function to execute the binary.
fd argument is used for handling input/output redirection.
close the directory stream.
free the memory allocated for the prefix path (pre_path),
regardless of whether the binary was found.
return the value of shell->e_status,
which is set by the handle_binaries function to indicate
the success or failure of executing the binary.
*/
int	is_executable(t_mini *shell, int fd)
{
	DIR				*dir;
	struct dirent	*d;
	char			*pre_path;
	char			*path;

	shell->e_status = 127;
	pre_path = filter_path(shell, shell->argv[0], &dir, &d);
	if (pre_path)
	{
		path = ft_strjoin(pre_path, d->d_name);
		if (!path)
			return (FAILURE);
		if (!handle_binaries(shell, fd, path))
			closedir(dir);
		else
			return (FAILURE);
	}
	free(pre_path);
	return (shell->e_status);
}
