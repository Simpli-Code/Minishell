/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 14:20:05 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/06 19:04:17 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
   The itoa() function is not a standard library function in C. However,
   it is a common function that is often implemented in various libraries or
   provided as a custom implementation. The name "itoa" stands for "integer
   to ASCII."
   The itoa() function converts an integer value to a string representation
   in base 10 (decimal). It takes an integer value as input and returns a
   pointer to a character array (string) that represents the input integer.
*/

#include "utils.h"

int	ft_length(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_n_is_nul(char *s, int n)
{
	if (n == 0)
	{
		s[0] = '0';
		s[1] = '\0';
	}
	return (s);
}

char	*ft_itoa(int n)
{
	long	num;
	char	*s;
	int		len;

	num = n;
	len = ft_length(num);
	s = (char *)ft_calloc(1, sizeof(char) * (len + 1));
	if (s == NULL)
		return (NULL);
	if (num < 0)
	{
		s[0] = '-';
		num = -num;
	}
	ft_n_is_nul(s, num);
	s[len] = '\0';
	while (num != 0)
	{
		len--;
		s[len] = (num % 10) + '0';
		num = num / 10;
	}
	return (s);
}
