/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:26:17 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/25 14:21:05 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirection_type2(int *fd0, t_cmd all_cmds)
{
	if (*fd0 != all_cmds.fd)
	{
		check_0_fd(*fd0);
		*fd0 = all_cmds.fd;
	}
}

char	*get_filename(int fd[2], int n)
{
	char	*filename;
	char	*num;
	char	*filename_template;

	filename_template = "/tmp/.tmp_minishell_";
	num = ft_itoa(n);
	if (!num)
		return (NULL);
	filename = ft_strjoin(filename_template, num);
	if (!filename)
		return (free(num), NULL);
	fd[0] = open(filename, O_RDWR | O_CREAT | O_EXCL, 0777);
	while (fd[0] == -1)
	{
		(free(filename), free(num), n++);
		num = ft_itoa(n);
		if (!num)
			return (NULL);
		filename = ft_strjoin(filename_template, num);
		if (!filename)
			return (free(num), NULL);
		fd[0] = open(filename, O_RDWR | O_CREAT | O_EXCL, 0777);
	}
	return (free(num), filename);
}

void	write_in_file_child(char *lim, int args[4], t_arg *arg)
{
	char	*line;
	char	*tmp;

	close(args[1]);
	dup2(args[0], 2);
	close(args[0]);
	signal(SIGINT, child_sigint);
	signal(SIGQUIT, SIG_IGN);
	line = readline("heredoc> ");
	while (line && ft_strcmp(line, lim))
	{
		if (args[3] && ft_strchr(line, '$'))
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
	(free(line), close(args[0]));
}
