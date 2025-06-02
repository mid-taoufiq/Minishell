/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chenv2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:53:37 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/21 12:14:09 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	chpwd2(t_env *exprt, char *new)
{
	t_env	*ttmp;
	char	*tmp;

	ttmp = exprt;
	exprt = exprt->next;
	while (exprt)
	{
		if (!ft_strcmp(exprt->key, "PWD"))
		{
			tmp = ft_strdup(new);
			if (!tmp)
				break ;
			(free(exprt->value), exprt->value = tmp);
			exprt->empty = 0;
			break ;
		}
		exprt = exprt->next;
	}
	if (!exprt)
	{
		tmp = ft_strjoin("PWD=", new);
		if (!tmp)
			return (free(new));
		(push_env(ttmp, new_env(tmp)), free(tmp), free(new));
	}
}
