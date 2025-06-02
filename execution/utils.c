/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:01:36 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/28 09:01:38 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_msg(int sig, int last_sig);

int	pre_execution(t_arg *arg, int *no_cmds, int p_fd[3])
{
	int	i;

	p_fd[2] = 0;
	*no_cmds = count_cmds(arg->all_cmds);
	if (*no_cmds != 1)
	{
		if (pipe(p_fd))
			return (1);
	}
	i = here_doc(arg, p_fd, *no_cmds);
	if (i < 0)
	{
		close_heredocs(arg);
		if (*no_cmds > 1)
			(close(p_fd[0]), close(p_fd[1]));
		if (i == -1)
			return (130);
		return (1);
	}
	return (0);
}

int	pipe_shit(int i, int no_cmds, int p_fd[3])
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
	return (0);
}

int	execute_builtins(t_arg *arg, int i, int *status, int p_fd[3])
{
	int	no_cmds;

	no_cmds = count_cmds(arg->all_cmds);
	if (arg->all_cmds[i].cmd[0]
		&& !ft_strcmp(arg->all_cmds[i].cmd[0], "export"))
		return (*status = execute_export(arg, i, p_fd), 1);
	else if (arg->all_cmds[i].cmd[0]
		&& !ft_strcmp(arg->all_cmds[i].cmd[0], "echo"))
		return (*status = execute_echo(arg, i, no_cmds, p_fd), 1);
	else if (arg->all_cmds[i].cmd[0]
		&& !ft_strcmp(arg->all_cmds[i].cmd[0], "cd"))
		return (*status = execute_cd(arg, i, p_fd), 1);
	else if (arg->all_cmds[i].cmd[0]
		&& !ft_strcmp(arg->all_cmds[i].cmd[0], "pwd"))
		return (*status = execute_pwd(arg, i, no_cmds, p_fd), 1);
	else if (arg->all_cmds[i].cmd[0]
		&& !ft_strcmp(arg->all_cmds[i].cmd[0], "exit"))
		return (*status = execute_exit(arg, i, no_cmds, p_fd), 1);
	else if (arg->all_cmds[i].cmd[0]
		&& !ft_strcmp(arg->all_cmds[i].cmd[0], "unset"))
		return (*status = execute_unset(arg, i, p_fd), 1);
	else if (arg->all_cmds[i].cmd[0]
		&& !ft_strcmp(arg->all_cmds[i].cmd[0], "env"))
		return (*status = execute_env(arg, i, p_fd), 1);
	return (0);
}

static void	wait_processes(int last_sig)
{
	int	tmp;
	int	sig;

	sig = 0;
	while (wait(&tmp) >= 0)
	{
		if (WIFSIGNALED(tmp))
		{
			if (WTERMSIG(tmp) == SIGINT || WTERMSIG(tmp) == SIGPIPE)
				sig = 1;
			else if (WTERMSIG(tmp) == SIGQUIT)
				sig = 2;
		}
	}
	sig_msg(sig, last_sig);
	return ;
}

int	execution_epilogue(int no_cmds, int p_fd[3], int *status)
{
	int	last_sig;

	last_sig = 0;
	if (no_cmds != 1)
		(close(p_fd[0]), close(p_fd[1]));
	if (p_fd[2])
		close(p_fd[2]);
	if (*status > 0)
	{
		waitpid((pid_t)(*status), status, 0);
		if (WIFSIGNALED(*status))
		{
			if (WTERMSIG(*status) == SIGINT || WTERMSIG(*status) == SIGPIPE)
				last_sig = 1;
			else if (WTERMSIG(*status) == SIGQUIT)
				last_sig = 2;
		}
	}
	wait_processes(last_sig);
	if (*status <= 0)
		return (*status * -1);
	if (last_sig)
		return (129 + last_sig);
	return (WEXITSTATUS(*status));
}
