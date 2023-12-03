/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_addstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 11:25:42 by chruhin           #+#    #+#             */
/*   Updated: 2023/12/01 12:36:14 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

// Add a string to the end of another string
void	ft_addstr(char **str, const char *addition)
{
	char	*temp;

	if (*str == NULL)
	{
		*str = strdup(addition);
	}
	else
	{
		temp = malloc(strlen(*str) + strlen(addition) + 1);
		if (temp == NULL)
		{
			perror("Memory allocation error");
			exit(EXIT_FAILURE);
		}
		strcpy(temp, *str);
		strcat(temp, addition);
		free(*str);
		*str = temp;
	}
}
