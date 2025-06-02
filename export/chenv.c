/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chenv.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:44:41 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/21 12:14:46 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	chexitstatus2(int status, t_env *exprt)
{
	char	*tmp;

	exprt = exprt->next;
	while (exprt)
	{
		if (!ft_strcmp(exprt->key, "?"))
		{
			tmp = ft_itoa(status);
			if (!tmp)
				break ;
			free(exprt->value);
			exprt->value = tmp;
			break ;
		}
		exprt = exprt->next;
	}
}

void	chexitstatus(int status, t_env *env, t_env *exprt)
{
	char	*tmp;

	env = env->next;
	while (env)
	{
		if (!ft_strcmp(env->key, "?"))
		{
			tmp = ft_itoa(status);
			if (!tmp)
				break ;
			free(env->value);
			env->value = tmp;
			break ;
		}
		env = env->next;
	}
	chexitstatus2(status, exprt);
}

void	choldpwd2(t_env	*exprt, char *new)
{
	char	*tmp;
	t_env	*ttmp;

	ttmp = exprt;
	exprt = exprt->next;
	while (exprt)
	{
		if (!ft_strcmp(exprt->key, "OLDPWD"))
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
		tmp = ft_strjoin("OLDPWD=", new);
		if (!tmp)
			return ;
		(push_env(ttmp, new_env(tmp)), free(tmp), free(new));
	}
}

void	choldpwd(t_env *env, t_env *exprt, char *new)
{
	t_env	*tmp;
	char	*ttmp;

	tmp = env;
	env = env->next;
	while (env)
	{
		if (!ft_strcmp(env->key, "OLDPWD"))
		{
			free(env->value);
			env->value = new;
			env->empty = 0;
			break ;
		}
		env = env->next;
	}
	if (!env)
	{
		ttmp = ft_strjoin("OLDPWD=", new);
		if (!ttmp)
			return (free(new));
		push_env(tmp, new_env(ttmp));
		free(ttmp);
	}
	choldpwd2(exprt, new);
}

void	chpwd(t_env *env, t_env *exprt, char *new)
{
	char	*tmp;
	t_env	*ttmp;

	ttmp = env;
	env = env->next;
	while (env)
	{
		if (!ft_strcmp(env->key, "PWD"))
		{
			(free(env->value), env->value = new);
			env->empty = 0;
			break ;
		}
		env = env->next;
	}
	if (!env)
	{
		tmp = ft_strjoin("PWD=", new);
		if (!tmp)
			return (free(new));
		(push_env(ttmp, new_env(tmp)), free(tmp));
	}
	chpwd2(exprt, new);
}
