/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 19:03:33 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/03 14:02:27 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
  The memcpy() function copies n bytes from memory area src to memory area dst.
  If dst and src overlap, behavior is undefined. Applications in which dst and
  src might overlap should use memmove(3) instead.

  The memcpy() function returns the original value of dst.
*/

#include "utils.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*str;

	if (!dest)
		return (NULL);
	if (!src)
		return (NULL);
	str = (unsigned char *)dest;
	while (n--)
		*str++ = *((unsigned char *) src++);
	return (dest);
}
