/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 22:25:34 by chruhin           #+#    #+#             */
/*   Updated: 2023/12/03 10:39:10 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H_
# define TYPES_H_

# include "minishell.h"
# include <sys/types.h>

# define PROMPT		"\033[1;32mMINISHELL:~$ \033[0;0m"

extern	__sig_atomic_t	g_var;


typedef struct	s_data
{
	int		argc;
	char	**argv;
	char	**envp;
	char	**export;
	int		ret;
	char	*str;
	char	**cmds;
	int		child;
	int		state;
}	t_data;

#endif
