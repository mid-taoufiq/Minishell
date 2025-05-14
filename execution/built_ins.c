/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 08:37:41 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/12 14:39:19 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	argslen(char **args)
{
	int	i;
	
	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	builtin_exit(char **args, int cmds_size)
{
	int		i;
	int		j;
	long	exit_value;
	int		success;

	exit_value = 0;
	success = 1;
	j = 0;
	i = argslen(args);
	if (cmds_size == 1)
		ft_putstr_fd("exit\n", 2);
	if (i >= 2)
	{
		while (args[1][j])
		{
			if (!ft_isdigit(args[1][j]) && !(j == 0 && (args[1][j] == '+' || args[1][j] == '-')))
			{
				ft_putstr_fd("exit: numeric argument required\n", 2);
				if (cmds_size == 1)
					exit(2);
				return (2);
			}
			j++;
		}
	}
	if (i > 2)
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	exit_value = ft_atol(args[1], &success);
	if (success == 0)
	{
		if (cmds_size > 1)
			return(ft_putstr_fd("exit: numeric argument required\n", 2), 2);
		else
			(ft_putstr_fd("exit: numeric argument required\n", 2), exit(2));
	}
	if (cmds_size > 1)
		return (0);
	if (i == 1)
		exit_value = get_status(NULL, 1);
	exit(exit_value % 256);
}

void	builtin_pwd(void)
{
	char	*path;
	
	path = get_pwd(1);
	if (path)
		printf("%s\n", path);
}

int	init_oldpwd(char **old_pwd, int cmds_size, t_env *env, t_env *exprt)
{
	*old_pwd = getcwd(NULL, 0);
	if (*old_pwd == NULL && cmds_size == 1)
	{
		choldpwd(env, exprt, getcwd(NULL, 0));
		chdir("/");
		chpwd(env, exprt, getcwd(NULL, 0));
		return (0);
	}
	return (1);
}

int		is_absolute(char **args, char **path, char *old_pwd)
{
	char	*tmp;

	if (args[1][0] == '/')
	{
		free(old_pwd);
		*path =  ft_strdup(args[1]);
		if (!*path)
			return (1);
		return (0);
	}
	tmp = ft_strjoin(old_pwd, "/");
	if (!tmp)
		return (free(old_pwd), 1);
	free(old_pwd);
	*path = ft_strjoin(tmp, args[1]);
	free(tmp);
	if (!*path)
		return (1);
	return (0);
}

int	change_directory(char *path, int cmds_size, t_env *env, t_env *exprt)
{
	struct stat	info;

	if (stat(path, &info) != 0)
		return (ft_putstr_fd("cd: No such file or directory\n", 2), free(path), 1);
	if (!S_ISDIR(info.st_mode))
		return (ft_putstr_fd("cd: Not a directory\n", 2), free(path), 1);
	if (cmds_size > 1)
		return (free(path), 0);
	choldpwd(env, exprt, getcwd(NULL, 0));
	chdir(path);
	free(path);
	chpwd(env, exprt, getcwd(NULL, 0));
	return (0);
}

int	builtin_cd(char **args, int cmds_size, t_env *env, t_env *exprt)
{
	char		*old_pwd;
	char		*path;

	if (args[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	if (!args[1])
	{
		path = ft_getenv(env, "HOME");
		if (!path)
			return (ft_putstr_fd("cd: HOME is not set\n", 2), 1);
	}
	else
	{
		if (init_oldpwd(&old_pwd, cmds_size, env, exprt) == 0)
			return (0);
		if (is_absolute(args, &path, old_pwd) != 0)
			return (1);
		}
	if (change_directory(path, cmds_size, env, exprt) != 0)
		return (1);
	return (0);
}

static int	n_flag(char *str)
{
	int i;

	i = 0;
	if (!str || str[i] != '-')
		return (0);
	i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] != 'n' && i >= 1)
			return (0);
		i++;
	}
	return (1);
}

void	builtin_echo(char **args)
{
	int	i;
	int	newline;


	i = 1;
	newline = 1;
	while (args[i] && n_flag(args[i]) == 1)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");

	return ;
}
