/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:57:49 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/04/27 10:19:36 by ayel-arr         ###   ########.fr       */
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

void	push_env(t_env *head, t_env *new)
{
	t_env *last;

	if (!new)
		return (perror("export error"));
	while (head)
	{
		if (head->key && !ft_strcmp(head->key, new->key))
		{
			if (new->empty)
			{
				free(new->key);
				free(new->value);
				free(new);
				return ;
			}
			free(new->key);
			free(head->value);
			head->value = new->value;
			head->empty = new->empty;
			free(new);
			return ;
		}
		last = head;
		head = head->next;
	}
	last->next = new;
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

t_env	*duplicate_env(char **env)
{
	int		i;
	t_env	*head;

	i = 0;
	head = new_env(NULL);
	while (env[i])
		(push_env(head, new_env(env[i])), i++);
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
