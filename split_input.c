/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:02:31 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/04/21 16:21:59 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_strlen_plus(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c && is_whitespace(s[i + 1]))
			break ;
		else if (s[i] == c && !is_whitespace(s[i + 1]))
		{
			while (!is_whitespace(s[i]))
				i++;
			return (i);
		}
		i++;
	}
	return (i);
}

static void	ft_strcpy_plus(char *src, char *dst, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (src[i])
	{
		if (src[i] == c && is_whitespace(src[i + 1]))
			break ;
		else if (src[i] == c && !is_whitespace(src[i + 1]))
		{
			while (!is_whitespace(src[i]))
			{
				dst[j] = src[i];
				i++;
				j++;
			}
			dst[0] = c;
			dst[j] = '\0';
			return ;
		}
		dst[j] = src[i];
		j++;
		i++;
	}
	dst[0] = c;
	dst[j + 1] = '\0';
	dst[j] = c;
}

static	char	*ft_strindup_quote(char *src, char c)
{
	char	*s;

	src++;
	s = (char *)malloc(ft_strlen_plus(src, c) + 3);
	if (!s)
		return (free(s), NULL);
	ft_strcpy_plus(src, s, c);
	return (s);
}

static	char	*ft_strindup(char *src, char c)
{
	int		i;
	char	*s;

	if (c == '\'' || c == '\"')
		return (ft_strindup_quote(src, c));
	i = 0;
	while (!is_whitespace(src[i]))
		i++;
	s = (char *)malloc(i + 1);
	if (!s)
		return (free(s), NULL);
	i = 0;
	while (!is_whitespace(src[i]))
	{
		s[i] = src[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

static	int	word_count(char	*s, char c)
{
	int		arr[3];
	char	tmp;

	if (s == NULL)
		return (-100);
	ft_bzero(arr, 12);
	tmp = c;
	while (s[arr[0]])
	{
		if (s[arr[0]] == c || (c == ' ' && is_whitespace(s[arr[0]])))
		{
			if (is_whitespace(c))
			{
				arr[1] = 0;
				c = tmp;
			}
			else if (is_quote(c) && is_whitespace(s[arr[0] + 1]))
			{
				arr[1] = 0;
				c = tmp;
			}
			else if (is_quote(c) && !is_whitespace(s[arr[0] + 1]))
				c = tmp;
		}
		else if (s[arr[0]] != c && arr[1] == 0)
		{
			if (s[arr[0]] == '\'' || s[arr[0]] == '\"')
				c = s[arr[0]];
			arr[1] = 1;
			arr[2]++;
		}
		arr[0]++;
	}
	return (arr[2]);
}

static void	*free_all(char **arr)
{
	int		i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr[i]);
	free(arr);
	return (NULL);
}

static void	setvars(int arr[3], char *tmp, char *c, char idk)
{
	if (idk == 0)
	{
		arr[0] = 0;
		arr[1] = 0;
		arr[2] = 0;
		*tmp = *c;
	}
	else if (idk == 1)
	{
		arr[2] = 0;
		*c = *tmp;
	}
}

// iterators[0] : i
// iterators[1] : words counter
// iterators[2] : 1 found a word, 0 found a space

char	**ft_split_input(char const *s, char c)
{
	int		iterators[3];
	char	**arr;
	char	tmp;

	setvars(iterators, &tmp, &c, 0);
	arr = (char **)malloc((word_count((char *)s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	while (s[iterators[0]])
	{
		if (s[iterators[0]] == c
			|| (c == ' ' && is_whitespace(s[iterators[0]])))
		{
			if (is_whitespace(c))
				setvars(iterators, &tmp, &c, 1);
			else if (is_quote(c) && is_whitespace(s[iterators[0] + 1]))
				setvars(iterators, &tmp, &c, 1);
			else if (is_quote(c) && !is_whitespace(s[iterators[0] + 1]))
				c = tmp;
		}
		else if (s[iterators[0]] != c && iterators[2] == 0)
		{
			if (s[iterators[0]] == '\'' || s[iterators[0]] == '\"')
				c = s[iterators[0]];
			arr[iterators[1]] = ft_strindup((char *)(s + iterators[0]), c);
			if (arr[iterators[1]] == NULL)
				return (free_all(arr));
			iterators[2] = 1;
			iterators[1]++;
		}
		iterators[0]++;
	}
	return (arr[iterators[1]] = NULL, arr);
}
