/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:12:31 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/12 16:07:25 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*push_char2(char *s, char c)
{
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	s = ft_strjoin(s, temp);
	return (s);
}

static char	*replace_expand_quotes(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'')
			s[i] = 1;
		if (s[i] == '\"')
			s[i] = 2;
		i++;
	}
	return (s);
}

static char	*exctract_dollar(char *str, int *i, char *res, t_env *envs)
{
	char	*var;
	char	*val;
	int		start;
	int		len;

	(*i)++;
	val = NULL;
	len = 0;
	var = ft_strdup("");
	if (str[*i] == '?')
	{
		val = ft_getenv(envs, "?");
		var = ft_strjoin(res, val);
		if (!var)
			return (perror("malloc"), free(res), free(val), NULL);
		(free(res), free(val));
		(*i)++;
		return (var);
	}
	if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		start = *i;
		while (ft_isalnum(str[*i]) || str[*i] == '_')
		{
			len++;
			(*i)++;
		}
		free(var);
		var = ft_substr(str, start, len);
		if (!var)
			return (perror("malloc"), NULL);
		val = replace_expand_quotes(ft_getenv(envs, var));
		free(var);
		var = ft_strjoin(res, val);
	}
	else if (!ft_isalpha(str[*i]) && str[*i] != '_')
	{
		free(var);
		var = push_char2(res, '$');
	}
	if (!var)
		return (perror("malloc"), free(res), free(val), NULL);
	(free(res), free(val));
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

static char	*expand_parse(char *str, t_env *envs)
{
	int		i;
	bool	in_single;
	bool	in_double;
	char	*result;

	init(&i, &in_double, &in_single, &result);
	if (!result)
		return (perror("malloc"), NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
		{
			push_char(&result, str[i]);
			if (!result)
				return (perror("malloc"), NULL);
			in_single = !in_single;
			i++;
		}
		else if (str[i] == '"' && !in_single)
		{
			push_char(&result, str[i]);
			if (!result)
				return (perror("malloc"), NULL);
			in_double = !in_double;
			i++;
		}
		else if (str[i] == '$' && !in_single)
		{
			if (str[i + 1] == '\"' && !in_double)
			{
				i += 2;
				while (str[i] && str[i] != '\"')
				{
					push_char(&result, str[i]);
					if (!result)
						return (perror("malloc"), NULL);
					i++;
				}
				if (str[i] == '\"')
					i++;
			}
			else
			{
				result = exctract_dollar(str, &i, result, envs);
				if (!result)
					return (NULL);
			}
		}
		else
		{
			push_char(&result, str[i]);
			if (!result)
				return (perror("malloc"), NULL);
			i++;
		}
	}
	if (!result)
		return (perror("malloc"), NULL);
	return (result);
}

char	*expand_parse_heredoc(char *str, t_env *envs)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (perror("malloc"), NULL);
	while (str[i])
	{
		if (str[i] == '$')
		{
			result = exctract_dollar(str, &i, result, envs);
			if (!result)
				return (NULL);
		}
		else
		{
			push_char(&result, str[i]);
			if (!result)
				return (perror("malloc"), NULL);
			i++;
		}
	}
	return (result);
}

int	expand(t_cmd *all_cmds, int i, int z, t_env *envs)
{
	char	*tmp;
	char	**split;
	int		len;
	char	**ttmp;

	tmp = NULL;
	while (all_cmds[i].cmd)
	{
		z = 0;
		while (all_cmds[i].cmd[z])
		{
			if (ft_strchr(all_cmds[i].cmd[z], '$'))
			{
				tmp = expand_parse(all_cmds[i].cmd[z], envs);
				if (!tmp)
					return (1);
				split = ft_split_input(tmp);
				len = ft_dstrlen(split);
				if (len == 1)
				{
					(free(all_cmds[i].cmd[z]), all_cmds[i].cmd[z] = tmp);
					z++;
					(free(split[0]), free(split)); 
					continue ;
				}
				ttmp = insert2darray(all_cmds[i].cmd, split, z);
				if (!ttmp)
					return (1);
				free(all_cmds[i].cmd);
				free(split);
				free(tmp);
				all_cmds[i].cmd = ttmp;
				z += len - 1;
			}
			z++;
		}
		z = 0;
		while (all_cmds[i].redirection[z].file)
		{
			if (ft_strchr(all_cmds[i].redirection[z].file, '$') && all_cmds[i].redirection[z].type != 2)
			{
				tmp = expand_parse(all_cmds[i].redirection[z].file, envs);
				if (!tmp)
					return (all_cmds[i].redirection[z].error = 1, 1);
				if (check_empty_string(tmp) || space_separated(tmp))
				{
					all_cmds[i].redirection[z].error = 1;
					free(tmp);
					z++;
					continue ;
				}
				free(all_cmds[i].redirection[z].file);
				all_cmds[i].redirection[z].file = tmp;
			}
			z++;
		}
		i++;
	}
	return (0);
}
