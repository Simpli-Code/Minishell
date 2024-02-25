/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:28:35 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 17:18:13 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
to count the number of output redirections

loop through the arguments and increments the counter
whenever it finds ">" or ">>".
if so, increment the counter and increment i by 1 to skip
the next argument (the file name after ">" or ">>")
return the final count of output redirections
*/
static int	count_redirection(t_mini *shell)
{
	int	count;
	int	i;

	i = -1;
	count = 0;
	while (++i < shell->argc)
	{
		if (!ft_memcmp(shell->argv[i], ">", 2) || \
			!ft_memcmp(shell->argv[i], ">>", 3))
		{
			i++;
			count++;
		}
	}
	return (count);
}

/*
to filter out redirection (>, >>) from the input and
create a new array without the (">" ">>")
update the shell->argc and shell->argv
Reduce the argument count in shell->argc based on the number of redirections
(each redirection operator corresponds to two arguments)
because each output redirection symbol (">" ">>") is followed by a file name,
and these pairs are removed from the argument count.
allocate memory for the filtered args with the size of adjusted argc.
loop through the original cmd and fill the new arguments array.
check if the current argument contains ">" or ">>"
if so, increment the index i by 2 to skip the file name that
follows the output redirection symbol.
If the current argument is not an output redirection symbol,
duplicate the argument using ft_strdup and assigns it to the new arguments
array (args).
wfree the memory allocated for the original arguments array.
update shell->argv to point to the newly created arguments array.
*/
char	**helper_redirection(t_mini *shell)
{
	char	**args;

	shell->argc -= count_redirection(shell) * 2;
	args = (char **)ft_calloc(sizeof(char *), shell->argc + 1);
	if (!args)
	{
		ft_free_2d_array(args);
		return (NULL);
	}
	return (args);
}

void	filter_redirection(t_mini *shell)
{
	int		i;
	int		j;
	char	**args;

	i = 0;
	j = 0;
	args = helper_redirection(shell);
	while (j < shell->argc)
	{
		if (!ft_memcmp(shell->argv[i], ">", 2)
			|| !ft_memcmp(shell->argv[i], ">>", 3))
			i += 2;
		else
		{
			args[j++] = ft_strdup(shell->argv[i++]);
			if (!args[j])
			{
				ft_free_2d_array(args);
				return ;
			}
		}
	}
	ft_free_2d_array(shell->argv);
	shell->argv = args;
}

/*
create file descriptor for processing the here document redirection
open a tmp file to write to the write end of the pipe
call handle here document and receive whatever it returns to the
read end side of the pipe
returns the read side of the pipe
*/
int	open_fd_heredoc(t_mini *shell)
{
	int	fd;

	fd = open(TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		ft_putstrs_fd(PMT1, TMP_FILE, NSFD, 2);
		return (-1);
	}
	fd = handle_heredoc(shell, fd);
	return (fd);
}

/*
setting up file descriptors for output redirection

loop through the argv  until a ">" or ">>" found
If the loop reaches the end of the argv and does not find ">" or ">>",
it means there is no output redirection specified,
and it returns the default file descriptor for standard output
If output redirection is found
call the out_file function to handle the redirection
the index i indicates the position of the redirection
in the argv, and the default file descriptor fd.
*/
int	init_fd(t_mini *shell)
{
	int	i;
	int	fd;

	i = 0;
	fd = 1;
	while (shell->argv[i] && ft_memcmp(shell->argv[i], ">", 2)
		&& ft_memcmp(shell->argv[i], ">>", 3))
	{
		i++;
	}
	if (!shell->argv[i])
		return (FAILURE);
	return (out_file(shell, i, fd));
}
