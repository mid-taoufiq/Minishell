/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:33:44 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/04/21 15:40:30 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quote(char c, int q)
{
	if (q == 0 && c == '\'')
		return (1);
	if (q == 0 && c == '\"')
		return (2);
	if (q == 2 && c == '\"')
		return (0);
	if (q == 1 && c == '\'')
		return (0);
	return (q);
}

int	ft_strlen_pipe(char *src, char c, char q)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		q = check_quote(src[i], q);
		if (src[i] == c && q == 0)
			break ;
		i++;
	}
	return (i);
}
