/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:35:48 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/13 15:41:44 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_env(t_env *env)
{
	env = env->next;
	while (env)
	{
		if (!env->empty && ft_strcmp(env->key, "?"))
		{
			printf("%s=%s\n", env->key, env->value);
		}
		env = env->next;
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
			tmp = env->value;
			env->value = ft_itoa(status);
			env->empty = 0;
			free(tmp);
		}
		env = env->next;
	}
	exprt = exprt->next;
	while (exprt)
	{
		if (!ft_strcmp(exprt->key, "?"))
		{
			tmp = exprt->value;
			exprt->value = ft_itoa(status);
			exprt->empty = 0;
			free(tmp);
		}
		exprt = exprt->next;
	}
}

void	choldpwd(t_env *env, t_env *exprt, char *new)
{
	char	*tmp;

	env = env->next;
	while (env)
	{
		if (!ft_strcmp(env->key, "OLDPWD"))
		{
			tmp = env->value;
			env->value = new;
			env->empty = 0;
			free(tmp);
			break ;
		}
		env = env->next;
	}
	exprt = exprt->next;
	while (exprt)
	{
		if (!ft_strcmp(exprt->key, "OLDPWD"))
		{
			tmp = exprt->value;
			exprt->value = ft_strdup(new);
			exprt->empty = 0;
			free(tmp);
			break ;
		}
		exprt = exprt->next;
	}
}

void	chpwd(t_env *env, t_env *exprt, char *new)
{
	char	*tmp;

	env = env->next;
	while (env)
	{
		if (!ft_strcmp(env->key, "PWD"))
		{
			tmp = env->value;
			env->value = new;
			free(tmp);
			break ;
		}
		env = env->next;
	}
	exprt = exprt->next;
	while (exprt)
	{
		if (!ft_strcmp(exprt->key, "PWD"))
		{
			tmp = exprt->value;
			exprt->value = ft_strdup(new);
			free(tmp);
			break ;
		}
		exprt = exprt->next;
	}
}

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
			free(tmp);
			free(new->value);
			free(new);
			return ;
		}
		last = head;
		head = head->next;
	}
	last->next = new;
}

int	export(t_env *env, t_env *exprt, char **cmd)
{
	int	i;
	int	j;

	j = 1;
	if (ft_dstrlen(cmd) == 1)
		return (display_export(exprt), 0);
	while (cmd[j])
	{
		i = 1;
		if (cmd[j][0] == '\0')
		{
			j++;
			continue ;
		}
		if (!isalpha(cmd[j][0]) && cmd[j][0] != '_')
		{
			perror("syntax error");
			j++;
			continue ;
		}
		while (cmd[j][i] != '=' && cmd[j][i] != '\0' && cmd[j][i] != '+')
		{
			if (!isalnum(cmd[j][i]) && cmd[j][i] != '_')
			{
				perror("syntax error");
				j++;
				continue ;
			}
			i++;
		}
		if (!ft_strcmp(cmd[j], "PATH") && env->i)
		{
			push_env(env, new_env("PATH=/usr/local/sbin:/usr/local/bin"
					":/usr/sbin:/usr/bin:/sbin:/bin"));
			push_export(exprt, new_env("PATH=/usr/local/sbin:/usr/local/bin"
					":/usr/sbin:/usr/bin:/sbin:/bin"));
			env->i = 0;
		}
		else if (cmd[j][i] == '=' || cmd[j][i] == '\0')
		{
			push_env(env, new_env(cmd[j]));
			push_export(exprt, new_env(cmd[j]));
		}
		else if (cmd[j][i] == '+' && cmd[j][i + 1] == '=')
		{
			append_env(env, new_env(cmd[j]));
			append_export(exprt, new_env(cmd[j]));
		}
		else
			perror("syntax error");
		j++;
	}
	return (0);
}
