/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 07:49:58 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/06 14:14:47 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
the free loop function will only free the memory that should be freed when the
inifinite loop reaches the end starts from beginig and allocates memory again.
*/
void	free_loop(t_mini *shell)
{
	if (shell->argv)
	{
		ft_free_2d_array(shell->argv);
		shell->argv = NULL;
	}
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->fd_in)
		close(shell->fd_in);
	if (shell->fd_out)
		close(shell->fd_out);
}
