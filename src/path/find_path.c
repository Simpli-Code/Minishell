/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:13:00 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:25:06 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
