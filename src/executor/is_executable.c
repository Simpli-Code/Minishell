/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_executable.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:15:40 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:23:24 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
