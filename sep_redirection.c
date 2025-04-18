/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:02:05 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/04/17 16:27:55 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	TODO:
		count how many redirections that are not seperated.
		the count how many spaces needeed.
		malloc.
		fill.
*/
void skip(char quote, int *i, char *str)
{
	(*i)++;
	while (str[*i] != quote)
	{

		(*i)++;
	}
	(*i)++;
}

void skip2(int *i, int *j, char *str, char *str2)
{
	char quote;

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
			skip(s[i], &i, s);
		if (s[i] == '>' || s[i] == '<')
			c = s[i];
		if (s[i] == c && (i != 0 && s[i - 1] != c && s[i - 1] != ' '))
			spaces++;
		if (s[i] == c && (s[i + 1] != c && s[i + 1] != ' '))
			spaces++;
		c = 0;
		i++;
	}
	return (spaces);
}

char	*seperate_redirections(char *s)
{
	int		size;
	char	*new;
	int		i;
	int		j;
	char	c;

	size = ft_strlen(s) + space_needed(s);
	new = malloc((size + 1) * sizeof(char));
	i = 0;
	j = 0;
	c = 0; 
	while (j < size)
	{
		if (is_quote(s[i]))
			skip2(&i, &j, s, new);
		if (s[i] == '>' || s[i] == '<')
			c = s[i];
		if (s[i] == c && (i != 0 && s[i - 1] != c && s[i - 1] != ' '))
		{
			new[j] = ' ';
			j++;
		}
		new[j] = s[i];
		j++;
		if (s[i] == c && (s[i + 1] != c && s[i + 1] != ' '))
		{
			new[j] = ' ';
			j++;
		}
		c = 0;
		i++;
	}
	new[j] = '\0';
	return (new);
}
