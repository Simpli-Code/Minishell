/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 17:56:40 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/20 13:04:47 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The strdup() function allocates sufficient memory for a copy of the string str,
does the copy, and returns a pointer to it.  The pointer may subsequently be
used as an argument to the function free(3).
The strndup() function copies at most n characters from the string s1 always
NULL terminating the copied string.
*/

#include "utils.h"

char	*ft_strdup(const char *str)
{
	char	*dupli;
	size_t	len;
	size_t	i;

	i = 0;
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	dupli = ft_calloc(1, sizeof(char) * (len + 1));
	if (dupli == NULL)
		return (NULL);
	while (str[i])
	{
		dupli[i] = str[i];
		i++;
	}
	dupli[i] = '\0';
	return (dupli);
}
