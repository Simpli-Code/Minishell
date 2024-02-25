/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 17:27:57 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/25 18:14:14 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
	To calculates the length of a substring in a string until the first
	occurrence of the pipe character ('|'),
	considering quotes and escape characters.
	If the quotes are not properly closed, an error message is printed,
	and the function returns the current index. Otherwise,
		it returns the length of the substring.

	iterate through the string until it reaches the pipe character ('|')
	or the end of the string.
	If the current character is a double or single quote
	It sets the variable c to the quote character and increments the index i
	to move past the quote.
	The second loop iterates through the characters until it finds the matching
	quote character (c).
	If the current character is a backslash ('') and the next character is a
	token (special character),
	it skips the next character (escaping).
	If the end of the string is reached and a matching quote is not found
	print an error message and return the current index i.
	if the current character is a backslash (''),
	it skips the next character (escaping).
	and returns the final index i, representing the length of the substring.
*/
int	ft_strlen_pipe(char *str)
{
	int		i;
	char	c;

	i = -1;
	while (str[++i] && str[i] != '|')
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			c = str[i++];
			while (str[i] && str[i] != c)
			{
				if (str[i] == '\\' && ft_is_escape_token(str[i + 1]))
					i++;
				i++;
			}
			if (!str[i])
			{
				ft_putstr_fd("Non finished quotes\n", 2);
				return (i);
			}
		}
		else if (str[i] == '\\')
			i++;
	}
	return (i);
}
