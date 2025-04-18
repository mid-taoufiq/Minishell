/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:17:34 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/04/16 14:39:52 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quote(char c, int q)
{
	if (c == '\'' || c == '\"')
	{
		if (q == 1)
			return (0);
		if (q == 0)
			return (1);
	}
	return (q);
}

static	char	*ft_strindup(char *src, char c)
{
	int		i;
	char	*s;
	int		q;

	i = 0;
	while (src[i] != '\0' && src[i] != c)
		i++;
	s = (char *)malloc(i + 1);
	if (!s)
	{
		free(s);
		return (NULL);
	}
	i = 0;
	q = 0;
	while (src[i] != '\0')
	{
		if (src[i] == c && q == 0)
			break ;
		q = check_quote(src[i], q);
		s[i] = src[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

static	int	word_count(char	*s, char c)
{
	int		i;
	int		count;
	int		f;
	int		q;

	if (s == NULL)
		return (-100);
	f = 0;
	i = 0;
	q = 0;
	count = 0;
	while (s[i])
	{
		q = check_quote(s[i], q);
		if (s[i] == c && q == 0)
			f = 0;
		else if (s[i] != c && f == 0)
		{
			f = 1;
			count++;
		}
		i++;
	}
	return (count);
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

static void	setzero(int *i, int *j, char *f)
{
	*i = 0;
	*j = 0;
	*f = 0;
}

char	**ft_split_pipe(char const *s, char c)
{
	int		i;
	int		j;
	char	**arr;
	char	f;
	int		q;

	setzero(&i, &j, &f);
	arr = (char **)malloc((word_count((char *)s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	q = 0;
	while (s[i])
	{
		q = check_quote(s[i], q);
		if (s[i] == c && q == 0)
			f = 0;
		else if (s[i] != c && f == 0)
		{
			arr[j] = ft_strindup((char *)(s + i), c);
			if (arr[j] == NULL)
				return (free_all(arr));
			f = 1;
			j++;
		}
		i++;
	}
	arr[j] = NULL;
	return (arr);
}
