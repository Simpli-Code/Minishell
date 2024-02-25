/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 08:08:33 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/03 08:29:37 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
to handle syntax errors, setting the exit status and
printing a informative error message for various syntax-related
issues based on the type of error encountered
*/
int	syntax_error(t_mini *shell, int err_flag, char c)
{
	shell->e_status = 2;
	if (err_flag == QUOTE)
	{
		ft_putstrs_fd(0, PMT1, "ERROR: unclosed ", 2);
		if (c == '\'')
		{
			ft_putstrs_fd(0, 0, "single quotes: `''\n", 2);
			shell->e_status = 127;
		}
		else if (c == '"')
		{
			ft_putstrs_fd(0, 0, "double quotes: `\"'\n", 2);
			shell->e_status = 127;
		}
		return (SYN_ERR);
	}
	ft_putstrs_fd(0, PMT1, "ERROR: near unexpected token `", 2);
	if (err_flag == TOKEN)
	{
		ft_putchar_fd(c, 2);
		ft_putstrs_fd(0, 0, "'\n", 2);
	}
	else if (err_flag == NL)
		ft_putendl_fd("newline'", 2);
	return (SYN_ERR);
}

/*
to check for valid redirection syntax during the parsing
of commands to ensure that the redirection syntax is correct
*/
int	check_redirection(t_mini *shell, char *str, int *pos, char symbol)
{
	int	i;

	i = *pos + 1;
	if (str[i] == symbol && str[i + 1] != symbol)
		i++;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '\0')
		return (syntax_error(shell, NL, '\n'));
	if (str[i] == '|' || str[i] == '<' || str[i] == '>')
		return (syntax_error(shell, TOKEN, str[i]));
	(*pos)++;
	return (SUCCESS);
}

/*
to check for valid pipe syntax
for example if there is pipe symbol at the beginig
or if at the end with no cmd after it
or multiple pipe symbol
if so, it returns an erre msg
*/
int	check_pipe(t_mini *shell, char *str, int *pos)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (i == *pos)
		return (syntax_error(shell, TOKEN, str[*pos]));
	i = *pos + 1;
	if (str[i] == '|')
		return (syntax_error(shell, TOKEN, str[*pos]));
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '\0')
		return (syntax_error(shell, TOKEN, str[*pos]));
	else if (str[i] == '|')
	{
		if (ft_isspace(str[i + 1]) || str[i + 1] == '\'' || \
			str[i + 1] == '\"')
			return (syntax_error(shell, TOKEN, str[i]));
	}
	(*pos)++;
	return (SUCCESS);
}

/*
to check if there is an open quotes in the input string
if so, it returns an error message indicating open quotes
*/
int	check_quotes(t_mini *shell, char *quote_start, int *pos, char target)
{
	char	*quote_end;

	quote_end = ft_strchr((quote_start + 1), target);
	if (!quote_end)
		return (syntax_error(shell, QUOTE, target));
	*pos += quote_end - quote_start;
	(*pos)++;
	return (SUCCESS);
}

/*
loops through the input string and checks for various
cases such as open quotes, invalid pipe symbol, and
redirection symbol in the input string
*/
int	is_input_valid(t_mini *shell)
{
	int	err_flag;
	int	i;

	i = 0;
	err_flag = 0;
	while (shell->input[i])
	{
		if (ft_isspace(shell->input[i]))
			i++;
		else if (shell->input[i] == '"' || shell->input[i] == '\'')
			err_flag = check_quotes(shell, \
				&shell->input[i], &i, shell->input[i]);
		else if (shell->input[i] == '|')
			err_flag = check_pipe(shell, shell->input, &i);
		else if (shell->input[i] == '<' || shell->input[i] == '>')
			err_flag = check_redirection(shell, \
				shell->input, &i, shell->input[i]);
		else
			i++;
		if (err_flag != SUCCESS)
			return (err_flag);
	}
	return (SUCCESS);
}
