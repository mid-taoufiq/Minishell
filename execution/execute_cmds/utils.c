/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:25:56 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/28 15:41:29 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_dir(char *path)
{
	struct stat	info;

	stat(path, &info);
	if (S_ISDIR(info.st_mode))
	{
		errno = EACCES;
		return (1);
	}
	return (0);
}

int	errno_to_estatus(void)
{
	if (errno == EACCES)
		return (126);
	else if (errno == ENOENT)
		return (127);
	else if (errno == ENOTDIR)
		return (126);
	return (1);
}

void	close_heredocs3(t_cmd *all_cmds, int cmd)
{
	int	i;

	i = 0;
	while (all_cmds[i].cmd)
	{
		if (i == cmd)
		{
			i++;
			continue ;
		}
		if (all_cmds[i].fd)
			close(all_cmds[i].fd);
		i++;
	}
}
