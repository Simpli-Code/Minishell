/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 19:14:06 by chruhin           #+#    #+#             */
/*   Updated: 2024/02/05 23:39:39 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"
# include "types.h"
# include <fcntl.h>
# include <stdlib.h>
# include <stddef.h>
# include <unistd.h>

int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isdigit(int c);
int		ft_isspace(int c);
int		ft_is_printable(int c);
int		ft_count_char(char *str, char c);

int		ft_atoi(const char *nptr);
char	*ft_itoa(int n);
char	**ft_split(char *str, char c);

int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strldup(char *str, int len);
char	*ft_strdup(const char *str);

int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strchr(const char *str, int c);
char	*ft_strjoin(char const *s1, char const *s2);

void	ft_putstr_fd(char *str, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *str, int fd);
void	ft_putstrlen_fd(char *str, int len, int fd);
int		ft_putstrs_fd(char *before, char *str, char *after, int fd);
int		ft_putstrs_fd(char *before, char *str, char *after, int fd);

size_t	ft_strlen(const char *str);
int		ft_strlen_char(char *str, char c);
int		ft_strlen_token(char *str);
int		ft_strlen_pipe(char *str);
int		ft_strlen_env(char *str);
int		ft_strlen_arg_token(char *str, char delimiter);
int		ft_strlen_operator_token(char *str);

void	ft_remove_char(char **str, int j);
void	ft_remove_token(char **arg);
void	ft_skip_spaces(char **str);
int		ft_is_escape_token(char c);
char	**ft_cpy_environ(char **envp, int add);

int		ft_count_commands(char *str);
void	ft_free_2d_array(char **array);
int		ft_strchr_idx(const char *s, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		find_env_var_index(char *var, char **envp);

#endif
