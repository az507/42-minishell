/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:51:31 by achak             #+#    #+#             */
/*   Updated: 2024/02/22 12:51:14 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_length(char const *s, char c, int j)
{
	size_t	length;

	length = 0;
	while (s[j] != c && s[j])
	{
		length++;
		j++;
	}
	return (length);
}

static size_t	word_count(char const *s, char c)
{
	size_t	words;

	words = 0;
	while (*s)
	{
		if (*s != c && (*(s + 1) == c || *(s + 1) == '\0'))
			words++;
		s++;
	}
	return (words);
}

static void	free_arr(char **arr, int i)
{
	while (i-- > 0)
		free(arr[i]);
	free(arr);
}

static char	**word_split(char **arr, char const *s, char c)
{
	size_t	i;
	size_t	size;
	int		j;

	i = -1;
	j = 0;
	while (++i < word_count(s, c))
	{
		while (s[j] == c)
			j++;
		size = word_length(s, c, j);
		arr[i] = ft_substr(s, j, size);
		if (arr[i] == NULL)
		{
			free_arr(arr, i);
			return (NULL);
		}
		j += size;
	}
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	size_t	words;

	if (!*s)
		return (NULL);
	words = word_count(s, c);
	arr = malloc(sizeof(char *) * (words + 1));
	if (arr == NULL)
		return (NULL);
	arr = word_split(arr, s, c);
	if (arr == NULL)
		return (NULL);
	arr[words] = 0;
	return (arr);
}

/*#include <stdio.h>

int	main(void)
{
	char const *s = "AFIAFIEFINDIHD";
	char c = 'A';
	char **arr = ft_split(s, c);

	printf("%s\n", arr[0]);
	printf("%s\n", arr[1]);
	printf("%s\n", arr[2]);
	printf("%s\n", arr[3]);
	free(arr);
}*/
