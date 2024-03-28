/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 10:15:31 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:21:46 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_export_command(t_mini *shell, int *i)
{
	shell->envp = export_cmd(shell, *i);
	if (!shell->envp)
		return ;
	if (shell->argv[*i + 1])
		*i = 2;
	else
		*i = 1;
}

int	export_value(t_mini *shell, int *i)
{
	char	**after;
	int		j;

	if (!ft_strchr(shell->argv[*i], '='))
	{
		j = 0;
		(*i)++;
		while (shell->export[j] && ft_memcmp(shell->export[j], shell
				-> argv[*i], ft_strlen(shell->argv[*i])))
			j++;
		if (!shell->export[j])
		{
			after = ft_cpy_environ(shell->export, 1);
			after[j] = ft_strdup(shell->argv[*i]);
			if (!after[j])
				return (FAILURE);
			after[j + 1] = 0;
			ft_free_2d_array(shell->export);
			shell->export = after;
		}
		(*i)++;
	}
	else
		return (handle_export_command(shell, i), FAILURE);
	return (SUCCESS);
}
