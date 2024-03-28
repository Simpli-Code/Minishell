/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:06:49 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:22:21 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**copy_and_extend_env(t_mini *shell, int size)
{
	char	**cpy;

	cpy = ft_cpy_environ(shell->envp, size);
	if (!cpy)
	{
		ft_free_2d_array(cpy);
		return (NULL);
	}
	return (cpy);
}

static char	**create_and_extend_env(t_mini *shell, int i, int j)
{
	char	**cpy;

	cpy = copy_and_extend_env(shell, i + 1);
	if (!cpy)
		return (NULL);
	cpy[i] = ft_strjoin(shell->argv[j], shell->argv[j + 1]);
	if (!cpy[i])
	{
		ft_free_2d_array(cpy);
		return (NULL);
	}
	ft_free_2d_array(shell->envp);
	return (cpy);
}

static char	**update_env_entry(t_mini *shell, int i, int j)
{
	free(shell->envp[i]);
	shell->envp[i] = ft_strjoin(shell->argv[j], shell->argv[j + 1]);
	if (!shell->envp[i])
	{
		ft_free_2d_array(shell->envp);
		return (NULL);
	}
	return (shell->envp);
}

char	**export_cmd(t_mini *shell, int j)
{
	int	i;

	i = 0;
	while (shell->envp[i] && ft_memcmp(shell->envp[i], shell->argv[j],
			ft_strlen(shell->argv[j])))
		i++;
	if (!shell->envp[i])
	{
		return (create_and_extend_env(shell, i, j));
	}
	else
	{
		return (update_env_entry(shell, i, j));
	}
}
