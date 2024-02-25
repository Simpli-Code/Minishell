/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:29:01 by chruhin           #+#    #+#             */
/*   Updated: 2024/01/25 18:22:14 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
removes a character at the specified index of a string
and creates a substring before and after the specified index,
concatenate them, and update the original string.

before = ft_strldup(*str, j);
create a substring before containing the characters before the specified
index j by duplicating the portion of the original string.

after = ft_strdup(*str + j + 1);
create a substring after containing the character
after the specified index j + 1
by duplicating the portion of the original string.

free(*str);
*str = ft_strjoin(before, after);
free the memory occupied by the original string,
and then concatenates the substrings
before and after to form the modified string,
updating the double pointer str.

free the memory occupied by the substrings after and before.
*/
void	ft_remove_char(char **str, int j)
{
	char	*before;
	char	*after;

	before = ft_strldup(*str, j);
	after = ft_strdup(*str + j + 1);
	free(*str);
	*str = ft_strjoin(before, after);
	free(after);
	free(before);
}
