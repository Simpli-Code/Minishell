/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 19:14:06 by chruhin           #+#    #+#             */
/*   Updated: 2023/12/03 10:37:28 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _UTILS_H_
# define _UTILS_H_

# include "minishell.h"
# include "types.h"
# include <fcntl.h>
# include <stdlib.h>
# include <stddef.h>
# include <unistd.h>
# define BUFFER_SIZE 4096

char	*ft_itoa(int n);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isdigit(int c);
int		ft_isspace(int c);
void	ft_skip_spaces(char **str);
void	free_matrix(char **matrix);
int		ft_putstrs_fd(char *before, char *str, char *after, int fd);
char	*ft_strldup(const char *s, int len);
char	**ft_split_case(char *s, char c);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strdup(const char *s);
void	ft_addchr(char **str, char c);
int		ft_atoi(const char *nptr);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_putstr_fd(char *s, int fd);
void	ft_putstrlen_fd(char *s, int len, int fd);
void	ft_putchar_fd(char c, int fd);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
int		get_next_line(int fd, char **line);
char	*get_next_line1(int fd);
void	ft_strdel(char **str);
void	ft_addstr(char **str, const char *addition);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_split_first_line_and_rest(char *s, char **rest);

#endif
