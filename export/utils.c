/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:57:49 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/14 15:27:20 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_env(char *env)
{
	t_env	*ret;
	char	*equal;
	char	*plus;

	ret = malloc(sizeof(t_env));
	if (!ret)
		return (NULL);
	if (env == NULL)
	{
		ret->key = NULL;
		ret->value = NULL;
		ret->next = NULL;
		return (ret);
	}
	equal = ft_strchr(env, '=');
	plus = ft_strchr(env, '+');
	if (plus == equal - 1)
		equal = plus;
	if (equal == NULL)
		equal = ft_strchr(env, '\0');
	ret->key = ft_substr(env, 0, equal - env);
	if (!(ret->key))
		return (free(ret), NULL);
	ret->empty = 0;
	if (*equal == '\0')
	{
		ret->value = ft_substr(env, equal - env, 0);
		if (!(ret->value))
			return (free(ret), NULL);
		ret->empty = 1;
	}
	else if (equal == plus)
	{
		ret->value = ft_substr(env, (equal - env) + 2, ft_strlen(equal + 2));
		if (!(ret->value))
			return (free(ret), NULL);
	}
	else
	{
		ret->value = ft_substr(env, (equal - env) + 1, ft_strlen(equal + 1));
		if (!(ret->value))
			return (free(ret), NULL);
	}
	ret->next = NULL;
	return (ret);
}

t_env	*push_env(t_env *head, t_env *new)
{
	t_env	*last;
	t_env	*hhead;

	hhead = head;
	if (!new)
		return (perror("export error"), NULL);
	while (head)
	{
		if (head->key && !ft_strcmp(head->key, new->key))
		{
			if (new->empty)
			{
				free(new->key);
				free(new->value);
				free(new);
				return (hhead);
			}
			free(new->key);
			free(head->value);
			head->value = new->value;
			head->empty = new->empty;
			free(new);
			return (hhead);
		}
		last = head;
		head = head->next;
	}
	last->next = new;
	return (hhead);
}

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

int	update_shlvl(t_env *env)
{
	char	*val;
	int		num;
	int		ss;

	val = ft_getenv(env, "SHLVL");
	if (!val)
		return (1);
	num = (int)ft_atol(val, &ss);
	num++;
	free(val);
	val = ft_itoa(num);
	if (!val)
		return (1);
	env = env->next;
	while (env)
	{
		if (!ft_strcmp(env->key, "SHLVL"))
		{
			if (!ft_strcmp(env->value, "999"))
			{
				ft_putstr_fd("warning: shell level (1000)"
					" too high, resetting to 1\n", 2);
				(free(env->value), free(val), env->value = ft_strdup("1"));
				return (0);
			}
			free(env->value);
			env->value = val;
			return (0);
		}
		env = env->next;
	}
	free(val);
	return (0);
}

t_env	*duplicate_env(char **env)
{
	int		i;
	t_env	*head;
	char	*tmp;
	char	*tmp2;
	t_env	*ttmp;

	i = 0;
	head = new_env(NULL);
	if (!env[0])
	{
		head->i = 1;
		tmp = getcwd(NULL, 0);
		tmp2 = ft_strjoin("PWD=", tmp);
		if (!tmp2)
			return (free(tmp), free_env(head), NULL);
		free(tmp);
		ttmp = push_env(head, new_env(tmp2));
		if (!ttmp)
			return (free_env(head), free(tmp2), NULL);
		free(tmp2);
		ttmp = push_env(head, new_env("SHLVL=1"));
		if (!ttmp)
			return (free_env(head), NULL);
		ttmp = push_env(head, new_env("_=/usr/bin/env"));
		if (!ttmp)
			return (free_env(head), NULL);
		ttmp = push_env(head, new_env("OLDPWD"));
		if (!ttmp)
			return (free_env(head), NULL);
		ttmp = push_env(head, new_env("?=0"));
		if (!ttmp)
			return (free_env(head), NULL);
		return (head);
	}
	head->i = 0;
	while (env[i])
	{
		ttmp = push_env(head, new_env(env[i]));
		if (!ttmp)
			return (free_env(head), NULL);
		i++;
	}
	tmp = ft_getenv(head, "PATH");
	if (!tmp)
		head->i = 1;
	free(tmp);
	if (update_shlvl(head))
		return (free_env(head), NULL);
	ttmp = push_env(head, new_env("?=0"));
	if (!ttmp)
		return (free_env(head), NULL);
	return (head);
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
