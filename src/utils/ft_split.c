/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 18:16:26 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/26 17:40:10 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
count how many 'c' in *s.
s[0] index for *s
s[1] counter for 'c'
if '\' and n[0]+1 ' or " or \ or c , skip
else if c start counting
else if ' or "
	remember where the quote starts
	loop until the end of s or found the next quote
	continue looping until the matching quote is found
	take into account escaped characters within the quotes
*/
static int	count_substrings(char *str, char c)
{
	int		n[2];
	char	quote;

	n[0] = -1;
	n[1] = 1;
	while (str[++n[0]])
	{
		if (str[n[0]] == '\\' && (str[n[0] + 1] == '\'' || str[n[0] + 1] == '"'
				|| str[n[0] + 1] == '\\' || str[n[0] + 1] == c))
			n[0]++;
		else if (str[n[0]] == c)
			n[1]++;
		else if (str[n[0]] && (str[n[0]] == '"' || str[n[0]] == '\''))
		{
			quote = str[n[0]++];
			while (str[n[0]] && str[n[0]] != quote)
			{
				if (str[n[0]] == '\\' && (str[n[0] + 1] == quote
						|| str[n[0] + 1] == '\\') && quote == '"')
					n[0]++;
				n[0]++;
			}
		}
	}
	return (n[1]);
}
/*
count the len of each substring until the delimeter c
loop through *s until c or end of *s
if current len if \ and len+1 is either ' or " or \ or c, skip len++
else if " or ' count the quote
loop until matching quote is found, skip escaped characters
*/

static int	get_substring_len(char *str, char c)
{
	char	quote;
	int		len;

	len = -1;
	while (str[++len] && str[len] != c)
	{
		if (str[len] == '\\' && (str[len + 1] == '\'' || str[len + 1] == '"'
				|| str[len + 1] == '\\' || str[len + 1] == c))
			len++;
		else if (str[len] == '"' || str[len] == '\'')
		{
			quote = str[len++];
			while (str[len] && str[len] != quote)
			{
				if (str[len] == '\\' && (str[len + 1] == quote
						|| str[len + 1] == '\\') && quote == '"')
					len++;
				len++;
			}
		}
	}
	return (len);
}

/*
fill the array with substrings
from the original string *s gets the length of each substrings
and duplicate it and save it into array
*/
static char	**populate_array(char **array, char *str, char c, int n)
{
	int		i;
	size_t	len;

	i = 0;
	if (array == NULL || str == NULL)
		return (NULL);
	while (i < n && str != NULL && *str != '\0')
	{
		len = get_substring_len(str, c);
		array[i] = ft_strldup(str, len);
		if (!array[i])
		{
			ft_free_2d_array(array);
			return (NULL);
		}
		str += len + 1;
		i++;
	}
	return (array);
}

char	**ft_split(char *str, char c)
{
	int		len;
	char	**array;

	if (!str)
		return (NULL);
	len = count_substrings(str, c);
	array = (char **)ft_calloc(sizeof(char *), len + 1);
	if (!array)
		return (NULL);
	array = populate_array(array, str, c, len);
	if (!array)
	{
		ft_free_2d_array(array);
		return (NULL);
	}
	return (array);
}
