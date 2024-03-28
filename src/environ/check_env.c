/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 09:33:11 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:21:41 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_env(t_mini *shell, int i)
{
	while (shell->input != NULL && shell->input[i])
	{
		if (shell->input && shell->input[i] == '\'')
		{
			if (shell->input && shell->input[i - 1] == '"')
				i++;
			else
				break ;
		}
		if (shell->input && shell->input[i] == '\\')
		{
			if (shell->input && shell->input[i + 1] == '$')
				ft_remove_char(&shell->input, i);
			if (shell->input[i + 1])
				i++;
		}
		else if (shell->input[i] == '$' && !(!shell->input[i + 1] || \
				ft_isspace(shell->input[i + 1]) || shell->input[i + 1] == '\'' \
				|| shell->input[i + 1] == '"' || shell->input[i + 1] == '/'))
			if (modify_env(shell, i) != SUCCESS)
				return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
