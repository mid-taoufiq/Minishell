/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:21:50 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/06 12:32:39 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_herdoc_signal = 0;

int	open_infile(char	*filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror("open"), -1);
	return (fd);
}

int open_outfile(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
		return (perror("open"), -1);
	dup2(fd, 1);
	close(fd);
	return (0);
}

int	open_append_file(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (fd == -1)
		return (perror("open"), -1);
	dup2(fd, 1);
	close(fd);
	return (0);
}

int	write_in_file(int fd[2], char *lim)
{
	char	*line;
	int		status;
	pid_t	pid;

	g_herdoc_signal = 1;
	pid = fork();
	if (!pid)
	{
		close(fd[1]);
		dup2(fd[0], 2);
		close(fd[0]);
		signal(SIGINT, child_sigint);
		signal(SIGQUIT, SIG_IGN);
		line = readline("heredoc> ");
		while (line && ft_strcmp(line, lim))
		{
			write(2, line, ft_strlen(line));
			write(2, "\n", 1);
			free(line);
			line = readline("heredoc> ");
		}
		free(line);
		close(fd[0]);
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == -1)
		return (g_herdoc_signal = 0, -1);
	g_herdoc_signal = 0;
	return (fd[0]);
}

int	open_heredoc(char *lim)
{
	int		fd[2];
	char	*filename_template;
	int		n;
	char	*filename;
	char	*num;

	filename_template = "/tmp/.tmp_minishell_";
	n = 0;
	num = ft_itoa(n);
	filename = ft_strjoin(filename_template, num);
	fd[0] = open(filename, O_RDWR | O_CREAT | O_EXCL , 0777);
	while (fd[0] == -1)
	{
		(free(filename), free(num));
		n++;
		num = ft_itoa(n);
		filename = ft_strjoin(filename_template, num);
		if (!filename)
			return (free(num), -1);
		fd[0] = open(filename, O_RDWR | O_CREAT | O_EXCL, 0777);
	}
	fd[1] = open(filename, O_RDONLY);
	if (fd[1] == -1)
		return (perror("heredoc"), free(filename), free(num), -1);
	unlink(filename);
	if (write_in_file(fd, lim) == -1)
	{
		close(fd[1]);
		close(fd[0]);
		(free(filename), free(num));
		return (-1);
	}
	close(fd[0]);
	(free(filename), free(num));
	return (fd[1]);
}

int	redirect(t_cmd all_cmds, int pfd[2], int nth, int no_cmds)
{
	int	red;
	int fd0;

	red = 0;
	fd0 = all_cmds.fd;
	if (nth == 0 && no_cmds != 1)
		(dup2(pfd[1], 1), close(pfd[1]), close(pfd[0]));
	else if (nth == no_cmds - 1 && no_cmds != 1 && fd0 == 0)
		fd0 = pfd[0];
	else if (no_cmds != 1 && nth != no_cmds - 1)
	{
		dup2(pfd[1], 1);
		close(pfd[1]);
		close(pfd[0]);
		if (fd0 == 0)
			fd0 = pfd[2];
	}
	while (all_cmds.redirection[red].file != NULL)
	{
		if (all_cmds.redirection[red].type == 0)
		{
			if (fd0 != 0 && fd0 != -1)
				close(fd0);
			fd0 = open_infile(all_cmds.redirection[red].file);
			if (fd0 == -1)
				return (-1);
		}
		else if (all_cmds.redirection[red].type == 1)
		{	
			if (open_outfile(all_cmds.redirection[red].file) == -1)
				return (-1);
		}
		else if (all_cmds.redirection[red].type == 3)
		{
			if (open_append_file(all_cmds.redirection[red].file) == -1)
				return (-1);
		}
		red++;
	}
	if (fd0 != 0)
	{
		dup2(fd0, 0);
		close(fd0);
	}
	return (1);
}
