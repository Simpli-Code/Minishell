/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 08:44:07 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/06 13:49:09 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
it check whether to skip the first argument ("-n") and then print
the remaining arguments to a specified file descriptor, and adds
a space between non-empty arguments
sets i value either to 1 or 0 depending on if ("-n") is present
loops thorough each argument starting from actual i value
inside the loop prints the current argument
checks if it's the last argument and the next argument is not empty
if so print a space
outside the loop it checks if the '-n' option is not present,
print a newline character
*/
void	handle_enn(t_mini *shell, int fd)
{
	int	i;
	int	j;
	int	flag;

	i = 1;
	j = 2;
	flag = 0;
	while ((int)ft_strlen(shell->argv[1]) > j)
	{
		if (shell->argv[1][j] != 'n')
		{
			flag = 1;
			i = 0;
			break ;
		}
		j++;
	}
	while (++i < shell->argc)
	{
		ft_putstr_fd(shell->argv[i], fd);
		if (i < shell->argc - 1 && ft_strlen(shell->argv[i + 1]))
			write(fd, " ", 1);
	}
	if (shell->argc > 1 && flag)
		write(fd, "\n", 1);
}

void	handle_echo(t_mini *shell, int fd)
{
	int	i;

	i = 0;
	if (shell->argc > 1 && shell->argv[1][0] == '-' && shell->argv[1][1] == 'n')
	{
		handle_enn(shell, fd);
		return ;
	}
	while (++i < shell->argc)
	{
		ft_putstr_fd(shell->argv[i], fd);
		if (i < shell->argc - 1 && ft_strlen(shell->argv[i + 1]))
			write(fd, " ", 1);
	}
	if (shell->argc > 1)
		write(fd, "\n", 1);
}
