/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:10:49 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/18 16:54:59 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_variable(t_env *env, char *key)
{
	t_env	*last;
	t_env	*tmp;

	if (!ft_strcmp(key, "PATH"))
		env->i = 0;
	last = env;
	env = env->next;
	while (env)
	{
		if (!ft_strcmp(key, env->key))
		{
			tmp = env;
			last->next = env->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		last = env;
		env = env->next;
	}
}

int	unset(char **cmd, t_env *env, t_env *export)
{
	int	i;

	i = 1;
	while (cmd[i])
	{
		if (!ft_strcmp(cmd[i], "?") || !ft_strcmp(cmd[i], "_"))
		{
			i++;
			continue ;
		}
		remove_variable(export, cmd[i]);
		remove_variable(env, cmd[i]);
		i++;
	}
	return (0);
}
