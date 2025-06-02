/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:57:49 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/20 10:51:59 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_head(t_env *ret);
int		chshlvl(t_env *env, char *val);
t_env	*set_default_env(t_env *head, char	*tmp, char	*tmp2);

t_env	*set_value(char *equal, char *env, char *plus, t_env *ret)
{
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
	return (ret);
}

t_env	*new_env(char *env)
{
	t_env	*ret;
	char	*equal;
	char	*plus;

	ret = malloc(sizeof(t_env));
	if (!ret)
		return (NULL);
	if (env == NULL)
		return (new_head(ret), ret);
	ret->next = NULL;
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
	return (set_value(equal, env, plus, ret));
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
				(free(new->key), free(new->value));
				return (free(new), hhead);
			}
			(free(new->key), free(head->value));
			head->value = new->value;
			head->empty = new->empty;
			return (free(new), hhead);
		}
		last = head;
		head = head->next;
	}
	last->next = new;
	return (hhead);
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
			if (chshlvl(env, val))
				return (1);
		}
		env = env->next;
	}
	return (0);
}

t_env	*duplicate_env(char **env)
{
	int		i;
	t_env	*head;
	char	*tmp;

	i = 0;
	head = new_env(NULL);
	if (!head)
		return (NULL);
	if (!env[0])
		return (set_default_env(head, NULL, NULL));
	while (env[i])
	{
		if (!push_env(head, new_env(env[i])))
			return (free_env(head), NULL);
		i++;
	}
	tmp = ft_getenv(head, "PATH");
	if (!tmp)
		(head->i = 1);
	free(tmp);
	if (update_shlvl(head))
		return (free_env(head), NULL);
	if (!push_env(head, new_env("?=0")))
		return (free_env(head), NULL);
	return (head);
}
