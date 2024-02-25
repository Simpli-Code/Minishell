/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_escape_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:30:55 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/19 12:19:58 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
check if quote of escape character
if so return FAIL
else return SUCCESS
*/
int	ft_is_escape_token(char c)
{
	if (c == '"' || c == '\\')
		return (FAILURE);
	return (SUCCESS);
}
