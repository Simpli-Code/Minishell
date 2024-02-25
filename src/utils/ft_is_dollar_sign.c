/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_dollar_sign.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 07:58:32 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/31 13:36:37 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
checks if there is a dollar sign in the input string
*/
int	is_dollar_sign(t_mini *shell)
{
	char	*ptr;

	ptr = ft_strchr(shell->input, '$');
	if (!ptr)
	{
		return (FAILURE);
	}
	else
		return (SUCCESS);
}
