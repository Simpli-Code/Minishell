/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:33:13 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/03 20:34:01 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
check the value of the errno variable and handle specific error conditions
errno-base.h
#define ENOENT		2	No such file or directory
#define EACCES		13	Permission denied

if errno is ENOENT or EACCES print error message
fd 2 stderr
set shell->e_status either to 127 or 126
127 for ENOENT
126 for EACCES
if it is not a directory sets e_status to 127
if it is a directory sets e_status to 126

else if errno is not ENOENT nor EACCES retrun (SUCCESS)
*/
static int	is_file_or_dir(t_mini *shell, char *str)
{
	if (errno == ENOENT || errno == EACCES)
	{
		ft_putstrs_fd(PMT1, str, ": ", 2);
		ft_putstrs_fd(strerror(errno), "\n", 0, 2);
		if (errno == ENOENT)
		{
			shell->e_status = 127;
		}
		else
		{
			shell->e_status = 126;
		}
		return (FAILURE);
	}
	return (SUCCESS);
}

/*
handles different types of paths, check if a path points to a directory
or a file and set the exit status and print an error message

If an error is detected, return early
else check the path type, if not a dir
open the file and parse it's contents

If the path is indeed a directory, it prints an error message and sets
the return code (shell->e_status) to indicate that the path is a directory.
*/
void	handle_path_type(t_mini *shell, char *str, char *path)
{
	DIR	*dir;
	int	fd;

	if (is_file_or_dir(shell, str))
		return ;
	dir = opendir(path);
	if (!dir)
	{
		ft_putstrs_fd(PMT1, str, ": Is a directory\n", 2);
		shell->e_status = 126;
		return ;
	}
	fd = open(path, O_RDONLY, 0666);
	if (fd == -1)
	{
		return ;
	}
	shell->e_status = 126;
	ft_putstrs_fd(PMT1, str, ": Not a directory\n", 2);
	close(fd);
}

/*
to modify a path by navigating to a parent directory based on "../"
extract a filename from str and then adjust the path accordingly

*filename;			Extracted filename
*p_ath;				Temporary storage for parent directory
*path_result;		Modified path result
*/
static void	helper_parent(char **modified_path, char *filename, int path_len)
{
	char	*path_result;
	char	*path;

	path = ft_strldup(*modified_path, path_len);
	if (!path)
		return ;
	free(*modified_path);
	path_result = ft_strjoin(path, "/");
	if (!path_result)
		return ;
	free(path);
	path = ft_strjoin(path_result, filename);
	if (!path)
		return ;
	free(path_result);
	free(filename);
	*modified_path = path;
}

static void	modify_parentpath(int path_len, char **modified_path, char *str)
{
	int		index;
	char	*filename;

	index = 0;
	while (!ft_memcmp(str + index, "../", 3))
		index += 3;
	filename = ft_strdup(str + index);
	if (!filename)
		return ;
	index /= 3;
	while (index-- > 0)
	{
		while ((*modified_path)[path_len] != '/')
			path_len--;
		path_len--;
	}
	helper_parent(modified_path, filename, path_len);
}

/*
to modify a path based on the input. If input starts with "/",
it sets the path directly to input; otherwise, it modifies the parent path
using the modify_parentpath function.

check if input starts with a "/" meaning absolute path
if so, it replaces the path with the input.
If input doesn't start with a forward slash, meaning it'input not relative path
it calls a modify_parentpath to adjust the current path by combining it with
input.
to adjusts the newPath, and assigns the modified newPath to *path.
and frees the temporary newPath.
*/
void	modify_path(char *input, char **path)
{
	int		len;
	char	*newpath;

	newpath = ft_strdup(*path);
	if (!newpath)
		return ;
	len = ft_strlen(*path);
	if (!ft_memcmp(input, "/", 1))
	{
		*path = ft_strdup(input);
		if (!*path)
			return ;
	}
	else
	{
		modify_parentpath(len, &newpath, input);
		*path = newpath;
		return ;
	}
	free(newpath);
}
