/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocument.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:41:58 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 17:18:44 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
This function is responsible for checking if in the input string
there is env variables, if so it calls handle_env_heredoc
else writes the context of here document to the specified fd
*/
static void	check_for_env(t_mini *shell, char *str[1], int fd)
{
	int	i;
	int	j;

	i = 0;
	while (str && str[1] && str[1][i])
	{
		j = i + 1;
		if (str[1][i] && str[1][i] == '$' && str[1][j] && ft_isalnum(str[1][j]))
			handle_env_heredoc(shell, str[1], &i, fd);
		else if (str[1][i])
		{
			write(fd, &str[1][i], 1);
		}
		i++;
	}
}

/*
This function is responsible for reading heredoc text via readline
exits loop when eof is entered or Ctrl+C or Ctrl+D
*/
char	*loop_heredoc(t_mini *shell, char *str[3])
{
	while (shell->e_status != 130 && g_state != 130)
	{
		if (!str[1])
			str[1] = ft_strdup("");
		if (!str[0])
			str[0] = ft_strdup("");
		str[2] = str[1];
		str[1] = ft_strjoin(str[1], str[0]);
		free(str[0]);
		free(str[2]);
		str[0] = readline("> ");
		if (!str[0])
		{
			printf("%s (wanted `%s\')\n", \
			"bash: warning: here-document terminated by Ctrl + D", shell->eof);
			break ;
		}
		str[2] = str[0];
		if (str[2] && str[2][0] && !ft_strncmp(str[2], shell->eof,
			ft_strlen(str[2])) && ft_strlen(str[2]) == ft_strlen(shell->eof))
			break ;
		str[0] = ft_strjoin(str[0], "\n");
		free(str[2]);
	}
	return (free(str[0]), str[1]);
}

/*
This function is responsible for setting up signal handling in heredoc,
as well as declaring string array for saving the readed input from heredoc
returns the read end side of pipe: fds[0] so that the content can later be read
*/
int	handle_heredoc(t_mini *shell, int fd)
{
	char	*str[3];

	str[0] = NULL;
	str[1] = NULL;
	str[2] = NULL;
	set_delimiter(shell);
	g_state = 0;
	shell->e_status = 0;
	signal(SIGINT, handle_sig_heredoc);
	signal(SIGQUIT, SIG_IGN);
	str[1] = loop_heredoc(shell, str);
	check_for_env(shell, str, fd);
	free(str[1]);
	return (fd);
}

/*
This function is respponsible for finding out if in the input string
there is a heredoc redirection symbol, if so it returns SUCCESS
indicating YES there is a here document to handle, else NO
*/
int	is_heredoc_sign(t_mini *shell)
{
	int	i;

	i = 0;
	while (shell->argv[i] && ft_memcmp(shell->argv[i], "<<", 3))
	{
		i++;
	}
	if (shell->argv[i])
	{
		return (SUCCESS);
	}
	else
	{
		return (FAILURE);
	}
}
