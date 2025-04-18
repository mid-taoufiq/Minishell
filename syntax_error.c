/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:29:19 by tibarike          #+#    #+#             */
/*   Updated: 2025/04/17 16:20:52 by ayel-arr         ###   ########.fr       */
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

static bool	check_redirections(char *str, int i)
{
	int		count;
	char	redire;
	char	quote;

	while (str[i])
	{
		if (str[i] == '\'' ||  str[i] == '\"')
		{
			quote = str[i];
			skip(quote, &i, str);
			continue ;
		}
		else if (str[i] == '>' || str[i] == '<')
		{
			redire = str[i];
			i++;
			count = 1;
			while (str[i] == redire)
			{
				count++;
				i++;
			}
			while (str[i] == ' ' || str[i] == '\t')
				i++;
			if (count > 2 || ( str[i] <= 'a' && str[i] >= 'z') || !str[i] || str[i] == '|')
				return (false);
		}
		else
			i++;
	}
	return (true);
}

static bool	check_pipes(char *str, int i, int seen_command)
{
	char quote;

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
			quote = str[i];
			seen_command = 1;
			skip(quote, &i, str);
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

bool	validate_input(char *input) 
{
	if (input[0] == '\0')
		return (true);
	if (!valid_quotes(input) || !check_pipes(input, 0, 0)
		|| !check_redirections(input, 0))
	{
		fprintf(stderr, "syntax error\n");
		return (false);
	}
	return (true);
}

