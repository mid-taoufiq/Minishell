/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:18:46 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/10 08:53:01 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	push_export(t_env *env, t_env *new)
{
	t_env	*last;

	if (!new)
		return (perror("export"));
	last = env;
	while (env)
	{
		if (env->key && !ft_strcmp(env->key, new->key))
		{
			if (new->empty)
				return (free(new->key), free(new->value), free(new));
			free(env->value);
			env->value = new->value;
			env->empty = new->empty;
			return (free(new->key), free(new));
		}
		else if ((!env->key || ft_strcmp(env->key, new->key) < 0) && (!env->next || ft_strcmp(env->next->key, new->key) > 0))
		{
			new->next = env->next;
			env->next = new;
			return ;
		}
		last = env;
		env = env->next;
	}
	last->next = new;
}

void	append_export(t_env *env, t_env *new)
{
	t_env	*last;
	char	*tmp;

	if (!new)
		return (perror("export"));
	last = env;
	while (env)
	{
		if (env->key && !ft_strcmp(env->key, new->key))
		{
			if (new->empty)
				return (free(new->key), free(new->value), free(new));
			tmp = env->value;
			env->value = ft_strjoin(env->value, new->value);
			if (!env->value)
			{
				env->value = tmp;
				return (free(new->value), free(new->key), free(new));
			}
			env->empty = new->empty;
			(free(new->value), free(new->key), free(tmp), free(new));
			return ;
		}
		if ((!env->key || ft_strcmp(env->key, new->key) < 0) && (!env->next || ft_strcmp(env->next->key, new->key) > 0))
		{
			new->next = env->next;
			env->next = new;
			return ;
		}
		last = env;
		env = env->next;
	}
	last->next = new;
}

char	*get_pwd(char option)
{
	static char	*pwd;

	if (option == 0)
	{
		free(pwd);
		pwd = getcwd(NULL, 0);
		return (pwd);
	}
	if (option == 1)
		return (pwd);
	if (option == 2)
		return (free(pwd), NULL);
	return (pwd);
}
