/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:16:29 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/14 10:49:21 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	should_i_skip(char c, char *quote, int *i)
{
	if (c == '\'' && *quote == 0)
	{
		*quote = 1;
		(*i)++;
		return (1);
	}
	else if (c == '\'' && *quote == 1)
	{
		*quote = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

static char	should_i_skip2(char c, char *quote, int *i)
{
	if (c == '\"' && *quote == 0)
	{
		*quote = 2;
		(*i)++;
		return (1);
	}
	else if (c == '\"' && *quote == 2)
	{
		*quote = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

static int	size_without_quotes(char *s)
{
	int		i;
	char	quote;
	int		j;

	i = 0;
	j = 0;
	quote = 0;
	while (s[i])
	{
		if (should_i_skip(s[i], &quote, &i))
			continue ;
		else if (should_i_skip2(s[i], &quote, &i))
			continue ;
		i++;
		j++;
	}
	return (j + 1);
}

int	remove_quotes(char **cmds, char *str, int n)
{
	char	*new;
	int		i;
	int		j;
	char	quote;

	i = 0;
	quote = 0;
	j = 0;
	new = malloc(size_without_quotes(str) * sizeof(char));
	if (!new)
		return (1);
	while (str[i])
	{
		if (should_i_skip(str[i], &quote, &i))
			continue ;
		else if (should_i_skip2(str[i], &quote, &i))
			continue ;
		new[j] = str[i];
		i++;
		j++;
	}
	new[j] = '\0';
	free(cmds[n]);
	cmds[n] = new;
	return (0);
}
