/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:00:05 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/25 18:17:04 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
	iterate through the string until it reaches a character that is not
	alphanumeric,
	an opening curly brace ('{'), the question mark ('?'),
		or the underscore ('_'), or the end of the string.
	For each valid character in the environment variable name,
	it increments the len variable and moves the pointer str to the next
	character.
	If the last character encountered is an equal sign ('='),
	it increments the len variable. return the final value of len, the length
	of the environmentvariable name or substring.
*/
int	ft_strlen_env(char *str)
{
	int	len;

	len = 0;
	while (*str && (ft_isalnum(*str)
			|| *str == '{' || *str == '?' || *str == '_'))
	{
		len++;
		str++;
	}
	if (*str == '=')
		len++;
	return (len);
}
