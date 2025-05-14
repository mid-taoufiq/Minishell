/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 09:38:56 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/11 14:53:54 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**insert2darray(char **src, char **new, int i)
{
	char	**ret;
	int		ii;
	int		j;
	int		y;
	int		size;

	ii = 0;
	j = 0;
	y = 0;
	size = ft_dstrlen(src) - 1;
	size += ft_dstrlen(new);
	size++;
	ret = malloc(size * sizeof(char *));
	if (!ret)
		return (perror("malloc"), NULL);
	while (ii < i)
	{
		ret[ii] = src[ii];
		ii++;
		j++;
	}
	free(src[ii]);
	ii++;
	while (new[y])
	{
		ret[j] = new[y];
		y++;
		j++;
	}
	while (src[ii])
	{
		ret[j] = src[ii];
		ii++;
		j++;
	}
	ret[j] = NULL;
	return (ret);
}
