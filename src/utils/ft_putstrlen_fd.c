/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstrlen_fd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 18:20:02 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/06 18:20:15 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	ft_putstrlen_fd(char *s, int len, int fd)
{
	int i;

	if (!s || !fd)
		return ;
	i = 0;
	while (s[i] && i < len)
	{
		write(fd, &s[i], 1);
		i++;
	}
}
