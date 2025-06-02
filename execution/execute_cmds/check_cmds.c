/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:23:44 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/28 15:48:44 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_dbl_ptr(char **ptr, int i)
{
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

static char	*check_commands_loop(char	**paths, char	*cmd)
{
	int		i;
	char	*tmp;
	char	*file_path;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		free(paths[i]);
		file_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(file_path, X_OK) == 0 && !check_dir(file_path))
			return (free_dbl_ptr(paths, i + 1), file_path);
		free(file_path);
		i++;
	}
	return (free(paths), perror(cmd), NULL);
}

char	*check_commands(t_env *env, char *cmd)
{
	char	**paths;
	char	*tmp;

	if (cmd[0] == '\0')
		return (access(cmd, X_OK), perror("\"\""), NULL);
	if (!access(cmd, X_OK) && ft_strchr(cmd, '/') && !check_dir(cmd))
		return (ft_strdup(cmd));
	if (ft_strchr(cmd, '/') && (access(cmd, X_OK) || check_dir(cmd)))
		return (perror(cmd), NULL);
	tmp = ft_getenv(env, "PATH");
	if (!tmp || !tmp[0])
	{
		if (env->i)
			tmp = ft_strdup("/usr/local/sbin:/usr/local/bin"
					":/usr/sbin:/usr/bin:/sbin:/bin");
		else if (access(cmd, X_OK) == 0 && !check_dir(cmd))
			return (ft_strdup(cmd));
		else
			return (perror(cmd), NULL);
	}
	paths = ft_split(tmp, ':');
	free(tmp);
	if (!paths)
		return (NULL);
	return (check_commands_loop(paths, cmd));
}
