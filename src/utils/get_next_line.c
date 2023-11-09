/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 18:08:40 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/06 19:05:15 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static char	*ft_strjoin1(char *s, char c)
{
	int		i;
	char	*str;

	i = 0;
	while (s[i])
		i++;
	if (!(str = (char *)malloc(i + 2)))
		return (0);
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = c;
	str[i + 1] = '\0';
	free(s);
	return (str);
}

int			get_next_line(int fd, char **line)
{
	char	*buffer;
	int		flag;

	buffer = (char *)malloc(2);
	if (!line || !(*line = (char *)malloc(1)) || !buffer)
		return (-1);
	*line[0] = '\0';
	while ((flag = read(fd, buffer, 1)) > 0)
	{
		if (buffer[0] == '\n')
			break ;
		*line = ft_strjoin1(*line, buffer[0]);
	}
	free(buffer);
	return (flag);
}
