/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 12:15:16 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/06 08:05:34 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "utils.h"
# include "types.h"
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

//		ERROR HANDLING
int		syntax_error(t_mini *shell, int err_flag, char c);
int		check_redirection(t_mini *shell, char *input, int *pos, char symbol);
int		check_pipe(t_mini *shell, char *input, int *pos);
int		check_quotes(t_mini *shell, char *quote_start, int *pos, char target);
int		is_input_valid(t_mini *shell);
void	free_loop(t_mini *shell);
void	free_shell(t_mini *shell);

//		SIGNAL HANDLING
void	setup_signals(t_mini *shell);
void	handle_sig_parent(int signal_code);
void	handle_sig_child(int signum);
void	handle_sig_heredoc(int signum);

//		ENVIRON HANDLING
int		is_open_braces(char **str);
int		check_braces(t_mini *shell, char **str, int *i, int j);
char	*dup_cmd(char **str, int *i);
char	*dup_value(t_mini *shell, char **str, int *len, int *i);
void	sort_env(char **envp, int fd, char c);
int		modify_env(t_mini *shell, int i);
int		export_value(t_mini *shell, int *i);
char	**export_unset(t_mini *shell, int fd);
char	**export_cmd(t_mini *shell, int i);
int		export_value(t_mini *shell, int *i);
int		handle_export_error(char **argv, int *i);
int		check_env(t_mini *shell, int i);
char	**ft_cpy_argvs(t_mini *shell);
int		is_dollar_sign(t_mini *shell);
void	print_error_export(t_mini *shell, int *i);

//		PATH HANDLING
char	*get_env_path(char **envp, char *env);
char	*filter_path(t_mini *shell, char *str, DIR **dir, struct dirent **d);
void	handle_path_type(t_mini *shell, char *str, char *path);
void	modify_path(char *str, char **path);

//		COMMAND HANDLING
int		process_input(t_mini *shell);
char	**handle_commands(t_mini *shell, char *str);
void	is_cmd_or_pipe(t_mini *shell);
int		handle_cmd_w_fork(t_mini *shell);
int		is_executable(t_mini *shell, int fd);
char	**execute_commands(t_mini *shell);
void	set_pipe_args(t_mini *shell, int idx);
void	swap_pipes(int *fds);
int		execute_pipes(t_mini *shell, int *fds);
int		is_invalid_cmd(t_mini *shell, char	*str);

//		BUILTINS
int		is_builtins(t_mini *shell, int fd);
void	handle_exit(t_mini *shell);
char	**handle_unset(t_mini *shell, int j);
void	handle_cd(t_mini *shell);
void	handle_echo(t_mini *shell, int fd);

//		REDIRECTIONS
void	set_delimiter(t_mini *shell);
char	*ft_getenv_heredoc(char *var, char **envp);
int		handle_infile(t_mini *shell);
int		handle_heredoc(t_mini *shell, int fd);
void	handle_env_heredoc(t_mini *shell, char *str, int *i, int fd);
int		out_file(t_mini *shell, int idx, int fd);
void	filter_redirection(t_mini *shell);
int		init_fd(t_mini *shell);
int		is_heredoc_sign(t_mini *shell);
int		is_infile_sign(t_mini *shell);
int		exec_heredocument(t_mini *shell, int fd, char *path);
int		open_fd_heredoc(t_mini *shell);

#endif
