/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 11:16:54 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/06 19:04:51 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
   The strjoin_fd() function is not a standard library function in C.
   strjoin() function takes two string 's1' and 's2' as parameter and
   joins 's1' and 's2' in a new string 'new_str' using malloc to allocate
   dynamicallay memory for the new string 'new_str'.
   Returns the new string 'new_str' and NULL if allocation fails.
*/

#include "utils.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	if (!s1 && !s2)
		return (NULL);
	new_str = (char *)ft_calloc(1, ft_strlen(s1) + ft_strlen(s2) + 1);
	if (new_str == NULL)
		return (NULL);
	if (s1)
		while (s1[i])
			new_str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		new_str[j++] = s2[i++];
	new_str[j] = '\0';
	return (new_str);
}
