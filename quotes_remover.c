/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_remover.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 11:20:40 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/12 16:13:26 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (s[i] == '\'' && quote == 0)
		{
			quote = 1;
			i++;
			continue ;
		}
		else if (s[i] == '\'' && quote == 1)
		{
			quote = 0;
			i++;
			continue ;
		}
		else if (s[i] == '\"' && quote == 0)
		{
			quote = 2;
			i++;
			continue ;
		}
		else if (s[i] == '\"' && quote == 2)
		{
			quote = 0;
			i++;
			continue ;
		}
		i++;
		j++;
	}
	return (j + 1);
}

static int	remove_quotes(char **cmds, char *str, int n)
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
		if (str[i] == '\'' && quote == 0)
		{
			quote = 1;
			i++;
			continue ;
		}
		else if (str[i] == '\'' && quote == 1)
		{
			quote = 0;
			i++;
			continue ;
		}
		else if (str[i] == '\"' && quote == 0)
		{
			quote = 2;
			i++;
			continue ;
		}
		else if (str[i] == '\"' && quote == 2)
		{
			quote = 0;
			i++;
			continue ;
		}
		new[j] = str[i];
		i++;
		j++;
	}
	new[j] = '\0';
	free(cmds[n]);
	cmds[n] = new;
	return (0);
}

static int is_there_quote(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (is_quote(s[i]))
			return (1);
		i++;
	}
	return (0);
}
static void replace_expand_quotes1(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == 1)
			s[i] = '\'';
		if (s[i] == 2)
			s[i] = '"';
		i++;
	}
}

int	remove_quotes_main(t_cmd *cmds)
{
	int	c;
	int	str;

	c = 0;
	while (cmds[c].cmd)
	{
		str = 0;
		while (cmds[c].cmd[str])
		{
			if (is_there_quote(cmds[c].cmd[str]))
			{
				if (remove_quotes(cmds[c].cmd, cmds[c].cmd[str], str))
					return (1);
			}
			replace_expand_quotes1(cmds[c].cmd[str]);
			str++;
		}
		str = 0;
		while (cmds[c].redirection[str].file)
		{
			if (is_there_quote(cmds[c].redirection[str].file))
			{
				if (remove_quotes(&(cmds[c].redirection[str].file), cmds[c].redirection[str].file, 0))
					return (1);
			}
			replace_expand_quotes1(cmds[c].redirection[str].file);
			str++;
		}
		c++;
	}
	return (0);
}
