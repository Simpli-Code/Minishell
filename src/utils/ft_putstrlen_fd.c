/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstrlen_fd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 18:20:02 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/25 18:29:28 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	ft_putstrlen_fd(char *s, int len, int fd)
{
	int	i;

	if (!s || !fd)
		return ;
	i = 0;
	while (s[i] && i < len)
	{
		write(fd, &s[i], 1);
		i++;
	}
}
