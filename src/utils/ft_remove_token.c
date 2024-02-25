/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:19:57 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 22:05:09 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
loop thorough each character of arg, starting from the pointed index
if the current character is a backslash (\) and the next character is a token
remove the backslash ft_remove_char

else if the current character is a double quote ("),
it breaks out of the loop.
stop processing if a double quote is found
*/
static void	ft_removebackslash(char **arg, int *i)
{
	while ((*arg)[*i])
	{
		if ((*arg)[*i] == '\\' && ft_is_escape_token((*arg)[*i + 1]))
			ft_remove_char(arg, *i);
		else if ((*arg)[*i] == '"')
			break ;
		(*i)++;
	}
}

int	is_pipe(char **arg)
{
	char	*ptr;

	ptr = ft_strchr(*arg, '|');
	if (!ptr)
		return (FAILURE);
	else
		return (SUCCESS);
}

/*
Removes tokens from string 'arg' tokens are "single/double quote, and backslash"

loops through each element of 'arg'
first checks for single quote and removes it
if it's a single quote then it's a single quoted string
the ft_remove_char function removes the single quote
i value will be increamented by the len of that character
call again ft_remove_char to remove the closing single quote

secode checks for double quote and removes it
ft_remove_char receives the string and the character to be removed
ft_removebackslash, removes escaped character
and remove the closing duoble quote

else if the current character is a backslash and next
character is either a token or a single quote ('),
it removes the backslash by calling ft_remove_char.

If none of the above conditions are met,
increment the index i to move to the next character.
*/
void	ft_remove_token(char **arg)
{
	int	i;

	i = 0;
	if (!is_pipe(arg))
		return ;
	while ((*arg)[i])
	{
		if ((*arg)[i] == '\'')
		{
			ft_remove_char(arg, i);
			i += ft_strlen_char(*arg + i, '\'');
			ft_remove_char(arg, i);
		}
		else if ((*arg)[i] == '"')
		{
			ft_remove_char(arg, i);
			ft_removebackslash(arg, &i);
			ft_remove_char(arg, i);
		}
		else if (((*arg)[i] == '\\') && (ft_is_escape_token((*arg)[i + 1])
					|| (*arg)[i + 1] == '\''))
			ft_remove_char(arg, i++);
		else
			i++;
	}
}
