/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:43:56 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/22 10:30:24 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_echo(t_arg *arg, int i, int no_cmds, int p_fd[3])
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
		builtin_echo(arg->all_cmds[i].cmd);
		(freencmds(arg->all_cmds, no_cmds), free_env(arg->env),
			free_env(arg->export));
		exit(0);
	}
	if (arg->all_cmds[i].fd)
		close(arg->all_cmds[i].fd);
	if (pid == -1)
		perror("fork");
	if (i == no_cmds - 1)
		return (pid);
	return (0);
}

int	execute_pwd(t_arg *arg, int i, int no_cmds, int p_fd[3])
{
	pid_t	pid;

	pid = fork();
	if (!pid)
	{
		close_heredocs3(arg->all_cmds, i);
		if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
		{
			(freencmds(arg->all_cmds, no_cmds), free_env(arg->env),
				free_env(arg->export));
			(get_pwd(2), exit(errno_to_estatus()));
		}
		builtin_pwd();
		get_pwd(2);
		(freencmds(arg->all_cmds, no_cmds), free_env(arg->env),
			free_env(arg->export));
		exit(0);
	}
	if (arg->all_cmds[i].fd)
		close(arg->all_cmds[i].fd);
	if (pid == -1)
		perror("fork");
	if (i == no_cmds - 1)
		return (pid);
	return (0);
}

int	execute_exit(t_arg *arg, int i, int no_cmds, int p_fd[3])
{
	pid_t	pid;

	if (no_cmds == 1)
	{
		if (arg->all_cmds[i].fd)
		{
			close(arg->all_cmds[i].fd);
			arg->all_cmds[i].fd = 0;
		}
		if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
			return (errno_to_estatus());
		return (builtin_exit(arg, no_cmds, i, 0) * -1);
	}
	pid = execute_exit_pipe(arg, i, no_cmds, p_fd);
	if (arg->all_cmds[i].fd)
		close(arg->all_cmds[i].fd);
	if (pid == -1)
		perror("fork");
	if (i == no_cmds - 1)
		return (pid);
	return (0);
}

int	execute_unset(t_arg *arg, int i, int p_fd[3])
{
	int		no_cmds;
	int		tmp;
	pid_t	pid;

	no_cmds = count_cmds(arg->all_cmds);
	if (no_cmds == 1)
	{
		tmp = dup(1);
		if (tmp == -1)
			return (perror("dup"), -1);
		if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
			return (close(tmp), errno_to_estatus() * -1);
		unset(arg->all_cmds[i].cmd, arg->env, arg->export);
		if (dup2(tmp, 1) == -1)
			return (perror("dup2"), close(tmp), -1);
		return (close(tmp), 0);
	}
	pid = execute_unset_pipe(arg, i, p_fd, no_cmds);
	if (arg->all_cmds[i].fd)
		close(arg->all_cmds[i].fd);
	if (pid == -1)
		perror("fork");
	if (i == no_cmds - 1)
		return (pid);
	return (0);
}

int	execute_env(t_arg *arg, int i, int p_fd[3])
{
	int		no_cmds;
	pid_t	pid;

	no_cmds = count_cmds(arg->all_cmds);
	pid = fork();
	if (!pid)
	{
		(get_pwd(2), close_heredocs3(arg->all_cmds, i));
		if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
		{
			(freencmds(arg->all_cmds, no_cmds), free_env(arg->env));
			(free_env(arg->export), exit(errno_to_estatus()));
		}
		display_env(arg->env);
		(freencmds(arg->all_cmds, no_cmds), free_env(arg->env));
		(free_env(arg->export), exit(0));
	}
	if (arg->all_cmds[i].fd)
		close(arg->all_cmds[i].fd);
	if (pid == -1)
		perror("fork");
	if (i == no_cmds - 1)
		return (pid);
	return (0);
}
