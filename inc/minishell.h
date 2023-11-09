/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 12:15:16 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/08 13:56:12 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MINISHELL_H_
# define _MINISHELL_H_

# include "utils.h"
# include <stdio.h>
# include <fcntl.h>
# include <dirent.h>
# include <signal.h>
# include <errno.h>
# include <string.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <readline/history.h>
# include <readline/readline.h>

# define PROMPT			"\033[1;32mMINISHELL:~$ \033[0;0m"

//typedef enum s_code
//{
//	SIG_IGNORE,
//	SIG_STD,
//	SIG_HEREDOC,
//}	t_code;


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
}	t_data;

void	set_args(char **argv, char *str, int argc);
void	child_sig_handler(int sig);
void	child_sig_handler_bash(int sig);
void	bash_command(t_data *param);
void	cd_command(t_data *param);
void	pwd_command(int fd);
void	exit_command(t_data *param);
void	parser(t_data *param);
void	export_value(t_data *param, int *i);
void	sort_envp(char **envp, int fd, char c);
void	rm_char(char **str, int j);
void	rm_token(char **arg);
void	command_or_pipe(t_data *param, int j);
char	*get_env(char **envp, char *env);
char	**copy_env(char **envp, int add);
char	**export_command(t_data *param, int i);
char	**unset_command(t_data *param, int i);
char	**copy_args(t_data *param);
char	**check_command(char *str, t_data *param);
int		check_builtins(int fd, t_data *param);
int		check_bin(int fd, t_data *param);
int		count_args(char *str);
int		ft_strlen_token(char *str);
int		ft_strlen_env(char *str);
int		ft_strlen_pipe(char *str);
int		ft_strlen_char(char *str, char c);
int		ft_strlen_arg_token(char *str, char c);
int		check_export_error(char **argv, int *i);
int		is_token(char c);
// void	setup_signal(int signum, int code);

#endif
