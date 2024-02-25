/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 09:29:44 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 10:41:14 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
handle environment variables for heredoc.
it finds the end of the variable starting at str[*i].
it extracts the variable name, looks up the variable,
and outputs to the given fd.
it sets *i to the next character after the variable name
*/
void	handle_env_heredoc(t_mini *shell, char *str, int *i, int fd)
{
	int		j;
	char	*tmp;
	char	*tmp2;

	j = *i + 1;
	while (str[j])
	{
		if (!ft_isalnum(str[j]) || str[j] == ' ' || str[j] == '\n'
			|| str[j] == '\t' || str[j] == '$' || str[j] == '\"'
			|| str[j] == '\'' || str[j] == '<')
			break ;
		j++;
	}
	tmp = ft_substr(str, *i + 1, j - *i - 1);
	tmp2 = ft_getenv_heredoc(tmp, shell->envp);
	write(fd, tmp2, ft_strlen(tmp2));
	free (tmp);
	free (tmp2);
	*i = j - 1;
}
