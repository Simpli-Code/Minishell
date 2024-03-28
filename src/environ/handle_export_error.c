/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 10:33:06 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:22:15 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	handle_export_error(char **argv, int *i)
{
	int	j;

	j = 0;
	while (ft_isalnum(argv[*i][j]) || argv[*i][j] == '_' ||
			argv[*i][j] == '#' || argv[*i][j] == '$' || argv[*i][j] == '=')
		j++;
	if (argv[*i][j] && argv[*i][j] != ' ')
	{
		if (ft_strchr(argv[*i], '='))
		{
			ft_putstrs_fd(PMT1, argv[0], ": `", 2);
			ft_putstrs_fd(argv[*i], argv[(*i) + 1], INVLD, 2);
			(*i)++;
		}
		else
		{
			ft_putstrs_fd(PMT1, argv[0], ": `", 2);
			ft_putstrs_fd(argv[*i], INVLD, 0, 2);
		}
		(*i)++;
		return (FAILURE);
	}
	return (SUCCESS);
}

void	print_error_export(t_mini *shell, int *i)
{
	ft_putstrs_fd(PMT1, shell->argv[0], ": `", 2);
	ft_putstrs_fd(shell->argv[*i], INVLD, 0, 2);
}

static char	**delete_env(char **envp, int i)
{
	int		j;
	int		len;
	char	**cpy;

	len = 0;
	while (envp[len])
		len++;
	cpy = (char **)ft_calloc(sizeof(char *), len);
	if (!cpy)
		return (NULL);
	j = -1;
	while (++j < i)
	{
		cpy[j] = ft_strdup(envp[j]);
		if (!cpy[j])
			return (NULL);
	}
	i++;
	while (envp[i])
	{
		cpy[j++] = ft_strdup(envp[i++]);
	}
	ft_free_2d_array(envp);
	return (cpy);
}

char	**handle_unset(t_mini *shell, int idx)
{
	int		i;
	int		len;
	char	*env;
	char	**cpy;

	if (shell->argc < 2)
		return (shell->envp);
	len = strlen(shell->argv[idx]);
	env = ft_strjoin(shell->argv[idx], "=");
	if (!env)
		return (NULL);
	i = 0;
	while (shell->envp[i] && ft_memcmp(env, shell->envp[i], len + 1))
		i++;
	if (shell->envp[i])
	{
		cpy = delete_env(shell->envp, i);
		if (!cpy)
			return (NULL);
	}
	else
		cpy = shell->envp;
	free(env);
	return (cpy);
}
