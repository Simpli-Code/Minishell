/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 08:44:07 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:21:32 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
