/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_remover.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 11:20:40 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/14 10:44:14 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_there_quote(char *s)
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

static void	replace_expand_quotes1(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == 1)
			s[i] = '\'';
		if (s[i] == 2)
			s[i] = '\"';
		i++;
	}
}

static int	remove_from_cmd(t_cmd *cmds, int c, int str)
{
	if (is_there_quote(cmds[c].cmd[str]))
	{
		if (remove_quotes(cmds[c].cmd, cmds[c].cmd[str], str))
			return (1);
	}
	replace_expand_quotes1(cmds[c].cmd[str]);
	return (0);
}

static int	remove_from_redirection(t_cmd *cmds, int c, int str)
{
	if (is_there_quote(cmds[c].redirection[str].file))
	{
		if (remove_quotes(&(cmds[c].redirection[str].file),
				cmds[c].redirection[str].file, 0))
			return (1);
	}
	replace_expand_quotes1(cmds[c].redirection[str].file);
	return (0);
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
			if (remove_from_cmd(cmds, c, str))
				return (1);
			str++;
		}
		str = 0;
		while (cmds[c].redirection[str].file)
		{
			if (remove_from_redirection(cmds, c, str))
				return (1);
			str++;
		}
		c++;
	}
	return (0);
}
