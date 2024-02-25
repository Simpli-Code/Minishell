/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 10:33:06 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 17:25:40 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
handle errors related to exporting variables
checks if the provided argument is a valid identifier
and print an error message if it's not
loop through the characters of the current argument,
check if each character is an alphanumeric character,
underscore, hash symbol, equal sign, or dollar sign.
if the first character of the current argument is a digit
or if the identifier is not empty
if the current argument contains the equals sign ('='),
print an error message indicating that it'str not a valid identifier,
and increments i to move to the next argument.
else if the current argument does not contain the equals sign:
print an error and specific part of the argument causing the error
*/
int	handle_export_error(char **argv, int *i)
{
	int	j;

	j = 0;
	while (ft_isalnum(argv[*i][j]) || argv[*i][j] == '_' ||
			argv[*i][j] == '#' || argv[*i][j] == '$' || argv[*i][j] == '=')
		j++;
	if (argv[*i][j] && argv[*i][j] != ' ')
	{
		if (ft_strchr(argv[*i], '='))
		{
			ft_putstrs_fd(PMT1, argv[0], ": `", 2);
			ft_putstrs_fd(argv[*i], argv[(*i) + 1], INVLD, 2);
			(*i)++;
		}
		else
		{
			ft_putstrs_fd(PMT1, argv[0], ": `", 2);
			ft_putstrs_fd(argv[*i], INVLD, 0, 2);
		}
		(*i)++;
		return (FAILURE);
	}
	return (SUCCESS);
}

void	print_error_export(t_mini *shell, int *i)
{
	ft_putstrs_fd(PMT1, shell->argv[0], ": `", 2);
	ft_putstrs_fd(shell->argv[*i], INVLD, 0, 2);
}

/*
creates a new array by excluding the entry at the specified index
from the original array.
The original array is then freed, and the new array is returned.
calculates the length of the original array by looping
allocate memory for the new array cpy using calloc copie the entries
from the original array to the new array up tothe specified index i.
skip the entry at the specified index i and continue copying the
remaining entries from the original array to the new array.
free the memory occupied by the original array.
return the new array with the specified entry removed.
*/
static char	**delete_env(char **envp, int i)
{
	int		j;
	int		len;
	char	**cpy;

	len = 0;
	while (envp[len])
		len++;
	cpy = (char **)ft_calloc(sizeof(char *), len);
	if (!cpy)
		return (NULL);
	j = -1;
	while (++j < i)
	{
		cpy[j] = ft_strdup(envp[j]);
		if (!cpy[j])
			return (NULL);
	}
	i++;
	while (envp[i])
	{
		cpy[j++] = ft_strdup(envp[i++]);
	}
	ft_free_2d_array(envp);
	return (cpy);
}

/*
removes the specified env from the original
env array and returns the modified array.
If the specified env is not found,
it returns the original array unchanged.
If fewer than two arguments
(meaning it lacks the env to unset),
return the original env array.
calculate the length of the specified env and create a string
by concatenating the specified env with an equal sign.
searche for the specified env in the original env array.
if (shell->envp[i])
cpy = delete_env(shell->envp, i);
If the specified env is found,
create a new array (cpy) by removing the entry at the founded index.
If not found, it returns the original env array.
free the memory occupied by the string env
and return the new modified env array.
*/
char	**handle_unset(t_mini *shell, int idx)
{
	int		i;
	int		len;
	char	*env;
	char	**cpy;

	if (shell->argc < 2)
		return (shell->envp);
	len = strlen(shell->argv[idx]);
	env = ft_strjoin(shell->argv[idx], "=");
	if (!env)
		return (NULL);
	i = 0;
	while (shell->envp[i] && ft_memcmp(env, shell->envp[i], len + 1))
		i++;
	if (shell->envp[i])
	{
		cpy = delete_env(shell->envp, i);
		if (!cpy)
			return (NULL);
	}
	else
		cpy = shell->envp;
	free(env);
	return (cpy);
}
