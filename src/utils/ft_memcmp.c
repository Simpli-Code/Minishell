/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 17:56:26 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/02 18:01:59 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
The	memcmp(void)  function  compares  the first n bytes (each interpreted as
unsigned char) of the memory areas s1 and s2.
The	memcmp(void) function returns an integer less than, equal to, or greater
than zero if the first n bytes of s1 is found, respectively, to be less than,
to match, or be greater than the first n bytes of s2.
For a nonzero return value,	the sign is determined by the sign of the difference
between the first pair of bytes (interpreted as unsigned char) that differ in
s1 and s2.
DIFFERENCE TO STRNCMP: return is 1 or -1, unregarding the real diff
If n is zero, the return value is zero.
*/

//int	ft_memcmp(const void *s1, const void *s2, size_t n)
//{
//	int					diff;
//	size_t				i;
//	const unsigned char	*str1;
//	const unsigned char	*str2;

//	i = 0;
//	diff = 0;
//	str1 = s1;
//	str2 = s2;
//	if (n == 0)
//		return (0);
//	while (diff == 0 && i < n)
//	{
//		if (str1[i] != str2[i])
//			diff = str1[i] - str2[i];
//		i++;
//	}
//	if (diff > 0)
//		return (1);
//	else if (diff < 0)
//		return (-1);
//	return (diff);
//}

// int	main(void)
// {
// 	char	*str1;
// 	char	*equal;
// 	char	*greater;
// 	char	*lesser;
// 	int		i;
// 	size_t	n;
// 	str1 = "chulu";
// 	equal = "chulu";
// 	greater = "cfuly";
// 	lesser = "chulxfobo";
// 	n = 5;
// 	i = memcmp(str1, equal, n);
// 	printf("Equal: %d\n", i);
// 	i = memcmp(str1, greater, n);
// 	printf("Greater: %d\n", i);
// 	i = memcmp(str1, lesser, n);
// 	printf("Lesser: %d\n", i);
// 	i = ft_memcmp(str1, equal, n);
// 	printf("Equal: %d\n", i);
// 	i = ft_memcmp(str1, greater, n);
// 	printf("Greater: %d\n", i);
// 	i = ft_memcmp(str1, lesser, n);
// 	printf("Lesser: %d\n", i);
// 	return (0);
// }

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	i = 0;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}
