/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 21:44:04 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 23:42:55 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
This function is responsible for checking if the first cmd is ">>" or "<<"
of '$' , if so it returns failure else success
*/
int	is_invalid_cmd(t_mini *shell, char	*str)
{
	int	i;

	i = 0;
	while (i < 1)
	{
		if (str[i] == '$' || str[i] == '\'' \
			|| str[i] == '>' || str[i + 1] == '<')
		{
			ft_putstrs_fd(PMT1, str, ": command not found\n", 2);
			shell->e_status = 127;
			return (FAILURE);
		}
		if ((str[i] == '"' || str[i] == '\'') \
			&& (str[i + 1] == '>' || str[i + 1] == '<'))
		{
			ft_putstrs_fd(PMT1, str, ": command not found\n", 2);
			shell->e_status = 127;
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}
