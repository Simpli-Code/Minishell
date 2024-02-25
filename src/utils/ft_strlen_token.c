/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 08:23:55 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/25 18:19:31 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
	To calculates the length of a token or a substring
	in a string until the first
	occurrence of specific characters,
	considering various characters as potential separators.
	increment the length for each character in the
	token and return the total length.
	If the last character is an equal sign,
	an additional increment is performed.

	iterate through the string until it reaches one of the specified characters
	(space, semicolon, pipe, less-than, greater-than, double and single quotes,
	equal sign) or the end of the string.
	len++; str++;: For each character that is
	not one of the specified separators,
	it increments the len variable
	and moves the pointer str to the next character.

	If the last character encountered is an equal sign ('='),
		it increments the len variable.
	return the final value of len, the length of the token or substring.
*/
int	ft_strlen_token(char *str)
{
	int	len;

	len = 0;
	while (*str && *str != ' ' && *str != ';' && *str != '|' && *str != '<'
		&& *str != '>' && *str != '"' && *str != '\'' && *str != '=')
	{
		len++;
		str++;
	}
	if (*str == '=')
		len++;
	return (len);
}
