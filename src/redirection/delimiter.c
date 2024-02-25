/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimiter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:30:20 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/31 11:44:19 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
This function is resposible for finding the delimiter variable in the
input string. It loop through the input str and if it it finds
the delimiter it updates the shell->eof accordingly.
*/
void	set_delimiter(t_mini *shell)
{
	int	i;

	i = 0;
	shell->eof = NULL;
	while (shell->argv[i])
	{
		if (ft_memcmp(shell->argv[i], "<<", 3))
		{
			shell->eof = shell->argv[i + 2];
			break ;
		}
		i++;
	}
}
