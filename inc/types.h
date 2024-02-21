/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 22:25:34 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/06 15:12:49 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include "minishell.h"
# include "utils.h"
# include <stdbool.h>
# include <sys/types.h>

extern int	g_state;
# define TMP_FILE		"/tmp/heredoc.tmp"
# define PMT			"MINISHELL:~$ "
# define PMT1			"bash:  "
# define INVLD			"': not a valid identifier\n"
# define NSFD			": No such file or directory\n"
# define BAD			": bad substitution\n"
# define BUF_SIZE 		4096
# define SUCCESS		0
# define FAILURE		1
# define SYN_ERR		2
# define QUOTE			3
# define NL				4
# define TOKEN			5

typedef struct s_mini
{
	char				*input;
	char				**argv;
	char				**envp;
	char				**export;
	char				*eof;
	int					argc;
	int					child;
	int					braces;
	int					fd_in;
	int					fd_out;
	int					buf_size;
	int					e_status;
	bool				sigint_child;
	bool				sigint_heredoc;
}	t_mini;

#endif
