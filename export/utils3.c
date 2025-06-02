/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:03:08 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/20 10:41:11 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*last;

	while (env)
	{
		free(env->key);
		free(env->value);
		last = env;
		env = env->next;
		free(last);
	}
}

char	*ft_getenv(t_env *envs, char *key)
{
	envs = envs->next;
	while (envs)
	{
		if (!strcmp(key, envs->key))
			return (ft_strdup(envs->value));
		envs = envs->next;
	}
	return (NULL);
}

void	new_head(t_env *ret)
{
	ret->key = NULL;
	ret->value = NULL;
	ret->next = NULL;
	ret->i = 0;
}

int	chshlvl(t_env *env, char *val)
{
	if (!ft_strcmp(env->value, "999"))
	{
		ft_putstr_fd("warning: shell level (1000)"
			" too high, resetting to 1\n", 2);
		(free(env->value), free(val));
		env->value = ft_strdup("1");
		if (!env->value)
			return (1);
		return (0);
	}
	free(env->value);
	env->value = val;
	return (0);
}

t_env	*set_default_env(t_env *head, char	*tmp, char	*tmp2)
{
	head->i = 1;
	tmp = getcwd(NULL, 0);
	tmp2 = ft_strjoin("PWD=", tmp);
	if (!tmp2)
		return (free(tmp), free_env(head), NULL);
	free(tmp);
	if (!push_env(head, new_env(tmp2)))
		return (free_env(head), free(tmp2), NULL);
	free(tmp2);
	if (!push_env(head, new_env("SHLVL=1")))
		return (free_env(head), NULL);
	if (!push_env(head, new_env("_=/usr/bin/env")))
		return (free_env(head), NULL);
	if (!push_env(head, new_env("OLDPWD")))
		return (free_env(head), NULL);
	if (!push_env(head, new_env("?=0")))
		return (free_env(head), NULL);
	return (head);
}
