/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:27:21 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/06 16:41:49 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmds(t_cmd *cmds)
{
	int	i;

	i = 0;
	while (cmds[i].cmd)
		i++;
	return (i);
}

void	close_heredocs(t_cmd *all_cmds)
{
	int	i;

	i = 0;
	while (all_cmds[i].cmd)
	{
		if (all_cmds[i].fd)
			close(all_cmds[i].fd);
		i++;
	}
}

int	here_doc(t_cmd *all_cmds)
{
	int	i;
	int	red;

	i = 0;
	red = 0;
	while (all_cmds[i].cmd)
	{
		red = 0;
		while (all_cmds[i].redirection[red].file != NULL)
		{
			if (all_cmds[i].redirection[red].type == 2)
			{
				if (all_cmds[i].fd != 0)
					close(all_cmds[i].fd);
				all_cmds[i].fd = open_heredoc(all_cmds[i].redirection[red].file);
				if (all_cmds[i].fd == -1)
					return (-1);
			}
			red++;
		}
		i++;
	}
	return (0);
}

int	execute(t_cmd *all_cmds, t_env *env, t_env *exprt)
{
	int		i;
	int		no_cmds;
	int		p_fd[3];
	int		status;
	int		tmp;
	pid_t	pid;

	i = 0;
	status = 0;
	p_fd[2] = 0;
	no_cmds = count_cmds(all_cmds);
	if (no_cmds != 1)
		pipe(p_fd);
	if (here_doc(all_cmds) == -1)
		return (close(p_fd[0]), close(p_fd[1]), -1);
	while (all_cmds[i].cmd)
	{
		if (i != 0 && i != no_cmds -1)
		{
			close(p_fd[1]);
			if (p_fd[2])
				close(p_fd[2]);
			p_fd[2] = p_fd[0];
			pipe(p_fd);
		}
		else if (i == no_cmds - 1 && no_cmds != 1)
			close(p_fd[1]);
		if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "export"))
		{
			if (no_cmds != 1)
			{
				if (!fork())
				{
					if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
					{
						(freencmds(all_cmds, no_cmds), free_env(env), free_env(exprt));
						exit(1);
					}
					export(env, exprt, all_cmds[i].cmd);
					(close(p_fd[0]), close(p_fd[1]));
					exit(0);
				}
				i++;
				continue ;
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
				i++;
				continue ;
			}
			export(env, exprt, all_cmds[i].cmd);
			dup2(tmp, 1);
			close(tmp);
		}
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "echo"))
			status = execute_echo(all_cmds, i, no_cmds, p_fd);
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "cd"))
		{
			tmp = dup(1);
			if (all_cmds[i].fd)
			{
				close(all_cmds[i].fd);
				all_cmds[i].fd = 0;
			}
			if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
			{
				i++;
				continue ;
			}
			builtin_cd(all_cmds[i].cmd, no_cmds, env, exprt);
			(dup2(tmp, 1), close(tmp));
		}
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "pwd"))
			status = execute_pwd(all_cmds, i, no_cmds, p_fd);
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "exit"))
			status = execute_exit(all_cmds, i, no_cmds, p_fd);
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "unset"))
			status = execute_unset(all_cmds, i, env, p_fd);
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "env"))
			status = execute_env(all_cmds, i, env, p_fd);
		else
		{
			pid = fork();
			if (!pid)
			{
				if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
					(freencmds(all_cmds, no_cmds), free_env(env), free_env(exprt), exit(1));
				execute_others(all_cmds[i], all_cmds, env, exprt);
			}
			if (p_fd[2])
				(close(p_fd[2]), p_fd[2] = 0);
			if (WIFSIGNALED(status))
				return(g_herdoc_signal = 0, -1);
			g_herdoc_signal = 0;
		}
		i++;
	}
	close_heredocs(all_cmds);
	if (no_cmds != 1)
		(close(p_fd[0]), close(p_fd[1]));
	if (p_fd[2])
		close(p_fd[2]);
	while (wait(&status) >= 0)
		continue ;
	return (WEXITSTATUS(status));
}
