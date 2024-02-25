/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_idx.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 11:08:40 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/31 08:10:27 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
is similar to the standard library function strchr, but it returns
the index of the first occurrence of the character c in the str.
*/
int	ft_strchr_idx(const char *str, int c)
{
	unsigned char	c_unsigned;
	int				i;

	i = 0;
	if (!str)
		return (-1);
	c_unsigned = (unsigned char)c;
	while (str[i])
	{
		if (str[i] == c_unsigned)
			return (i);
		i++;
	}
	if (c_unsigned == '\0')
		return (i);
	return (-1);
}
