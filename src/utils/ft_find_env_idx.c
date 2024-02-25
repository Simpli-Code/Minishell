/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_env_idx.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:20:14 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/25 19:08:03 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	find_env_var_index(char *var, char **envp)
{
	int	i;
	int	n;
	int	n2;

	i = 0;
	n = ft_strlen(var);
	while (envp && envp[i])
	{
		n2 = n;
		if (n2 < ft_strchr_idx(envp[i], '='))
			n2 = ft_strchr_idx(envp[i], '=');
		if (!ft_strncmp(envp[i], var, n2))
		{
			return (i);
		}
		i++;
	}
	return (-1);
}
