/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:22:53 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/22 09:37:54 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_0_fd(int fd)
{
	if (fd != 0)
		close(fd);
}

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

int	open_infile(char *filename, char error)
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
	if (dup2(fd, 1) == -1)
		return (perror("dup2"), close(fd), -1);
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
	if (dup2(fd, 1) == -1)
		return (perror("dup2"), close(fd), -1);
	close(fd);
	return (0);
}
