/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:02:05 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/13 16:09:24 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip(char quote, int *i, char *str)
{
	(*i)++;
	while (str[*i] != quote)
	{
		(*i)++;
	}
	(*i)++;
}

char	skip2(int *i, int *j, char *str, char *str2)
{
	char	quote;

	quote = str[*i];
	str2[*j] = str[*i];
	(*j)++;
	(*i)++;
	while (str[*i] != quote)
	{
		str2[*j] = str[*i];
		(*i)++;
		(*j)++;
	}
	str2[*j] = str[*i];
	(*i)++;
	(*j)++;
	return (1);
}

static int	space_needed(char *s)
{
	int		i;
	int		spaces;
	char	c;

	i = 0;
	spaces = 0;
	c = 0;
	while (s[i])
	{
		if (is_quote(s[i]))
		{
			skip(s[i], &i, s);
			continue ;
		}
		if (s[i] == '>' || s[i] == '<')
			c = s[i];
		if (s[i] == c && (i != 0 && s[i - 1] != c && !is_whitespace(s[i - 1])))
			spaces++;
		if (s[i] == c && (s[i + 1] != c && !is_whitespace(s[i + 1])))
			spaces++;
		c = 0;
		i++;
	}
	return (spaces);
}

static int	inset_space(char *new, int j)
{
	new[j] = ' ';
	j++;
	return (j);
}

char	*seperate_redirections(char *s, int i, int j, char c)
{
	int		size;
	char	*new;

	size = ft_strlen(s) + space_needed(s);
	new = malloc((size + 1) * sizeof(char));
	if (!new)
		return (NULL);
	while (j < size)
	{
		if (is_quote(s[i]) && skip2(&i, &j, s, new))
			continue ;
		if (s[i] == '>' || s[i] == '<')
			c = s[i];
		if (s[i] == c && (i != 0 && s[i - 1] != c && !is_whitespace(s[i - 1])))
			j = inset_space(new, j);
		new[j] = s[i];
		j++;
		if (s[i] == c && (s[i + 1] != c && !is_whitespace(s[i + 1])))
			j = inset_space(new, j);
		c = 0;
		i++;
	}
	return (new[j] = '\0', new);
}
