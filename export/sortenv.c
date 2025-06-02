/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sortenv.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:58:06 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/14 11:58:48 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*duplicate_node(t_env *node)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->empty = node->empty;
	new->key = ft_strdup(node->key);
	if (!new->key && node->key)
		return (free(new), NULL);
	new->value = ft_strdup(node->value);
	if (!new->value && node->value)
		return (free(new), free(new->key), NULL);
	new->next = NULL;
	return (new);
}

t_env	*duplicate_list(t_env *env)
{
	t_env	*head;
	t_env	*curr;

	head = duplicate_node(env);
	if (!head)
		return (NULL);
	curr = head;
	env = env->next;
	while (env)
	{
		if (!ft_strcmp(env->key, "_"))
		{
			env = env->next;
			continue ;
		}
		curr->next = duplicate_node(env);
		if (!curr->next)
			return (free_env(head), NULL);
		curr = curr->next;
		env = env->next;
	}
	return (head);
}

t_env	*smallest_node(t_env *lst)
{
	t_env	*ret;
	t_env	*last;
	t_env	*ret_last;

	last = lst;
	lst = lst->next;
	ret = lst;
	while (lst)
	{
		if (ft_strcmp(lst->key, ret->key) <= 0)
		{
			ret_last = last;
			ret = lst;
		}
		last = lst;
		lst = lst->next;
	}
	ret_last->next = ret->next;
	return (ret);
}

t_env	*sort_lst(t_env *lst)
{
	t_env	*new;
	t_env	*curr;
	t_env	*i;
	t_env	*duplicated;

	duplicated = duplicate_list(lst);
	if (!duplicated)
		return (NULL);
	new = duplicate_node(lst);
	if (!new)
		return (free_env(duplicated), NULL);
	curr = new;
	i = lst->next;
	while (i->next)
	{
		curr->next = smallest_node(duplicated);
		curr = curr->next;
		i = i->next;
	}
	free(duplicated);
	return (new);
}
