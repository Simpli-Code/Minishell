/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:06:49 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/31 20:49:24 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
handling the export cmd by updating the env variables based on the arguments
if the specified variable is not found, creates a copy of the current env
variables and adds the new variable.
if the variable is found, it updates the existing variable
the function returns a pointer to the updated env variables.

iterate through the current env variables to find a match for the
specified variable (shell->argv[j]). if the specified variable is not found
in the current env variables: create a copy of the current env
variables using the ft_cpy_environ function. strjoin to concatenates the
variable and its value into a single string and
assigns it to the corresponding position in the copied env variables.
free the memory if the specified variable is found in the current env variables:
assign the pointer to the current env variables to cpy.
free the memory

concatenates the variable and its value into a single string and assigns
it to the corresponding position in the current env variables.
return the pointer to the updated env variables.
*/
static char	**copy_and_extend_env(t_mini *shell, int size)
{
	char	**cpy;

	cpy = ft_cpy_environ(shell->envp, size);
	if (!cpy)
	{
		ft_free_2d_array(cpy);
		return (NULL);
	}
	return (cpy);
}

static char	**create_and_extend_env(t_mini *shell, int i, int j)
{
	char	**cpy;

	cpy = copy_and_extend_env(shell, i + 1);
	if (!cpy)
		return (NULL);
	cpy[i] = ft_strjoin(shell->argv[j], shell->argv[j + 1]);
	if (!cpy[i])
	{
		ft_free_2d_array(cpy);
		return (NULL);
	}
	ft_free_2d_array(shell->envp);
	return (cpy);
}

static char	**update_env_entry(t_mini *shell, int i, int j)
{
	free(shell->envp[i]);
	shell->envp[i] = ft_strjoin(shell->argv[j], shell->argv[j + 1]);
	if (!shell->envp[i])
	{
		ft_free_2d_array(shell->envp);
		return (NULL);
	}
	return (shell->envp);
}

char	**export_cmd(t_mini *shell, int j)
{
	int	i;

	i = 0;
	while (shell->envp[i] && ft_memcmp(shell->envp[i], shell->argv[j],
			ft_strlen(shell->argv[j])))
		i++;
	if (!shell->envp[i])
	{
		return (create_and_extend_env(shell, i, j));
	}
	else
	{
		return (update_env_entry(shell, i, j));
	}
}
