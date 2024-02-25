/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:13:00 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 09:06:36 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
to get the value of env by looking through the array in envp variables
the result is a pointer to the value of env variable,
or 0 if no variable is found.

input env is modified by concatenating it with the equal sign ("=").
to prepares the string for comparison with the environment variables in envp.

calculate the len of the modified env string.
loop through the array in envp variables until an env variable is found
that starts with the modified env string
the comparison is performed up to the len of the modified env string

if a matching env variable is found (envp[i] is not NULL),
tmp is assigned a pointer to the value of the env variable,
excluding the part that matches the specified env.
if no matching env variable is found, tmp is set to 0.

free the allocated memo for the modified env
return a pointer to the value of env variable
*/
char	*get_env_path(char **envp, char *env)
{
	int		i;
	int		len;
	char	*tmp;

	env = ft_strjoin(env, "=");
	if (!env)
		return (NULL);
	len = strlen(env);
	i = 0;
	while (envp[i] && ft_memcmp(env, envp[i], len))
		i++;
	if (envp[i])
	{
		tmp = envp[i] + len;
	}
	else
	{
		tmp = NULL;
	}
	free(env);
	return (tmp);
}

/*
to split the "PATH" env variable into individual directory path_arr.
in the context of searching for an executable binary in multiple directories.

**path_arr to store the individual directory path_arr when splitting the "PATH"
get_env_path extract the value of the "PATH"
from the shell->envp array.if path is not NULL, if "PATH"  is set, if true,
split the "PATH" variable based on the colon (':') delimiter

else if 'path' is NULL, "PATH" is not set
prints an error msg to the stderr
set shell->e_status to 127 to indicate an error and returns NULL.
If the "PATH" variable is set,
return the array of directory path_arr obtained by splitting it
*/
static char	**get_path(t_mini *shell, char *argv_zero)
{
	char	*path;
	char	**path_arr;

	if ((*argv_zero == '.' || *argv_zero == '\0'))
	{
		shell->e_status = 127;
		return (NULL);
	}
	path = get_env_path(shell->envp, "PATH");
	if (path)
		path_arr = ft_split(path, ':');
	else
	{
		ft_putstrs_fd(PMT1, argv_zero, ": No such file or directory\n", 2);
		shell->e_status = 127;
		return (NULL);
	}
	return (path_arr);
}

/*
to searche for an executable binary in directories specified by the "PATH"
and return the full path of the first match found. If no match is found,
it return NULL.

**path_arr	to store each directory path_arr extracted by splitting the "PATH"
*path	to store the full path  of founded executable.
get_path will extract the directory path_arr from the "PATH"
if fail return NULL.
loop through the directory path_arr
using opendir to open the current directory in the loop and
store the directory stream pointer in *dir.
loop through the directory entries using readdir.
The loop continues as long as the directory stream is open ((*dir) is not NULL)
and there are no permission issues (errno != EACCES)
and there are more directory entries	((*d = readdir(*dir))).
ft_memcmp compares the current directory entry's name ((*d)->d_name) with the
target string argv_zero. "char	d_name[256]; Null-terminated filename"
If there is a match, it means the executable file is found.
ft_strjoin to concatenate the directory path with a '/' character
to create the full path of the executable.
free the memory allocated for the array of directory path_arr.
return the full path of the found executable
After the loop, close the directory stream of the last directory.
free the memory allocated for the array of directory path_arr if
no match is found. If no executable is found in any of
the directories, it returns NULL.
*/
struct dirent	*read_dir(DIR **dir, struct dirent **d)
{
	errno = 0;
	*d = readdir(*dir);
	if (errno != 0)
		return (NULL);
	return (*d);
}

char	*filter_path( t_mini *shell, char *str, DIR **dir, struct dirent **d)
{
	char	**path_arr;
	char	*path;
	int		i;

	i = 0;
	path_arr = get_path(shell, str);
	if (!path_arr)
		return (NULL);
	while (path_arr[++i])
	{
		*dir = opendir(path_arr[i]);
		while ((*dir) && errno != EACCES && (read_dir(dir, d)))
		{
			if (!ft_memcmp(str, (*d)->d_name, ft_strlen(str) + 1))
			{
				path = ft_strjoin(path_arr[i], "/");
				ft_free_2d_array(path_arr);
				return (path);
			}
		}
		closedir(*dir);
	}
	ft_free_2d_array(path_arr);
	return (NULL);
}
