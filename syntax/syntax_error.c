/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:29:19 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/28 15:38:41 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	skip(char quote, int *i, char *str)
{
	(*i)++;
	while (str[*i] != quote)
		(*i)++;
	(*i)++;
}

static	char	check_redirections_child(char redire,
		char *str, int *i, int count)
{
	redire = str[*i];
	(*i)++;
	count = 1;
	while (str[*i] == redire)
	{
		count++;
		(*i)++;
	}
	while (is_whitespace(str[*i]) && str[*i])
		(*i)++;
	if (count > 2 || str[*i] == '|' || str[*i] == '>'
		|| str[*i] == '<' || !str[*i])
		return (1);
	return (0);
}

static bool	check_redirections(char *str, int i, int count, char redire)
{
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			skip(str[i], &i, str);
			continue ;
		}
		else if (str[i] == '>' || str[i] == '<')
		{
			if (check_redirections_child(redire, str, &i, count))
				return (false);
		}
		else
			i++;
	}
	return (true);
}

static bool	check_pipes(char *str, int i, int seen_command)
{
	while (str[i])
	{
		if (str[i] == '|')
		{
			if (!seen_command)
				return (false);
			seen_command = 0;
			i++;
			continue ;
		}
		else if (str[i] == '\'' || str[i] == '\"')
		{
			(skip(str[i], &i, str), seen_command = 1);
			continue ;
		}
		else if (str[i] != ' ' && str[i] != '\t')
			seen_command = 1;
		i++;
	}
	if (!seen_command)
		return (false);
	return (true);
}

bool	validate_input(char *input, t_env *envs, t_env *s_env)
{
	if (!input[0])
		return (false);
	add_history(input);
	if (check_empty_string(input))
		return (false);
	if (!valid_quotes(input) || !check_pipes(input, 0, 0)
		|| !check_redirections(input, 0, 0, 0))
	{
		chexitstatus(2, envs, s_env);
		fprintf(stderr, "syntax error\n");
		return (false);
	}
	return (true);
}
