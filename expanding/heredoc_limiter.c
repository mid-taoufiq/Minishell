/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_limiter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 09:11:36 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/22 10:16:23 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	shift_to_the_end(char	*s, int *i)
{
	int		iter;
	char	tmp;

	iter = *i;
	while (s[iter + 1])
	{
		tmp = s[iter];
		s[iter] = s[iter + 1];
		s[iter + 1] = tmp;
		iter++;
	}
	s[iter] = '\0';
	(*i)--;
	return (0);
}

void	if_quoted(t_cmd *all_cmds, int *z, int i, char *tmp)
{
	free(all_cmds[i].cmd[*z]);
	all_cmds[i].cmd[*z] = tmp;
}

int	is_quoted(char	*s)
{
	int		i;
	char	quoted;

	i = 0;
	quoted = 0;
	while (s[i])
	{
		if (is_quote(s[i]) && quoted == 0)
			quoted = 1;
		else if (is_quote(s[i]) && quoted == 1)
			quoted = 0;
		else if (s[i] == '$' && quoted)
			return (1);
		i++;
	}
	return (0);
}

int	rm_dollar(char	*str)
{
	int		i;
	char	quotes;

	i = 0;
	quotes = 0;
	while (str[i])
	{
		if (str[i] == '"' && quotes == 0)
			quotes = 2;
		else if (str[i] == '"' && quotes == 2)
			quotes = 0;
		else if (str[i] == '\'' && quotes == 0)
			quotes = 1;
		else if (str[i] == '\'' && quotes == 1)
			quotes = 0;
		else if (str[i] == '$' && is_quote(str[i + 1]) && quotes == 0)
			shift_to_the_end(str, &i);
		i++;
	}
	return (0);
}
