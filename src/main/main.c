/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 20:32:38 by chruhin           #+#    #+#             */
/*   Updated: 2023/12/03 10:39:53 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

__sig_atomic_t	g_var;


/*
	If more than one argument return
	parameters:

	data;		a pointer to data struct
	argv_zero;	first argument of argv
*/
int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	char	*line;

	if (argc != 1)
		return (1);

	setup_params(&data, argv, envp);
	while (1)
	{
		setup_signals(1);
		if (isatty(0))
			line = readline(PROMPT);
		if (line == NULL)
		{
			ft_putstr_fd("exit\n", 2);
			break ;
		}
		else
		{
			ft_addstr(&(data->str), line);
			//parser(data);
			add_history(line);
			free(line);
		}
	}
	return (0);
}


/*
	To Initialize and malloc the data structure
	*data	a pointer to t_data
	envp	array of strings containing environment variables.
			it calls copy_env func to create a copy of envp
	export	malloc for an array of strings
	argv	array of strings representing cmd line arguments
	ret		initialize ret to 0
	str		initialize str to 0 indicating empty str
	child	initialize to 0, to track chile proccesses
*/
void	setup_params(t_data **data, char **argv, char **envp)
{
	(*data) = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return ;
	(*data)->export = (char **)ft_calloc(sizeof(char *), 1);
	(*data)->argv = argv;
	(*data)->ret = 0;
	(*data)->str = 0;
	(*data)->child = 0;
}
