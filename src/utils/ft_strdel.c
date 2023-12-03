/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 11:24:08 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/30 11:25:25 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

// Free the memory allocated for a string
void	ft_strdel(char **str)
{
	if (*str != NULL)
	{
		free(*str);
		*str = NULL;
	}
}
