/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:07:54 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/28 10:13:45 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_exit_pipe(t_arg *arg, int i, int no_cmds, int p_fd[3])
{
	pid_t	pid;

	pid = fork();
	if (!pid)
	{
		get_pwd(2);
		close_heredocs3(arg->all_cmds, i);
		if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
		{
			(freencmds(arg->all_cmds, no_cmds), free_env(arg->env),
				free_env(arg->export));
			exit(errno_to_estatus());
		}
		builtin_exit(arg, no_cmds, i, 0);
		exit(0);
	}
	return ((int)pid);
}

int	execute_unset_pipe(t_arg *arg, int i, int p_fd[3], int no_cmds)
{
	pid_t	pid;

	pid = fork();
	if (!pid)
	{
		get_pwd(2);
		close_heredocs3(arg->all_cmds, i);
		if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
		{
			(freencmds(arg->all_cmds, no_cmds), free_env(arg->env),
				free_env(arg->export));
			exit(errno_to_estatus());
		}
		unset(arg->all_cmds[i].cmd, arg->env, arg->export);
		(freencmds(arg->all_cmds, no_cmds), free_env(arg->env),
			free_env(arg->export));
		exit(0);
	}
	return ((int)pid);
}

int	execute_export_pipe(t_arg *arg, int i, int p_fd[3], int no_cmds)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (!pid)
	{
		get_pwd(2);
		close_heredocs3(arg->all_cmds, i);
		if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
		{
			(freencmds(arg->all_cmds, no_cmds), free_env(arg->env));
			(free_env(arg->export), exit(errno_to_estatus()));
		}
		status = export(arg->env, arg->export, arg->all_cmds[i].cmd);
		(freencmds(arg->all_cmds, no_cmds), free_env(arg->env));
		(free_env(arg->export), exit(status));
	}
	return ((int)pid);
}
