/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:27:21 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/03 14:28:36 by ayel-arr         ###   ########.fr       */
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

int	here_doc(t_cmd *all_cmds)
{
	int	i;
	int	red;

	i = 0;
	red = 0;
	while (all_cmds[i].cmd)
	{
		while (all_cmds[i].redirection[red].file != NULL)
		{
			if (all_cmds[i].redirection[red].type == 2)
			{
				if (all_cmds[i].fd != 0)
					close(all_cmds[i].fd);
				all_cmds[i].fd = open_heredoc(all_cmds[i].redirection[red].file);
			}
			red++;
		}
		i++;
	}
	return (0);
}

int	execute(t_cmd *all_cmds, t_env *env, t_env *exprt, char **o_env)
{
	int		i;
	char	*cmd_path;
	int		no_cmds;
	int		p_fd[3];
	int		status;

	i = 0;
	status = 0;
	no_cmds = count_cmds(all_cmds);
	if (no_cmds != 1)
		pipe(p_fd);
	here_doc(all_cmds);
	while (all_cmds[i].cmd)
	{
		if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "export"))
			export(env, exprt, all_cmds[i].cmd);
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "echo"))
			builtin_echo(all_cmds[i].cmd);
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "cd"))
			builtin_cd(all_cmds[i].cmd, no_cmds, env, exprt);
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "pwd"))
			builtin_pwd();
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "exit"))
			builtin_exit(all_cmds[i].cmd, no_cmds);
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "unset"))
			unset(all_cmds[i].cmd, env);
		else if (all_cmds[i].cmd[0] && !ft_strcmp(all_cmds[i].cmd[0], "env"))
			display_env(env);
		else
		{
			if (all_cmds[i].fd == -1)
			{
				i++;
				continue ;
			}
			if (i != 0 && i != no_cmds -1)
			{
				close(p_fd[1]);
				p_fd[2] = p_fd[0];
				pipe(p_fd);
			}
			else if (i == no_cmds - 1 && no_cmds != 1)
				close(p_fd[1]);
			if (!fork())
			{
				if (redirect(all_cmds[i], p_fd, i, no_cmds) == -1)
				{
					(freencmds(all_cmds, no_cmds), free_env(env), free_env(exprt));
					exit(1);
				}
				if (!all_cmds[i].cmd[0])
				{
					(freencmds(all_cmds, no_cmds), free_env(env), free_env(exprt));
					exit(1);
				}
				cmd_path = check_commands(env, all_cmds[i].cmd[0]);
				if (!cmd_path)
				{
					(freencmds(all_cmds, no_cmds), free_env(env), free_env(exprt));
					exit(1);
				}
				execve(cmd_path, all_cmds[i].cmd, o_env);
				perror("execve");
				(freencmds(all_cmds, no_cmds), free_env(env), free_env(exprt));
				exit(1);
			}
		}
		i++;
	}
	if (no_cmds != 1)
		(close(p_fd[0]), close(p_fd[1]));
	while (wait(&status) >= 0)
		continue ;
	return (status % 255);
}
