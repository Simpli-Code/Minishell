/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:20:42 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:22:48 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	assign_ret_value(t_mini *shell, char **value, char **cur_env)
{
	if (ft_memcmp(*value, "?", 2) == 0)
		*cur_env = ft_itoa(shell->e_status);
	else
		*cur_env = NULL;
}

static int	extract_value(t_mini *shell, char **cur_env, char **value)
{
	if (*cur_env == NULL)
	{
		*cur_env = get_env_path(shell->envp, *value);
		if (!*cur_env)
		{
			*cur_env = ft_strdup(" ");
			if (!*cur_env)
				return (SUCCESS);
		}
		else
		{
			*cur_env = ft_strdup(*cur_env);
			if (!*cur_env)
				return (FAILURE);
		}
	}
	return (SUCCESS);
}

static char	*concat_str(char **str, char **cmd, \
	char **cur_env, char **substraft)
{
	char	*result;

	result = ft_strjoin(*cmd, *cur_env);
	if (!result)
		return (free(result), NULL);
	if (*cmd)
		free(*cmd);
	if (*cur_env)
		free(*cur_env);
	*str = ft_strjoin(result, *substraft);
	if (!*str)
		return (free(*str), NULL);
	if (result)
		free (result);
	if (*substraft)
		free (*substraft);
	return (*str);
}

char	*helper_modify_env(t_mini *shell, char *tmp, int len, int i)
{
	char	*cmd;
	char	*value;
	char	*cur_env;
	char	*substraft;

	cmd = dup_cmd(&tmp, &i);
	if (!cmd)
		return (free(cmd), NULL);
	value = dup_value(shell, &tmp, &len, &i);
	if (!value)
		return (free(value), NULL);
	assign_ret_value(shell, &value, &cur_env);
	if (shell->braces)
		substraft = ft_strdup(tmp + i + 1 + (shell->braces * 2) + len);
	else
		substraft = ft_strdup(tmp + i + 1 + len);
	if (!substraft)
		return (free(substraft), NULL);
	if (extract_value(shell, &cur_env, &value) != SUCCESS)
		return (NULL);
	free(value);
	tmp = concat_str(&tmp, &cmd, &cur_env, &substraft);
	if (!tmp)
		return (free(tmp), NULL);
	return (tmp);
}

int	modify_env(t_mini *shell, int i)
{
	int		j;
	int		len;
	char	*tmp;

	j = 0;
	tmp = ft_strdup(shell->input);
	if (!tmp)
		return (-1);
	free(shell->input);
	len = check_braces(shell, &tmp, &i, j);
	if (len == -1)
	{
		free (tmp);
		return (-1);
	}
	shell->input = helper_modify_env(shell, tmp, len, i);
	free(tmp);
	tmp = NULL;
	return (SUCCESS);
}
