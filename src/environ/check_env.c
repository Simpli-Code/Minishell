/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 09:33:11 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 20:01:33 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
scan the cmd for dollar sign to process env variable
It also handles special cases like single quotes and escaping characters.
If open single quotes are detected, it returns 1; otherwise, it returns 0.

initialize braces to 0,
to check if the current env variable is wrapped in curly braces.
iterate through the cmd until the end.
If a single quote is found,
it calls the open_quotes function to ensure the quotes is closed.
If open quotes, the function returns 1.
if ((*str)[i] && (*str)[i] == '\\') { ... }:
If a backslash, it checks for special cases:
If the next character is a dollar sign ('$'),
it removes the backslash.
if ((*str)[i + 1]) i++;
If there is a next character,
it increments the index to skip the escaped character
else if a dollar sign ('$') and it'str not followed by whitespace,
single quote, double quote, or a slash,
it calls the modify_env function to process the env variable.
*/
int	check_env(t_mini *shell, int i)
{
	while (shell->input != NULL && shell->input[i])
	{
		if (shell->input && shell->input[i] == '\'')
		{
			if (shell->input && shell->input[i - 1] == '"')
				i++;
			else
				break ;
		}
		if (shell->input && shell->input[i] == '\\')
		{
			if (shell->input && shell->input[i + 1] == '$')
				ft_remove_char(&shell->input, i);
			if (shell->input[i + 1])
				i++;
		}
		else if (shell->input[i] == '$' && !(!shell->input[i + 1] || \
				ft_isspace(shell->input[i + 1]) || shell->input[i + 1] == '\'' \
				|| shell->input[i + 1] == '"' || shell->input[i + 1] == '/'))
			if (modify_env(shell, i) != SUCCESS)
				return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
