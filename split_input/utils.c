/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:44:04 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/13 20:40:44 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_quotes(char c, int *q)
{
	if (c == '\'' && *q == 0)
		*q = 1;
	else if (c == '\'' && *q == 1)
		*q = 0;
	else if (c == '\"' && *q == 0)
		*q = 2;
	else if (c == '\"' && *q == 2)
		*q = 0;
}

void	freedblchar(char **dbl, int size)
{
	int		i;

	i = 0;
	while (i < size)
	{
		free(dbl[i]);
		i++;
	}
	free(dbl);
}

int	fill_split_string(char *s, char *ret)
{
	int		ii;
	char	c;

	ii = 0;
	while (!is_whitespace(s[ii]))
	{
		if (is_quote(s[ii]))
		{
			c = s[ii];
			ret[ii] = s[ii];
			ii++;
			while (s[ii] != c)
			{
				ret[ii] = s[ii];
				ii++;
			}
		}
		ret[ii] = s[ii];
		ii++;
	}
	ret[ii] = '\0';
	return (ii);
}
