/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_file.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 20:36:12 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 10:43:43 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
This function is responsible for setting up a file descriptor for
infileredirection. It does it so by looping through the input str
whenever it founds a "<" symbol it breaks the loop and sets a fd to i + 1
*/
int	handle_infile(t_mini *shell)
{
	int	fd;
	int	i;

	fd = 0;
	i = 0;
	while (shell->argv[i] && ft_memcmp(shell->argv[i], "<", 2))
	{
		i++;
	}
	if (shell->argv[i])
	{
		fd = open(shell->argv[i + 1], O_RDONLY, 0666);
		if (fd < 0)
		{
			ft_putstrs_fd(PMT1, shell->argv[i + 1], NSFD, 2);
			return (-1);
		}
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (SUCCESS);
}
