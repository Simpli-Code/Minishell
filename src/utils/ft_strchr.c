/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 17:56:33 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/28 22:37:04 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The strchr() function locates the first occurrence of c
(converted to a char) in the string pointed to by s.
The terminating null character is considered to be part of the string;
therefore if c is `\0', the functions locate the terminating `\0'.

The functions strchr() return a pointer to the located character,
or NULL if the character does not appear in the string.
*/

#include "utils.h"

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s != '\0' && *s != (unsigned char) c)
	{
		s++;
	}
	if (*s == (unsigned char) c)
		return ((char *) s);
	return (NULL);
}
