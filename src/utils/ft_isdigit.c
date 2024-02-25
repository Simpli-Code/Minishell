/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 13:48:41 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/06 19:04:10 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
   The isdigit() function tests for a decimal digit character.
   Regardless of locale, this includes the following characters only:

     ``0''         ``1''         ``2''         ``3''         ``4''
     ``5''         ``6''         ``7''         ``8''         ``9''
   The isdigit() functions return zero if the character tests false and
   return non-zero if the character tests true.
*/

#include "utils.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}
