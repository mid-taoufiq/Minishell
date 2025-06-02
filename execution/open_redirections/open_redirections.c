/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:21:50 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/27 15:56:43 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirection_type2(int *fd0, t_cmd all_cmds);

int	write_in_file(int args[4], char *lim, int p_fd[3], t_arg *arg)
{
	int		status;
	pid_t	pid;

	g_herdoc_signal = 1;
	pid = fork();
	if (pid == -1)
		return (-1);
	if (!pid)
	{
		close_heredocs(arg);
		if (args[2] > 1)
			(close(p_fd[0]), close(p_fd[1]));
		if (p_fd[2])
			close(p_fd[2]);
		write_in_file_child(lim, args, arg);
		(freencmds(arg->all_cmds, args[2]), free_env(arg->env));
		(free_env(arg->export), exit(0));
	}
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 130)
		return (g_herdoc_signal = 0, -1);
	else if (WEXITSTATUS(status) == 1)
		return (g_herdoc_signal = 0, -2);
	g_herdoc_signal = 0;
	return (args[0]);
}

int	open_heredoc(char *lim, int p_fd[3], int args[2], t_arg *arg)
{
	int		fd[4];
	char	*filename;
	int		n;

	filename = get_filename(fd, 0);
	if (!filename)
		return (-2);
	fd[1] = open(filename, O_RDONLY);
	if (fd[1] == -1)
		return (perror("heredoc"), free(filename), -1);
	unlink(filename);
	fd[2] = args[0];
	fd[3] = args[1];
	free(filename);
	n = write_in_file(fd, lim, p_fd, arg);
	if (n < 0)
		return (close(fd[1]), close(fd[0]), n);
	close(fd[0]);
	return (fd[1]);
}

static int	init(int nth, int no_cmds, int pfd[2], int *fd0)
{
	if (nth == 0 && no_cmds != 1)
	{
		if (dup2(pfd[1], 1) == -1)
			return (close(pfd[1]), close(pfd[0]), 1);
		(close(pfd[1]), close(pfd[0]));
	}
	else if (nth == no_cmds - 1 && no_cmds != 1)
		*fd0 = pfd[0];
	else if (no_cmds != 1 && nth != no_cmds - 1)
	{
		if (dup2(pfd[1], 1) == -1)
			return (close(pfd[1]), close(pfd[0]), 1);
		(close(pfd[1]), close(pfd[0]));
		*fd0 = pfd[2];
	}
	return (0);
}

int	call_red_functions(int red, t_cmd all_cmds, int *fd0)
{
	if (all_cmds.redirection[red].type == 0)
	{
		check_0_fd(*fd0);
		*fd0 = open_infile(all_cmds.redirection[red].file,
				all_cmds.redirection[red].error);
		if (*fd0 == -1)
			return (check_0_fd(all_cmds.fd), -1);
	}
	else if (all_cmds.redirection[red].type == 1)
	{
		if (open_outfile(all_cmds.redirection[red].file,
				all_cmds.redirection[red].error) == -1)
			return (check_0_fd(*fd0), check_0_fd(all_cmds.fd), -1);
	}
	else if (all_cmds.redirection[red].type == 2)
		redirection_type2(fd0, all_cmds);
	else if (all_cmds.redirection[red].type == 3)
	{
		if (open_append_file(all_cmds.redirection[red].file,
				all_cmds.redirection[red].error) == -1)
		{
			return (check_0_fd(*fd0), check_0_fd(all_cmds.fd), -1);
		}
	}
	return (0);
}

int	redirect(t_cmd all_cmds, int pfd[2], int nth, int no_cmds)
{
	int	red;
	int	fd0;

	red = 0;
	fd0 = 0;
	if (init(nth, no_cmds, pfd, &fd0))
		return (-1);
	while (all_cmds.redirection[red].file != NULL)
	{
		if (call_red_functions(red, all_cmds, &fd0))
			return (-1);
		red++;
	}
	if (fd0 != 0)
	{
		if (dup2(fd0, 0) == -1)
			return (close(fd0), -1);
		close(fd0);
	}
	return (1);
}
