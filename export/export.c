/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:35:48 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/28 10:11:10 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	display_export(t_env *env)
{
	env = env->next;
	while (env)
	{
		if (!ft_strcmp(env->key, "?"))
		{
			env = env->next;
			continue ;
		}
		printf("declare -x %s", env->key);
		if (env->empty == 0)
			printf("=\"%s\"\n", env->value);
		else
			printf("\n");
		env = env->next;
	}
}

static void	append_env(t_env *head, t_env *new)
{
	t_env	*last;
	char	*tmp;

	if (!new)
		return (perror("export error"));
	while (head)
	{
		if (head->key && !ft_strcmp(head->key, new->key))
		{
			free(new->key);
			tmp = head->value;
			head->value = ft_strjoin(head->value, new->value);
			if (!head->value)
			{
				head->value = tmp;
				return (free(new->value), free(new));
			}
			(free(tmp), free(new->value), free(new));
			return ;
		}
		last = head;
		head = head->next;
	}
	last->next = new;
}

static int	check_export_syntax(char	**cmd, int *j, int *i, int	*ret)
{
	*i = 1;
	if (cmd[*j][0] == '\0')
	{
		(*j)++;
		return (1);
	}
	if (!isalpha(cmd[*j][0]) && cmd[*j][0] != '_')
	{
		(perror("syntax error"), *ret = 1);
		(*j)++;
		return (1);
	}
	while (cmd[*j][*i] != '=' && cmd[*j][*i] != '\0' && cmd[*j][*i] != '+')
	{
		if (!isalnum(cmd[*j][*i]) && cmd[*j][*i] != '_')
		{
			(perror("syntax error"), *ret = 1);
			(*j)++;
			return (1);
		}
		(*i)++;
	}
	return (0);
}

void	push_path(t_env *env, t_env *exprt)
{
	push_env(env, new_env("PATH=/usr/local/sbin:/usr/local/bin"
			":/usr/sbin:/usr/bin:/sbin:/bin"));
	push_export(exprt, new_env("PATH=/usr/local/sbin:/usr/local/bin"
			":/usr/sbin:/usr/bin:/sbin:/bin"));
	env->i = 0;
}

int	export(t_env *env, t_env *exprt, char **cmd)
{
	int	i;
	int	j;
	int	ret;

	j = 1;
	ret = 0;
	if (ft_dstrlen(cmd) == 1)
		return (display_export(exprt), 0);
	while (cmd[j])
	{
		if (check_export_syntax(cmd, &j, &i, &ret))
			continue ;
		if (!ft_strcmp(cmd[j], "PATH") && env->i)
			push_path(env, exprt);
		else if (cmd[j][i] == '=' || cmd[j][i] == '\0')
			(push_env(env, new_env(cmd[j])),
				push_export(exprt, new_env(cmd[j])));
		else if (cmd[j][i] == '+' && cmd[j][i + 1] == '=')
			(append_env(env, new_env(cmd[j])),
				append_export(exprt, new_env(cmd[j])));
		else
			(perror("syntax error"), ret = 1);
		j++;
	}
	return (ret);
}
