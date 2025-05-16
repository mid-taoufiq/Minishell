/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:44:58 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/15 13:20:05 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_dbl_ptr(char **ptr, int i)
{
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

int	check_dir(char *path)
{
	struct stat	info;

	stat(path, &info);
	if (S_ISDIR(info.st_mode))
	{
		errno = EACCES;
		return (1);
	}
	return (0);
}

char	*check_commands(t_env *env, char *cmd)
{
	int		i;
	char	**paths;
	char	*file_path;
	char	*tmp;

	if (cmd[0] == '\0')
		return (access(cmd, X_OK), perror("\"\""), NULL);
	if (!access(cmd, X_OK) && ft_strchr(cmd, '/') && !check_dir(cmd))
		return (ft_strdup(cmd));
	if (ft_strchr(cmd, '/') && (access(cmd, X_OK) || check_dir(cmd)))
		return (perror(cmd), NULL);
	tmp = ft_getenv(env, "PATH");
	if (!tmp)
	{
		if (env->i)
			tmp = ft_strdup("/usr/local/sbin:/usr/local/bin"
					":/usr/sbin:/usr/bin:/sbin:/bin");
		else if (access(cmd, X_OK) == 0 && !check_dir(cmd))
			return (ft_strdup(cmd));
		else
			return (perror(cmd), NULL);
	}
	paths = ft_split(tmp, ':');
	free(tmp);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		free(paths[i]);
		file_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(file_path, X_OK) == 0 && !check_dir(file_path))
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

int	errno_to_estatus(void)
{
	if (errno == EACCES)
		return (126);
	else if (errno == ENOENT)
		return (127);
	return (1);
}

void	close_heredocs3(t_cmd *all_cmds, int cmd)
{
	int	i;

	i = 0;
	while (all_cmds[i].cmd)
	{
		if (i == cmd)
		{
			i++;
			continue;
		}
		if (all_cmds[i].fd)
			close(all_cmds[i].fd);
		i++;
	}
}

int	execute_others(t_arg *arg, int i, int no_cmds)
{
	char	*cmd_path;
	char	**dblenv;

	dblenv = envlst_to_array(arg->env);
	if (!arg->all_cmds[i].cmd[0] || !dblenv)
	{
		(freencmds(arg->all_cmds, no_cmds), free_env(arg->env), free_env(arg->export));
		exit(1);
	}
	cmd_path = check_commands(arg->env, arg->all_cmds[i].cmd[0]);
	if (!cmd_path)
	{
		(freencmds(arg->all_cmds, no_cmds), free_env(arg->env), free_env(arg->export));
		exit(errno_to_estatus());
	}
	(free_env(arg->env), free_env(arg->export));
	execve(cmd_path, arg->all_cmds[i].cmd, dblenv);
	if (access(cmd_path, X_OK))
		perror("execve");
	(freencmds(arg->all_cmds, no_cmds), free_dbl_ptr(dblenv, 0));
	exit(errno_to_estatus());
}

int	execute_others_main(t_arg *arg, int i, int p_fd[3])
{
	int		no_cmds;
	pid_t	pid;
	int		status;

	no_cmds = count_cmds(arg->all_cmds);
	g_herdoc_signal = 1;
	pid = fork();
	if (!pid)
	{
		get_pwd(2);
		signal(SIGQUIT, sigquit_handler);
		close_heredocs3(arg->all_cmds, i);
		if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
		{
			(freencmds(arg->all_cmds, no_cmds), free_env(arg->env),
				free_env(arg->export), exit(1));
		}
		if (!fork())
			execute_others(arg, i, no_cmds);
		wait(&status);
		(freencmds(arg->all_cmds, no_cmds), free_env(arg->env),
			free_env(arg->export));
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				(printf("\n"), exit(130));
			else if (WTERMSIG(status) == SIGQUIT)
				(printf("Quit (core dumped)\n"), exit(131));
		}
		exit(WEXITSTATUS(status));
	}
	if (arg->all_cmds[i].fd)
			close(arg->all_cmds[i].fd);
	return (0);
}
