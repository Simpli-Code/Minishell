/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 20:32:38 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/24 11:35:33 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_state;

/*
extract and set each args from the input string into an array
skip white spaces
loop until the end of argc
calculate the len of each elements
duplicate each elements and save it in argv array
the ft_remove_token to check '' "" and \ and removes it
increament the i value
increament the pointer str by the len of that character
*/
int	fill_array(t_mini *shell, char *input)
{
	int	i;
	int	len;

	i = 0;
	ft_skip_spaces(&input);
	while (i < shell->argc)
	{
		ft_skip_spaces(&input);
		len = ft_strlen_operator_token(input);
		shell->argv[i] = ft_strldup(input, len);
		if (!shell->argv[i])
		{
			ft_free_2d_array(shell->argv);
			return (FAILURE);
		}
		ft_remove_token(&(shell->argv[i]));
		i++;
		input += len;
	}
	return (SUCCESS);
}

/*
check if a dollar sign for env expansion
count the number of cmds
malloc to store the array of cmds
fill the array with each cmds
check for cmds for pipe
*/
int	process_input(t_mini *shell)
{
	int	i;

	i = 0;
	shell->argv = NULL;
	if (!is_dollar_sign(shell))
		if (check_env(shell, i) != SUCCESS)
			return (FAILURE);
	shell->argc = ft_count_commands(shell->input);
	shell->argv = (char **)ft_calloc(sizeof(char *), (shell->argc + 1));
	if (!shell->argv)
	{
		ft_free_2d_array(shell->argv);
		return (FAILURE);
	}
	if (fill_array(shell, shell->input) != SUCCESS)
		return (FAILURE);
	is_cmd_or_pipe(shell);
	return (SUCCESS);
}

/*
To Initialize and malloc the shell structure
*shell	a pointer to t_mini
envp	array of strings containing environment variables.
		it calls ft_cpy_environ func to create a copy of envp
export	malloc for an array of strings
argv	array of strings representing cmd line arguments
e_status		initialize e_status to 0
input		initialize input to 0 indicating empty input
child	initialize to 0, to track chile proccesses
ret_len	array of integers with 2 elem. (return values)
*/
static int	init_shell(t_mini *shell, char **envp)
{
	shell->export = (char **)ft_calloc(sizeof(char *), 1);
	if (!shell->export)
		return (FAILURE);
	shell->envp = ft_cpy_environ(envp, 0);
	if (!shell || (shell && !shell->envp && shell->export))
	{
		ft_free_2d_array(shell->export);
		return (FAILURE);
	}
	shell->argc = 0;
	shell->child = 0;
	shell->fd_in = 0;
	shell->fd_out = 0;
	shell->braces = 0;
	shell->argv = NULL;
	shell->input = NULL;
	shell->e_status = 0;
	shell->eof = NULL;
	shell->buf_size = BUF_SIZE;
	return (SUCCESS);
}

int	lunch_minishell(t_mini *shell)
{
	while (42)
	{
		setup_signals(shell);
		shell->input = readline(GRN PMT RST);
		if (!shell || shell->input == NULL)
		{
			ft_putstr_fd("exit\n", 2);
			free_shell(shell);
			exit(0);
		}
		if (shell && shell->input[0] != '\0')
		{
			add_history(shell->input);
			if (is_input_valid(shell) != SUCCESS)
				continue ;
			if (is_invalid_cmd(shell, shell->input) != SUCCESS)
				continue ;
			if (process_input(shell) != SUCCESS)
				continue ;
		}
		if (shell)
			free_loop(shell);
	}
	return (SUCCESS);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_mini	shell;

	if (argc == 1)
	{
		if (init_shell(&shell, envp) == SUCCESS)
		{
			if (lunch_minishell(&shell) != SUCCESS)
			{
				free_shell(&shell);
				return (FAILURE);
			}
		}
		else
			return (FAILURE);
		rl_clear_history();
	}
	else
	{
		ft_putstrs_fd(PMT1, argv[1], NSFD, 2);
		return (127);
	}
	free_shell(&shell);
	return (SUCCESS);
}
