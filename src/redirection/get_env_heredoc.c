/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 23:38:50 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/03 09:18:53 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
get environment variable by name.
the variable is looked up by index.
if not found, returns NULL.
if found, duplicates the string of the value and returns it
*/
char	*ft_getenv_heredoc(char *var, char **envp)
{
	int			var_index;
	const char	*env_var;
	const char	*value_start;

	if (var[0] == 0)
		return (ft_strdup("'$'"));
	var_index = find_env_var_index(var, envp);
	if (var_index == -1)
		return (NULL);
	env_var = envp[var_index];
	value_start = ft_strchr(env_var, '=');
	if (value_start)
	{
		value_start++;
		return (ft_strdup(value_start));
	}
	return (NULL);
}
