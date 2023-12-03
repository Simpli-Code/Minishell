/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 19:23:16 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/29 19:23:46 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*ft_substr(char const *s, unsigned int position, size_t len)
{
	size_t	i;
	size_t	slen;
	char	*substr;

	i = 0;
	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (position >= slen)
		len = 0;
	if (len >= slen - position)
		len = slen - position;
	substr = (char *)ft_calloc(1, sizeof(char) * (len + 1));
	if (substr == NULL)
		return (NULL);
	while (i < len)
	{
		substr[i++] = s[position++];
	}
	substr[i] = '\0';
	return (substr);
}
