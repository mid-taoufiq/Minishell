/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_issues.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:28:06 by tibarike          #+#    #+#             */
/*   Updated: 2025/04/12 16:29:26 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator_char(char c)
{
	return (c == '|' || c == '&');
}

int	check_operator_block(const char *str, int *i, int last_was_op)
{
	char	op;
	int		count;

	op = str[(*i)++];
	count = 1;
	while (str[*i] == op)
	{
		count++;
		(*i)++;
	}
	if (count > 2 || last_was_op)
	{
		fprintf(stderr, "minishell: syntax error near `%c'\n", op);
		return (1);
	}
	return (0);
}

int	check_operator_issues(const char *str)
{
	int	i;
	int	last_was_op;

	i = 0;
	last_was_op = 1;
	while (str[i])
	{
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (is_operator_char(str[i]))
		{
			if (check_operator_block(str, &i, last_was_op))
				return (1);
			last_was_op = 1;
		}
		else
		{
			last_was_op = 0;
			while (str[i] && !is_operator_char(str[i]) && str[i] != ' ')
				i++;
		}
	}
	if (last_was_op)
		return (fprintf(stderr, "syntax error\n"), 1);
	return (0);
}
