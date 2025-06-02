/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:39:06 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/21 16:43:45 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	push_char(char **s, char c)
{
	char	temp[2];
	char	*tmp;

	temp[0] = c;
	temp[1] = '\0';
	tmp = *s;
	*s = ft_strjoin(*s, temp);
	free(tmp);
}

static char	*ft_getenv2(t_env *envs, char *key)
{
	char	*tmp;

	envs = envs->next;
	while (envs)
	{
		if (!strcmp(key, envs->key))
		{
			tmp = ft_strdup(envs->value);
			if (!tmp)
				return (NULL);
			return (tmp);
		}
		envs = envs->next;
	}
	return (ft_strdup(""));
}

static char	*expand_heredoc(char *str, int *i, char *res, t_env *envs)
{
	int		start;
	int		len;
	char	*val;
	char	*var;

	start = *i;
	len = 0;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
	{
		len++;
		(*i)++;
	}
	var = ft_substr(str, start, len);
	if (!var)
		return (free(res), perror("malloc"), NULL);
	val = ft_getenv2(envs, var);
	if (!val)
		return (free(res), free(var), NULL);
	(replace_expand_quotes1(val), free(var));
	var = ft_strjoin(res, val);
	return (free(val), free(res), var);
}

static char	*expand_dollar_heredoc(char *str, char *res, t_env *envs, int *i)
{
	char	*tmp;

	if (str[*i] == '?')
	{
		res = question_mark(envs, res, i);
		if (res)
			return (NULL);
	}
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		res = expand_heredoc(str, i, res, envs);
		if (!res)
			return (NULL);
	}
	else if (!ft_isalpha(str[*i]) && str[*i] != '_')
	{
		tmp = res;
		res = push_char2(res, '$');
		free(tmp);
		if (!res)
			return (NULL);
	}
	return (res);
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
			i++;
			result = expand_dollar_heredoc(str, result, envs, &i);
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
