/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 14:12:11 by cha               #+#    #+#             */
/*   Updated: 2023/11/29 15:19:32 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*get_next_line1(int fd)
{
	char		*buffer;
	size_t		bytes_read;
	static char	*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (line)
			free(line);
		line = NULL;
		return (NULL);
	}
	buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while ((ft_strchr(line, '\n') == NULL) && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		line = ft_strjoin(line, buffer);
	}
	free(buffer);
	return (ft_split_first_line_and_rest(line, &line));
}

char	*ft_split_first_line_and_rest(char *s, char **rest)
{
	size_t	i;
	char	*first_line;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] != '\n' && s[i])
		i++;
	first_line = ft_substr(s, 0, i + 1);
	if (rest)
	{
		if (s[i] == '\n' && s[i + 1])
			*rest = ft_substr(s, i + 1, ft_strlen(s) - i - 1);
		else
			*rest = NULL;
	}
	free(s);
	return (first_line);
}
