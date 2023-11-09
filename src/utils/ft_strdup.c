/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 17:56:40 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/09 17:20:49 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
   The strdup() function allocates sufficient memory for a copy of the string s1,
   does the copy, and returns a pointer to it.  The pointer may subsequently be
   used as an argument to the function free(3).
   The strndup() function copies at most n characters from the string s1 always
   NULL terminating the copied string.
*/

#include "utils.h"

char	*ft_strdup(const char *s)
{
	char	*dupli;
	size_t	len;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dupli = ft_calloc(1, sizeof(char) * (len + 1));
	if (dupli == NULL)
		return (NULL);
	while (s[i])
	{
		dupli[i] = s[i];
		i++;
	}
	dupli[i] = '\0';
	return (dupli);
}
