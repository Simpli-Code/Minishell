/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 20:08:59 by chruhin           #+#    #+#             */
/*   Updated: 2024/03/28 10:22:34 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
