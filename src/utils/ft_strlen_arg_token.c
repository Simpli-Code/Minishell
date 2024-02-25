/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_arg_token.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 23:13:31 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/19 12:18:11 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
calculate the len of substr in the str
loop through str until the delimeter is found or EOL
check for scape character to have the correct len

if the current character is a backslash ('')
and the next character is a token (special character),
skip the next character (escaping).
return the final index i, the length of the substring.
*/
int	ft_strlen_arg_token(char *str, char delimiter)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != delimiter)
	{
		if (str[len] == '\\' && ft_is_escape_token(str[len + 1]))
		{
			len++;
		}
		len++;
	}
	return (len);
}
