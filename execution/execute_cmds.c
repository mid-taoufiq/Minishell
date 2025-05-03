/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:44:58 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/04/29 10:53:17 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void free_dbl_ptr(char **ptr, int i)
{
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

char	*check_commands(t_env *env, char *cmd)
{
	int		i;
	char	**paths;
	char	*file_path;
	char	*tmp;

	if (access(cmd, X_OK) == 0 && ft_strchr(cmd, '/') != NULL)
		return (ft_strdup(cmd));
	if (ft_strchr(cmd, '/') != NULL && access(cmd, X_OK) != 0)
		return (perror(cmd), NULL);
	if (!(tmp = ft_getenv(env, "PATH")))
		return (free(tmp), perror(cmd), NULL);
	paths = ft_split(tmp, ':');
	free(tmp);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		free(paths[i]);
		file_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(file_path, X_OK) == 0)
			return (free_dbl_ptr(paths, i + 1), file_path);
		free(file_path);
		i++;
	}
	return (free(paths), perror(cmd), NULL);
}