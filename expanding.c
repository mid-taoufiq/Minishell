/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:12:31 by tibarike          #+#    #+#             */
/*   Updated: 2025/04/20 13:48:20 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*exctract_dollar(char *str, int *i, char *res)
{
	char	*var;
	char	*val;
	int		start;
	int		len;

	(*i)++;
	val = NULL;
	len = 0;
	if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		start = *i;
		while (ft_isalnum(str[*i]) || str[*i] == '_')
		{
			len++;
			(*i)++;
		}
		var = ft_substr(str, start, len);
		val = getenv(var);
		free(var);
	}
	var = ft_strjoin(res, val);
	free(res);
	return (var);
}

static void	push_char(char **s, char c)
{
	char	temp[2];
	char	*tmp;

	temp[0] = c;
	temp[1] = '\0';
	tmp = *s;
	*s = ft_strjoin(*s, temp);
	free(tmp);
}

static void	init(int *i, bool *in_double, bool *in_single, char **result)
{
	*i = 0;
	*in_double = false;
	*in_single = false;
	*result = ft_strdup("");
}

static char	*expand_parse(char *str)
{
	int		i;
	bool	in_single;
	bool	in_double;
	char	*result;

	init(&i, &in_double, &in_single, &result);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			i++;
		}
		else if (str[i] == '"' && !in_single)
		{
			in_double = !in_double;
			i++;
		}
		else if (str[i] == '$' && !in_single)
			result = exctract_dollar(str, &i, result);
		else
			(push_char(&result, str[i]), i++);
	}
	return (result);
}

void	expand(t_cmd *all_cmds, int i, int z, char *tmp)
{
	while (all_cmds[i].cmd)
	{
		z = 0;
		while (all_cmds[i].cmd[z])
		{
			if (ft_strchr(all_cmds[i].cmd[z], '$'))
			{
				tmp = expand_parse(all_cmds[i].cmd[z]);
				(free(all_cmds[i].cmd[z]), all_cmds[i].cmd[z] = tmp);
			}
			z++;
		}
		z = 0;
		while (all_cmds[i].redirection[z].file)
		{
			if (ft_strchr(all_cmds[i].redirection[z].file, '$'))
			{
				tmp = expand_parse(all_cmds[i].redirection[z].file);
				free(all_cmds[i].redirection[z].file);
				all_cmds[i].redirection[z].file = tmp;
			}
			z++;
		}
		i++;
	}
}
