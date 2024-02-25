/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 09:16:21 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/25 18:13:33 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
To calculates the length of a substring in a string until the first occurrence
of a specified character (c)
It counts the number of characters in the substring until the target character
is encountered or the end of the string is reached,
and then returns the length of the substring.

iterate through the string until it reaches the target character (c)
or the end
of the string.
return the final index i, the length of the substring.
*/
int	ft_strlen_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}
