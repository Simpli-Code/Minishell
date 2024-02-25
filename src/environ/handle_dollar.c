/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 09:24:57 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/06 13:28:50 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
This function is responsible for printing an error msg
if after the environment variables there is no closing
braces
*/
int	is_open_braces(char **str)
{
	char	*ptr;

	ptr = ft_strchr((*str), '}');
	if (!ptr)
	{
		ft_putstrs_fd(PMT1, *str + 5, BAD, 2);
		return (FAILURE);
	}
	return (SUCCESS);
}

/*
This function is responsible for calculating the length
of the environment variables if the env is in braces
*/
int	helper_braces(t_mini *shell, char **str, int *i, int len)
{
	len = ft_strlen_char(*str + *i + 1, ':');
	if (len < ft_strlen_env(*str + *i + 1))
		len = len + 1;
	else
		len = ft_strlen_env(*str + *i + 1) + 1 + shell->braces;
	return (len);
}

/*
This function responsible for checking if the environment variables
enclosed in braces and if inside the braces there's no spaces
if inside the braces there's spaces it prints an error msg
*/
int	check_braces(t_mini *shell, char **str, int *i, int j)
{
	int	len;

	len = 0;
	if ((*str)[*i + 1] == '{')
	{
		if (is_open_braces(str) != SUCCESS)
			return (-1);
		j = (*i + 2);
		while (str[0][j])
		{
			if (str[0][j] == '}')
				break ;
			if (str[0][j] == ' ')
				return (ft_putstrs_fd(PMT1, *str + 5, BAD, 2), -1);
			j++;
		}
		shell->braces = 1;
	}
	else
		shell->braces = 0;
	return (helper_braces(shell, str, i, len));
}

/*
This function is responsible for duplicating the environment variables
from the given index
*/
char	*dup_cmd(char **str, int *i)
{
	char	*cmd;

	cmd = ft_strldup(*str, *i);
	if (!cmd)
		return (NULL);
	return (cmd);
}

char	*dup_value(t_mini *shell, char **str, int *len, int *i)
{
	char	*value;

	*len = (*len - 1 - shell->braces * 2);
	value = ft_strldup(*str + *i + 1 + shell->braces, *len);
	if (!value)
		return (NULL);
	return (value);
}
