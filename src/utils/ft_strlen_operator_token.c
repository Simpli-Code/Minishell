/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_operator_token.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:51:45 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/25 18:28:24 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
Calculate the length of a token related to operators

calculate the length of a substring within a str.
The len is determined based on certain characters (<, >, =, |),
spaces, and quoted strings.

if the character at index i in the input str is one of the special characters:
'<', '>', '=', or '|'. If it is, set i to either 1 or 2,
depending on whether it's a single '>' or a double '>>'.

If the character at index i is not one of the special characters,
enter the loop.
The loop will breaks if it finds a white space or special characters
if single or dubble quote increament i value (skip over them)
If the loop encounters an '=' character, it increments i by 1.
return the final value of i, the length of the substring.
*/

bool	ft_is_operator(char c)
{
	return (c == '<' || c == '>' || c == '=' || c == '|');
}

int	helper_function(char *str, int len)
{
	while (str[len] && !ft_isspace(str[len])
		&& !ft_is_operator(str[len]))
	{
		if (str[len] == '\'' || str[len] == '"')
		{
			len++;
			len += ft_strlen_arg_token(str + len, str[len - 1]);
			if (!str[len])
				return (len);
		}
		len++;
	}
	if (str[len] == '=')
		len++;
	return (len);
}

int	ft_strlen_operator_token(char *str)
{
	int	len;

	len = 0;
	if (ft_is_operator(str[len]))
	{
		if ((str[len] == '>' || str[len] == '<')
			&& (str[len + 1] == '>' || str[len + 1] == '<'))
			len = 2;
		else
			len = 1;
	}
	else
		len = helper_function(str, len);
	return (len);
}
