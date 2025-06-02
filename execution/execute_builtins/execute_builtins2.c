/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:32:58 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/28 10:13:05 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	execute_pipe_cd(t_arg *arg, int no_cmds, int p_fd[3], int i)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (!pid)
	{
		(get_pwd(2), close_heredocs3(arg->all_cmds, i));
		if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
		{
			(freencmds(arg->all_cmds, no_cmds), free_env(arg->env));
			(free_env(arg->export), exit(errno_to_estatus()));
		}
		status = builtin_cd(arg->all_cmds[i].cmd,
				no_cmds, arg->env, arg->export);
		(freencmds(arg->all_cmds, no_cmds), free_env(arg->env));
		(free_env(arg->export), exit(status));
	}
	return (pid);
}

static int	cd_norm(int tmp, int status)
{
	get_pwd(0);
	if (dup2(tmp, 1) == -1)
		return (close(tmp), -1);
	close(tmp);
	return (status * -1);
}

int	execute_cd(t_arg *arg, int i, int p_fd[3])
{
	int		tmp;
	int		no_cmds;
	int		status;
	pid_t	pid;

	no_cmds = count_cmds(arg->all_cmds);
	if (no_cmds != 1)
	{
		pid = execute_pipe_cd(arg, no_cmds, p_fd, i);
		if (pid == -1)
			perror("cd");
		if (i == no_cmds -1)
			return (pid);
		return (0);
	}
	tmp = dup(1);
	if (!tmp)
		return (perror("dup"), -1);
	if (arg->all_cmds[i].fd)
		(close(arg->all_cmds[i].fd), arg->all_cmds[i].fd = 0);
	if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
		return (errno_to_estatus());
	status = builtin_cd(arg->all_cmds[i].cmd, no_cmds, arg->env, arg->export);
	return (cd_norm(tmp, status));
}

int	execute_export(t_arg *arg, int i, int p_fd[3])
{
	int		tmp;
	int		no_cmds;
	pid_t	pid;
	int		status;

	no_cmds = count_cmds(arg->all_cmds);
	if (no_cmds != 1)
	{
		pid = execute_export_pipe(arg, i, p_fd, no_cmds);
		if (arg->all_cmds[i].fd)
			close(arg->all_cmds[i].fd);
		if (pid == -1)
			perror("fork");
		if (i == no_cmds - 1)
			return (pid);
		return (0);
	}
	tmp = dup(1);
	if (arg->all_cmds[i].fd)
		(close(arg->all_cmds[i].fd), arg->all_cmds[i].fd = 0);
	if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
		return (close(tmp), errno_to_estatus());
	status = export(arg->env, arg->export, arg->all_cmds[i].cmd);
	return (dup2(tmp, 1), close(tmp), status * -1);
}
