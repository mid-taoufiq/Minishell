/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:27:21 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/15 14:47:12 by ayel-arr         ###   ########.fr       */
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

void	close_heredocs(t_arg *arg)
{
	int	i;

	i = 0;
	while (arg->all_cmds[i].cmd)
	{
		if (arg->all_cmds[i].fd)
			close(arg->all_cmds[i].fd);
		i++;
	}
}

void	for_norm(t_cmd cmd, int args[2], int no_cmds, int red)
{
	if (cmd.fd != 0)
		close(cmd.fd);
	args[0] = no_cmds;
	args[1] = cmd.redirection[red].expandable;
}

int	here_doc(t_arg *arg, int p_fd[3], int no_cmds)
{
	int	i;
	int	red;
	int	args[2];

	i = 0;
	while (arg->all_cmds[i].cmd)
	{
		red = 0;
		while (arg->all_cmds[i].redirection[red].file != NULL)
		{
			if (arg->all_cmds[i].redirection[red].type == 2)
			{
				for_norm(arg->all_cmds[i], args, no_cmds, red);
				arg->all_cmds[i].fd
					= open_heredoc(arg->all_cmds[i].redirection[red].file,
						p_fd, args, arg);
				args[1] = arg->all_cmds[i].fd;
				if (arg->all_cmds[i].fd < 0)
					return (arg->all_cmds[i].fd = 0, args[1]);
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
	t_arg	arg;

	arg.env = env;
	arg.export = exprt;
	arg.all_cmds = all_cmds;
	status = pre_execution(&arg, &no_cmds, p_fd);
	if (status)
		return (status);
	i = 0;
	while (all_cmds[i].cmd)
	{
		pipe_shit(i, no_cmds, p_fd);
		if (execute_builtins(&arg, i, &status, p_fd))
			;
		else
			execute_others_main(&arg, i, p_fd);
		if (p_fd[2])
			(close(p_fd[2]), p_fd[2] = 0);
		i++;
	}
	return (execution_epilogue(no_cmds, p_fd, &status), WEXITSTATUS(status));
}
