/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 19:06:37 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/06 12:33:57 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
counting the number of arguments in a given string str
loop thorough 'str'
skip white spaces
the counter 'n' counts the number of arguments
advance the string pointer by the length of the current argument
skip white spaces again
return the total count of arguments 'n'
*/
int	ft_count_commands(char *str)
{
	int	count;

	count = 0;
	ft_skip_spaces(&str);
	while (*str)
	{
		ft_skip_spaces(&str);
		count++;
		str += ft_strlen_operator_token(str);
		ft_skip_spaces(&str);
	}
	return (count);
}
