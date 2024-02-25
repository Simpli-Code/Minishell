/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cpy_argvs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 11:07:56 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/30 20:07:48 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
to create a copy of elements of argv into newly allocated (args)

iterate thorough each argv, to find the number of elements  until it
encounters an element that doesn't start with <.
This number is stored in i.
allocates memory for a new array of strings args with a size of i + 1
plus 1 for a NULL pointer to indicate the end of the array.

iterate again thorough each element and duplicate
Free previously allocated memory
Null-terminate the args array
returns the args array,
which is a copy of the relevant elements from shell->argv.
*/
char	**ft_cpy_argvs(t_mini *shell)
{
	int		i;
	int		j;
	char	**args;

	i = 0;
	while (shell->argv[i] && ft_memcmp(shell->argv[i], "<", 2))
		i++;
	args = ft_calloc(sizeof(char *), i + 1);
	if (!args)
		return (NULL);
	j = 0;
	while (j < i)
	{
		args[j] = ft_strdup(shell->argv[j]);
		if (args[j] == NULL)
		{
			ft_free_2d_array(args);
			free(args);
			return (NULL);
		}
		j++;
	}
	args[i] = NULL;
	return (args);
}
