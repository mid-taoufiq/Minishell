/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:17:34 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/04/21 15:44:14 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quote(char c, int q);
int	ft_strlen_pipe(char *src, char c, char q);

static	char	*ft_strindup(char *src, char c)
{
	int		i;
	char	*s;
	int		q;

	i = 0;
	q = 0;
	s = (char *)malloc((ft_strlen_pipe(src, c, 0) + 1) * sizeof(char));
	if (!s)
		return (free(s), NULL);
	while (src[i] != '\0')
	{
		q = check_quote(src[i], q);
		if (src[i] == c && q == 0)
			break ;
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

static void	setzero(int *i, int *j, char f[2])
{
	*i = 0;
	*j = 0;
	f[0] = 0;
	f[1] = 0;
}

char	**ft_split_pipe(char const *s, char c)
{
	int		i;
	int		j;
	char	**arr;
	char	flags[2];

	setzero(&i, &j, flags);
	arr = (char **)malloc((word_count((char *)s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	while (s[i])
	{
		flags[0] = check_quote(s[i], flags[0]);
		if (s[i] == c && flags[0] == 0)
			flags[1] = 0;
		else if (s[i] != c && flags[1] == 0)
		{
			arr[j] = ft_strindup((char *)(s + i), c);
			if (arr[j] == NULL)
				return (free_all(arr));
			flags[1] = 1;
			j++;
		}
		i++;
	}
	return (arr[j] = NULL, arr);
}
