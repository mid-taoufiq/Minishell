/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:44:58 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/23 09:01:49 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_others(t_arg *arg, int i, int no_cmds)
{
	char	*cmd_path;
	char	**dblenv;

	dblenv = envlst_to_array(arg->env);
	if (!arg->all_cmds[i].cmd[0] || !dblenv)
	{
		(freencmds(arg->all_cmds, no_cmds), freedbl((void **)dblenv),
			free_env(arg->env), free_env(arg->export));
		if (!dblenv)
			exit(1);
		exit(0);
	}
	cmd_path = check_commands(arg->env, arg->all_cmds[i].cmd[0]);
	if (!cmd_path)
	{
		(freencmds(arg->all_cmds, no_cmds),
			free_env(arg->env), free_env(arg->export));
		(freedbl((void **)dblenv), exit(errno_to_estatus()));
	}
	(free_env(arg->env), free_env(arg->export));
	execve(cmd_path, arg->all_cmds[i].cmd, dblenv);
	(freencmds(arg->all_cmds, no_cmds), freedbl((void **)dblenv));
	if (access(cmd_path, X_OK))
		(perror("execve"), free(cmd_path), exit(errno_to_estatus()));
	(free(cmd_path), exit(0));
}

static void	free_all_(t_arg *arg, int no_cmds)
{
	freencmds(arg->all_cmds, no_cmds);
	free_env(arg->env);
	free_env(arg->export);
}

int	execute_others_main(t_arg *arg, int i, int p_fd[3])
{
	int		no_cmds;
	pid_t	pid;

	no_cmds = count_cmds(arg->all_cmds);
	g_herdoc_signal = 1;
	pid = fork();
	if (!pid)
	{
		(get_pwd(2), signal(SIGQUIT, sigquit_handler));
		close_heredocs3(arg->all_cmds, i);
		if (redirect(arg->all_cmds[i], p_fd, i, no_cmds) == -1)
			(free_all_(arg, no_cmds), exit(1));
		execute_others(arg, i, no_cmds);
	}
	if (arg->all_cmds[i].fd)
		close(arg->all_cmds[i].fd);
	if (pid == -1)
		perror("fork");
	return (pid);
}
