/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 17:56:13 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/06 19:04:00 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	The  calloc()  function allocates memory for an array of nmemb elements
    of size bytes each and returns a pointer to the allocated memory.   The
    memory  is  set  to zero.  If nmemb or size is 0, then calloc() returns
    either NULL, or a unique pointer value that can later  be  successfully
    passed to free().  If the multiplication of nmemb and size would result
    in integer overflow, then calloc() returns an error.  By  contrast,  an
    integer  overflow  would  not be detected in the following call to mal‐
    loc(), with the result that an incorrectly sized block of memory  would
    be allocated:
*/

#include "utils.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*str;
	size_t	i;

	i = 0;
	str = (char *)malloc(nmemb * size);
	if (str == NULL)
		return (NULL);
	while (i < nmemb * size)
	{
		str[i] = 0;
		i++;
	}
	return ((void *)str);
}
