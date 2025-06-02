/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst_to_arr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:24:55 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/24 15:02:00 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	empty_var(char **ret, int *i, t_env **tmp)
{
	if ((*tmp)->empty)
	{
		ret[*i] = (*tmp)->key;
		(*i)++;
		*tmp = (*tmp)->next;
		return (1);
	}
	return (0);
}

static char	**fill_env_arr(char	**ret, t_env *tmp)
{
	int		i;
	char	*ttmp;

	i = 0;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "?"))
		{
			tmp = tmp->next;
			continue ;
		}
		if (empty_var(ret, &i, &tmp))
			continue ;
		ttmp = ft_strjoin(tmp->key, "=");
		if (!ttmp)
			return (perror("malloc"), NULL);
		ret[i] = ft_strjoin(ttmp, tmp->value);
		if (!ret[i])
			return (freedbl((void **)ret), perror("malloc"), NULL);
		free(ttmp);
		tmp = tmp->next;
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

char	**envlst_to_array(t_env *env)
{
	int		i;
	char	**ret;
	t_env	*tmp;

	i = 0;
	env = env->next;
	tmp = env;
	while (env)
	{
		if (!ft_strcmp(env->key, "?"))
		{
			env = env->next;
			continue ;
		}
		i++;
		env = env->next;
	}
	ret = malloc((i + 1) * sizeof(char *));
	if (!ret)
		return (perror("malloc"), NULL);
	return (fill_env_arr(ret, tmp));
}
