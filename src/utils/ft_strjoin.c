/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 11:16:54 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/31 18:29:07 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
strjoin() function takes two string 's1' and 's2' as parameter and
joins 's1' and 's2' in a new string 'new_str' using malloc to allocate
dynamicallay memory for the new string 'new_str'.
Returns the new string 'new_str' and NULL if allocation fails.
*/

#include "utils.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*str;

	if (!s1 && !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = malloc (len1 + len2 + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, len1);
	ft_memcpy(str + len1, s2, len2);
	str[len1 + len2] = '\0';
	return (str);
}
