/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:02:31 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/13 20:40:23 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	skip(char quote, int *i, char *str)
{
	(*i)++;
	while (str[*i] != quote)
		(*i)++;
}

char	*ft_strindup(char *s, int *i)
{
	int		ii;
	char	*ret;

	ii = 0;
	while (!is_whitespace(s[ii]))
	{
		if (is_quote(s[ii]))
			skip(s[ii], &ii, s);
		ii++;
	}
	ret = malloc((ii + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	ii = fill_split_string(s, ret);
	*i = *i + ii;
	return (ret);
}

int	wordcount(char *s)
{
	int	i;
	int	q;
	int	count;
	int	f;

	i = 0;
	f = 1;
	q = 0;
	count = 0;
	while (s[i])
	{
		if (is_whitespace(s[i]) && q == 0)
			f = 1;
		change_quotes(s[i], &q);
		if (f == 1 && !is_whitespace(s[i]))
		{
			count++;
			f = 0;
		}
		i++;
	}
	return (count);
}

static void	init(int *i, int *f, int *count)
{
	*i = 0;
	*f = 1;
	*count = 0;
}

char	**ft_split_input(char *str)
{
	char	**ret;
	int		i;
	int		count;
	int		f;

	init(&i, &f, &count);
	ret = malloc((wordcount(str) + 1) * sizeof(char *));
	if (!ret)
		return (NULL);
	while (str[i])
	{
		if (is_whitespace(str[i]))
			f = 1;
		else if (f == 1 && !is_whitespace(str[i]))
		{
			ret[count] = ft_strindup(str + i, &i);
			if (!ret[count])
				return (freedblchar(ret, count), NULL);
			count++;
			f = 0;
			continue ;
		}
		i++;
	}
	return (ret[count] = NULL, ret);
}
