/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 12:15:16 by chruhin           #+#    #+#             */
/*   Updated: 2023/12/03 10:43:47 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MINISHELL_H_
# define _MINISHELL_H_

# include "utils.h"
# include "types.h"
# include <stdio.h>
# include <fcntl.h>
# include <dirent.h>
# include <signal.h>
# include <errno.h>
# include <string.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <readline/history.h>
# include <readline/readline.h>



void	setup_signals(int token);
void	handle_sig_child(int signum);
void	setup_params(t_data **data, char *argv[], char *envp[]);

#endif
