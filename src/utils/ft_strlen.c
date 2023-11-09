/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:16:48 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/06 19:05:03 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
   The strlen() function computes the length of the string s.
   The strnlen() function attempts to compute the length of s
   but never scans beyond the first maxlen bytes of s.

   The strlen() function returns the number of characters that precede the
   terminating NUL character.  The strnlen() function returns either the same
   result as strlen() or maxlen, whichever is smaller.
*/

#include "utils.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		i++;
	}
	return (i);
}
