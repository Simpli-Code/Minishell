/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:08:59 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/06 07:44:21 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
printing env variables to a file descriptor (fd)
including the variable value enclosed in double quotes

loop through the env variables.
write the string "declare -x " to the specified file descriptor.
write a substring of env_var[i] up to the position of
the equals sign ('=') plus one
check if the current env variable contains an equals sign ('=')
if true, write the value of the env variable
(substring after the equals sign) in double quotes.
write a newline character
indicating the end of the current env variable
free the memory
*/
static void	put_env_to_fd(char **env_var, int fd)
{
	int	i;

	i = -1;
	while (env_var[++i])
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstrlen_fd(env_var[i], ft_strlen_char(env_var[i], '=') + 1, fd);
		if (ft_strchr(env_var[i], '='))
			ft_putstrs_fd("\"", ft_strchr(env_var[i], '=') + 1, "\"", fd);
		ft_putstr_fd("\n", fd);
	}
}

/*
it sorts the env variables based on the delimiter c
create a copy of the original env variables
iterate through the env_var array
calculate the len up to the first occurrence of delimiter c in the current
and next env variables determine the len based on the shorter of the two
compare the substrings of the current and next env variables up to the len
if the comparison is greater than 0
it means the current variable comes after the next variable lexicographically
swap the positions of the current and next env variables
in the env_var array, resetting the index i to -1 to start the comparison again.
the put_env_to_fd puts the sorted env variables to the specified file descriptor
*/
static void	helper_sort(char **env_var, int fd, char c)
{
	int		i;
	int		len;
	char	*tmp;

	i = 0;
	while (env_var[i] && env_var[i + 1])
	{
		len = ft_strlen_char(env_var[i], c);
		if (len > ft_strlen_char(env_var[i + 1], c))
			len = ft_strlen_char(env_var[i], c);
		else
			len = ft_strlen_char(env_var[i + 1], c);
		if (ft_memcmp(env_var[i], env_var[i + 1], len) > 0)
		{
			tmp = env_var[i];
			env_var[i] = env_var[i + 1];
			env_var[i + 1] = tmp;
			i = -1;
		}
		i++;
	}
	put_env_to_fd(env_var, fd);
	ft_free_2d_array(env_var);
}

void	sort_env(char **envp, int fd, char c)
{
	char	**env_var;

	if (envp == NULL)
		return ;
	env_var = ft_cpy_environ(envp, 0);
	if (!env_var)
	{
		ft_free_2d_array(env_var);
		return ;
	}
	helper_sort(env_var, fd, c);
}

void	helper_sort_env(t_mini *shell, int fd)
{
	sort_env(shell->envp, fd, '=');
	sort_env(shell->export, fd, 0);
}

/*
It is responsible to processes env variables
Check if the cmd is "export" and has no arguments (argc == 1 and empty argv[1])
It sort the env variables shell->envp and exports shell->export by '='
and 0 criteria loop through each element of argv "i = 1"
starting from second argv handle_export_error checks if there's an error
while proccessing the current argv if error increament 'shell->e_status'
else if the cmd is export (shell->argv[0]) start with export value
else if the command is unset start with unset, to remove an env calling
handle_unset and increament i value
after proccessing all arguments, if error set shell->e_status to 1, else to 0
and return the updated env variables
*/
char	**export_unset(t_mini *shell, int fd)
{
	int	i;

	shell->e_status = 0;
	if (!ft_memcmp(shell->argv[0], "export", 7) && shell->argc == 1
		&& !ft_strlen(shell->argv[1]))
		helper_sort_env(shell, fd);
	i = 1;
	while (shell->argv[i])
	{
		if (handle_export_error(shell->argv, &i) == SUCCESS)
		{
			if (!ft_memcmp(shell->argv[0], "export", 7))
			{
				if (!export_value(shell, &i))
				{
					print_error_export(shell, &i);
					shell->e_status = 0;
				}
				break ;
			}
			else if (!ft_memcmp(shell->argv[0], "unset", 6))
				shell->envp = handle_unset(shell, i++);
		}
	}
	return (shell->envp);
}
