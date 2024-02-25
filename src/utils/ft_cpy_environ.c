/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cpy_environ.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 13:18:25 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/31 18:07:11 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
create a copy and allocate memory for of env variables,
duplicate each string from the original array,
add parameter allows for flexibility in
the size of the copied array.

calculate the number of strings in the input array envp by counting
the elements until a NULL pointer is found.

iterate through each element of the input array envp and
duplicate the string for each element
store the duplicated string in the copy array
return a pointer to the copied array of environment variables.
*/
char	**ft_cpy_environ(char **envp, int add)
{
	int		len;
	int		i;
	char	**cpy;

	len = 0;
	while (envp[len])
		len++;
	cpy = (char **)ft_calloc(sizeof(char *), (len + add + 1));
	if (!cpy)
		return (NULL);
	i = -1;
	while (++i < len)
	{
		cpy[i] = ft_strdup(envp[i]);
		if (!cpy[i])
			return (NULL);
	}
	return (cpy);
}
