/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:21:50 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/15 13:43:41 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	space_separated(char *str)
{
	int	i;
	int	f;

	i = 0;
	f = 0;
	while (str[i])
	{
		if (!is_whitespace(str[i]))
			f = 1;
		else if (is_whitespace(str[i]) && !is_whitespace(str[i + 1]) && f == 1)
			return (1);
		i++;
	}
	return (0);
}

int	open_infile(char	*filename, char error)
{
	int	fd;

	if (error)
		return (perror("ambiguous redirect"), -1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror("open"), -1);
	return (fd);
}

int	open_outfile(char *filename, char error)
{
	int	fd;

	if (error)
		return (perror("ambiguous redirect"), -1);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
		return (perror("open"), -1);
	dup2(fd, 1);
	close(fd);
	return (0);
}

int	open_append_file(char *filename, char error)
{
	int	fd;

	if (error)
		return (perror("ambiguous redirect"), -1);
	fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (fd == -1)
		return (perror("open"), -1);
	dup2(fd, 1);
	close(fd);
	return (0);
}

int	write_in_file(int args[4], char *lim, int p_fd[3], t_arg *arg)
{
	char	*line;
	int		status;
	pid_t	pid;
	char	*tmp;

	g_herdoc_signal = 1;
	pid = fork();
	if (!pid)
	{
		close_heredocs(arg);
		if (args[2] > 1)
			(close(p_fd[0]), close(p_fd[1]));
		if (p_fd[2])
			close(p_fd[2]);
		close(args[1]);
		dup2(args[0], 2);
		close(args[0]);
		signal(SIGINT, child_sigint);
		signal(SIGQUIT, SIG_IGN);
		line = readline("heredoc> ");
		while (line && ft_strcmp(line, lim))
		{
			if (args[3])
			{
				tmp = line;
				line = expand_parse_heredoc(line, arg->env);
				if (!line)
					(free(tmp), close(args[0]), exit(1));
				free(tmp);
			}
			write(2, line, ft_strlen(line));
			write(2, "\n", 1);
			free(line);
			line = readline("heredoc> ");
		}
		free(line);
		close(args[0]);
		(freencmds(arg->all_cmds, args[2]), free_env(arg->env),
			free_env(arg->export));
		exit(0);
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
	char	*filename_template;
	int		n;
	char	*filename;
	char	*num;

	filename_template = "/tmp/.tmp_minishell_";
	n = 0;
	num = ft_itoa(n);
	if (!num)
		return (-2);
	filename = ft_strjoin(filename_template, num);
	if (!filename)
		return (free(num), -2);
	fd[0] = open(filename, O_RDWR | O_CREAT | O_EXCL, 0777);
	while (fd[0] == -1)
	{
		(free(filename), free(num));
		n++;
		num = ft_itoa(n);
		if (!num)
			return (-2);
		filename = ft_strjoin(filename_template, num);
		if (!filename)
			return (free(num), -2);
		fd[0] = open(filename, O_RDWR | O_CREAT | O_EXCL, 0777);
	}
	fd[1] = open(filename, O_RDONLY);
	if (fd[1] == -1)
		return (perror("heredoc"), free(filename), free(num), -1);
	unlink(filename);
	fd[2] = args[0];
	fd[3] = args[1];
	(free(filename), free(num));
	n = write_in_file(fd, lim, p_fd, arg);
	if (n < 0)
		return (close(fd[1]), close(fd[0]), n);
	close(fd[0]);
	return (fd[1]);
}

int	redirect(t_cmd all_cmds, int pfd[2], int nth, int no_cmds)
{
	int	red;
	int	fd0;

	red = 0;
	fd0 = all_cmds.fd;
	if (nth == 0 && no_cmds != 1)
		(dup2(pfd[1], 1), close(pfd[1]), close(pfd[0]));
	else if (nth == no_cmds - 1 && no_cmds != 1)
	{
		if (fd0 == 0)
			fd0 = pfd[0];
		else
			close(pfd[0]);
	}
	else if (no_cmds != 1 && nth != no_cmds - 1)
	{
		dup2(pfd[1], 1);
		close(pfd[1]);
		close(pfd[0]);
		if (fd0 == 0)
			fd0 = pfd[2];
		else
			close(pfd[2]);
	}
	while (all_cmds.redirection[red].file != NULL)
	{
		if (all_cmds.redirection[red].type == 0)
		{
			if (fd0 != 0)
				close(fd0);
			fd0 = open_infile(all_cmds.redirection[red].file,
					all_cmds.redirection[red].error);
			if (fd0 == -1)
				return (-1);
		}
		else if (all_cmds.redirection[red].type == 1)
		{
			if (open_outfile(all_cmds.redirection[red].file,
					all_cmds.redirection[red].error) == -1)
			{
				if (fd0 != 0)
					close(fd0);
				return (-1);
			}
		}
		else if (all_cmds.redirection[red].type == 3)
		{
			if (open_append_file(all_cmds.redirection[red].file,
					all_cmds.redirection[red].error) == -1)
			{
				if (fd0 != 0)
					close(fd0);
				return (-1);
			}
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
