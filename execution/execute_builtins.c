/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:43:56 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/12 15:52:44 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_echo(t_cmd *all_cmds, int i, int no_cmds, int p_fd[3])
{
	if (!fork())
	{
		if (all_cmds[i].fd)
		{
			close(all_cmds[i].fd);
			all_cmds[i].fd = 0;
		}
		if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
			exit(errno_to_estatus());
		builtin_echo(all_cmds[i].cmd);
		exit(0);
	}
	return (0);
}

int execute_pwd(t_cmd *all_cmds, int i, int no_cmds, int p_fd[3])
{
	if (!fork())
	{
		if (all_cmds[i].fd)
		{
			close(all_cmds[i].fd);
			all_cmds[i].fd = 0;
		}
		if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
			exit(errno_to_estatus());
		builtin_pwd();
		exit(0);
	}
	return (0);
}

int	execute_exit(t_cmd *all_cmds, int i, int no_cmds, int p_fd[3])
{
	if (no_cmds == 1)
	{
		if (all_cmds[i].fd)
		{
			close(all_cmds[i].fd);
			all_cmds[i].fd = 0;
		}
		if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
			return (errno_to_estatus());
		builtin_exit(all_cmds[i].cmd, no_cmds);
		return (0);
	}
	if (!fork())
	{
		if (all_cmds[i].fd)
		{
			close(all_cmds[i].fd);
			all_cmds[i].fd = 0;
		}
		if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
			exit(errno_to_estatus());
		builtin_exit(all_cmds[i].cmd, no_cmds);
		exit(0);
	}
	return (0);
}

int	execute_unset(t_cmd *all_cmds, int i, t_arg arg, int p_fd[3])
{
	int	no_cmds;
	int	tmp;

	no_cmds = count_cmds(all_cmds);
	if (no_cmds == 1)
	{
		tmp = dup(1);
		if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
			return (close(tmp), errno_to_estatus());
		unset(all_cmds[i].cmd, arg.env, arg.export);
		dup2(tmp, 1);
		close(tmp);
		return (0);
	}
	if (!fork())
	{
		if (all_cmds[i].fd)
		{
			close(all_cmds[i].fd);
			all_cmds[i].fd = 0;
		}
		if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
			exit(errno_to_estatus());
		unset(all_cmds[i].cmd, arg.env, arg.export);
		exit(0);
	}
	return (0);
}

int execute_env(t_cmd *all_cmds, int i, t_env *env, int p_fd[3])
{
	int	no_cmds;

	no_cmds = count_cmds(all_cmds);
	if (!fork())
	{
		if (all_cmds[i].fd)
		{
			close(all_cmds[i].fd);
			all_cmds[i].fd = 0;
		}
		if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
			exit(errno_to_estatus());
		display_env(env);
		exit(0);
	}
	return (0);
}

int	execute_cd(t_cmd *all_cmds, int i, t_arg arg, int p_fd[3])
{
	int	tmp;
	int	no_cmds;
	int	status;

	no_cmds = count_cmds(all_cmds);
	tmp = dup(1);
	if (all_cmds[i].fd)
	{
		close(all_cmds[i].fd);
		all_cmds[i].fd = 0;
	}
	if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
		return (errno_to_estatus());
	status =  builtin_cd(all_cmds[i].cmd, no_cmds, arg.env, arg.export);
	get_pwd(0);
	(dup2(tmp, 1), close(tmp));
	return (status);
}

int	execute_export(t_cmd *all_cmds, int i, t_arg arg, int p_fd[3])
{
	int	tmp;
	int	no_cmds;

	no_cmds = count_cmds(all_cmds);
	if (no_cmds != 1)
	{
		if (!fork())
		{
			if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
			{
				(freencmds(all_cmds, no_cmds), free_env(arg.env), free_env(arg.export));
				exit(errno_to_estatus());
			}
			export(arg.env, arg.export, all_cmds[i].cmd);
			(close(p_fd[0]), close(p_fd[1]));
			exit(0);
		}
		return (0);
	}
	tmp = dup(1);
	if (all_cmds[i].fd)
	{
		close(all_cmds[i].fd);
		all_cmds[i].fd = 0;
	}
	if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
	{
		close(tmp);
		return (errno_to_estatus());
	}
	export(arg.env, arg.export, all_cmds[i].cmd);
	dup2(tmp, 1);
	close(tmp);
	return (0);
}
