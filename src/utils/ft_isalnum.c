/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 14:28:33 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/19 12:20:17 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The isalnum() function tests for any character for which isalpha(3) or
isdigit(3) is true.
The value of the argument must be representable as an unsigned char or the
value of EOF.
The isalnum() function returns zero if the character tests false and returns
non-zero if the character tests true.
*/

#include "utils.h"

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	else
		return (0);
}
