/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:51:52 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/06 19:04:28 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
   The putstr_fd() function is not a standard library function in C.
   putstr_fd() is a function that writes a string of characters to a
   specified file descriptor."

   File descriptors are integer values that represent open files or other
   input/output resources in a program. Standard file descriptors include
   stdin (0), stdout (1), and stderr (2), where input, output, and error
   streams are typically associated, respectively.
*/

#include "utils.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}
