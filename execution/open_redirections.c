/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:21:50 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/03 14:06:50 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	fd = open(filename, O_WRONLY | O_CREAT, 0777);
	if (fd == -1)
		return (perror("open"), -1);
	dup2(fd, 1);
	close(fd);
	return (0);
}

int	open_append_file(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_APPEND | O_CREAT);
	if (fd == -1)
		return (perror("open"), -1);
	dup2(fd, 1);
	close(fd);
	return (0);
}

int	write_in_file(int fd, char *lim)
{
	char	*line;

	line = readline("heredoc> ");
	while (line && ft_strcmp(line, lim))
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("heredoc> ");
	}
	free(line);
	return (fd);
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
	write_in_file(fd[0], lim);
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
		(dup2(pfd[1], 1), close(pfd[1]));
	else if (nth == no_cmds - 1 && no_cmds != 1)
		fd0 = pfd[0];
	else if (no_cmds != 1)
	{
		dup2(pfd[1], 1);
		close(pfd[1]);
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
