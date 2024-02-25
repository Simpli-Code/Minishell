/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_printable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 12:24:18 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/25 18:13:02 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
Check if the character is printable
The ASCII printable characters have values between 32 and 126
*/
int	ft_is_printable(int c)
{
	return (c >= 32 && c <= 126);
}
