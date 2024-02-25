/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strldup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:59:02 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/27 19:25:29 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*ft_strldup(char *str, int len)
{
	char	*ptr;
	int		size;
	int		i;

	size = 0;
	while (str[size])
		size++;
	if (len < size)
		size = len;
	ptr = (char *)ft_calloc(1, sizeof(char) * (size + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < size)
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
