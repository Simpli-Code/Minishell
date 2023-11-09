/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:14:03 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/06 19:04:07 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
   The isalpha() function tests for any character for which isupper(3) or
   islower(3) is true. The value of the argument must be representable as
   an unsigned char or the value of EOF.
   The isalpha() function returns zero if the character tests false and
   returns non-zero if the character tests true.
*/

#include "utils.h"

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}
