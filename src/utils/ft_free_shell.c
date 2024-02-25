/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 07:55:46 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/04 12:58:45 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
to ensure that the memory allocated for various fields in the t_mini structure
is properly freed, preventing memory leaks.
when the t_mini structure is no longer needed,
typically when the program is finished using it or when cleaning up resources.
*/
void	free_shell(t_mini *shell)
{
	if (shell->envp)
	{
		ft_free_2d_array(shell->envp);
		shell->envp = NULL;
	}
	if (shell->export)
	{
		ft_free_2d_array(shell->export);
		shell->export = NULL;
	}
	if (shell->argv)
	{
		ft_free_2d_array(shell->argv);
		shell->argv = NULL;
	}
	if (shell->fd_in)
		close(shell->fd_in);
	if (shell->fd_out)
		close(shell->fd_out);
}
