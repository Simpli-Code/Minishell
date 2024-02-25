/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 21:16:08 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/01 18:46:44 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
This function is responsible for handling the output redirection by opening
and managing a file descriptors based on the presence of ">" and ">>"
checks if the file is readable, read a character from it.
If there is an issue reading the file (e.g., due to permissions),
print an error message.
checks if there are more arguments, and if the next argument also
indicates output redirection ("<<" or ">").
If so, it closes the current file descriptor to prepare for the next redirection.
return the file descriptor after completing the redirection operations.
*/
int	out_file(t_mini *shell, int idx, int fd)
{
	int		bytesread;
	char	c;

	while (shell->argv[idx])
	{
		if (!ft_memcmp(shell->argv[idx], ">", 2))
			fd = open(shell->argv[idx + 1], O_RDWR | O_CREAT | O_TRUNC, 0666);
		else if (!ft_memcmp(shell->argv[idx], ">>", 3))
		{
			fd = open(shell->argv[idx + 1], O_RDWR | O_CREAT | O_APPEND, 0666);
			bytesread = 0;
			while (bytesread != 0)
			{
				bytesread = read(fd, &c, 1);
				if (bytesread == -1)
					break ;
			}
		}
		idx++;
		if (shell->argv[idx] && (!ft_memcmp(shell->argv[idx], ">>", 3)
				|| !ft_memcmp(shell->argv[idx], ">", 2)))
			close(fd);
	}
	return (fd);
}
