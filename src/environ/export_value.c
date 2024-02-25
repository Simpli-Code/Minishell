/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 10:15:31 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/31 20:48:24 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
to handle the exporting of values, check whether the argument contains
an equal sign (=). If it does not, it adds the argument to the export array.
If it contains an equal sign, the export_cmd function
and updates the envp accordingly
if the current argument does not contain the '='
it'str a variable without a value.

loop through the existing export variables to find a
match for the specified variable (shell->argv[*i])

if the specified variable is not found in the existing export variables
create a copy of the existing export variables using
the ft_cpy_environ. duplicate the specified variable and assigns it to the
corresponding position in the copied export variables.
set the next position to NULL to mark the end of the array.
free the memory
assign the pointer to the copied export variables to shell->export.
increments the index i.

if the current argument contains the '=' it's a variable with a value
the export_cmd will handle the export cmd for the specified variable
and its value. and update the environment variables. increment the
index i based on whether there is a value after the current
argument (increments by 2 if a value is present, otherwise by 1).
*/
void	handle_export_command(t_mini *shell, int *i)
{
	shell->envp = export_cmd(shell, *i);
	if (!shell->envp)
		return ;
	if (shell->argv[*i + 1])
		*i = 2;
	else
		*i = 1;
}

int	export_value(t_mini *shell, int *i)
{
	char	**after;
	int		j;

	if (!ft_strchr(shell->argv[*i], '='))
	{
		j = 0;
		(*i)++;
		while (shell->export[j] && ft_memcmp(shell->export[j], shell
				-> argv[*i], ft_strlen(shell->argv[*i])))
			j++;
		if (!shell->export[j])
		{
			after = ft_cpy_environ(shell->export, 1);
			after[j] = ft_strdup(shell->argv[*i]);
			if (!after[j])
				return (FAILURE);
			after[j + 1] = 0;
			ft_free_2d_array(shell->export);
			shell->export = after;
		}
		(*i)++;
	}
	else
		return (handle_export_command(shell, i), FAILURE);
	return (SUCCESS);
}
