/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:44:58 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/06 16:41:59 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void free_dbl_ptr(char **ptr, int i)
{
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

char	*check_commands(t_env *env, char *cmd)
{
	int		i;
	char	**paths;
	char	*file_path;
	char	*tmp;

	if (access(cmd, X_OK) == 0 && ft_strchr(cmd, '/') != NULL)
		return (ft_strdup(cmd));
	if (ft_strchr(cmd, '/') != NULL && access(cmd, X_OK) != 0)
		return (perror(cmd), NULL);
	if (!(tmp = ft_getenv(env, "PATH")))
		return (free(tmp), perror(cmd), NULL);
	paths = ft_split(tmp, ':');
	free(tmp);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		free(paths[i]);
		file_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(file_path, X_OK) == 0)
			return (free_dbl_ptr(paths, i + 1), file_path);
		free(file_path);
		i++;
	}
	return (free(paths), perror(cmd), NULL);
}

char	**envlst_to_array(t_env *env)
{
	int		i;
	char	**ret;
	t_env	*tmp;
	char	*ttmp;

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
	i = 0;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "?"))
		{
			tmp = tmp->next;
			continue ;
		}
		ttmp = ft_strjoin(tmp->key, "=");
		if (!ttmp)
			return (perror("malloc"), NULL);
		ret[i] = ft_strjoin(ttmp, tmp->value);
		if (!ret[i])
			return (free_dbl_ptr(ret, 0), perror("malloc"), NULL);
		free(ttmp);
		tmp = tmp->next;
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

int	execute_others(t_cmd cmd, t_cmd *all_cmds, t_env *env, t_env *exprt)
{
	char	*cmd_path;
	int		no_cmds;
	char	**dblenv;

	g_herdoc_signal = 2;
	no_cmds = count_cmds(all_cmds);
	dblenv = envlst_to_array(env);
	if (!cmd.cmd[0] || !dblenv)
	{
		(freencmds(all_cmds, no_cmds), free_env(env), free_env(exprt));
		exit(1);
	}
	cmd_path = check_commands(env, cmd.cmd[0]);
	if (!cmd_path)
	{
		(freencmds(all_cmds, no_cmds), free_env(env), free_env(exprt));
		exit(1);
	}
	execve(cmd_path, cmd.cmd, dblenv);
	perror("execve");
	(freencmds(all_cmds, no_cmds), free_env(env), free_env(exprt));
	exit(1);
}
