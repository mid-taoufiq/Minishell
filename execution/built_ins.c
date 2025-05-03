/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 08:37:41 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/02 14:14:58 by ayel-arr         ###   ########.fr       */
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
void	builtin_exit(char **args, int cmds_size)
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
				return ;
			}
			j++;
		}
	}
	if (i > 2)
		return (ft_putstr_fd("exit: too many arguments\n", 2));
	exit_value = ft_atol(args[1], &success);
	if (success == 0)
	{
		if (cmds_size > 1)
			return(ft_putstr_fd("exit: numeric argument required\n", 2));
		else
			(ft_putstr_fd("exit: numeric argument required\n", 2), exit(2));
	}
	if (cmds_size > 1)
		return ;
	exit(exit_value % 256);
}

void	builtin_pwd(void)
{
	char	*path;
	
	path = getcwd(NULL, 0);
	if (path)
	{
		printf("%s\n", path);
		free(path);	
	}
}

void	builtin_cd(char **args, int cmds_size, t_env *env, t_env *exprt)
{
	char		*old_pwd;
	char		*path;
	char		*tmp;
	int			i;
	struct stat	info;

	i = argslen(args);
	if (i >= 3)
		return (ft_putstr_fd("too many arguments\n", 2));
	if (i == 1)
	{
		path = ft_strdup(getenv("HOME"));
		if (!path)
			return(ft_putstr_fd("cd: HOME is not set\n", 2));
	}
	else
	{
		if ((old_pwd = getcwd(NULL, 0)) == NULL)
		{
			choldpwd(env, exprt, getcwd(NULL, 0));
			chdir("/");
			chpwd(env, exprt, getcwd(NULL, 0));
			return ;
		}
		else if (args[1][0] == '/')
		{
			path = ft_strdup(args[1]);
			if (!path)
				return (free(old_pwd));
			free(old_pwd);
		}
		else
		{
			tmp = ft_strjoin(old_pwd, "/");
			if (!tmp)
				return (free(old_pwd));
			free(old_pwd);
			path = ft_strjoin(tmp, args[1]);
			free(tmp);
			if (!path)
				return ;
		}
	}
	if (stat(path, &info) != 0)
		return (ft_putstr_fd("cd: No such file or directory\n", 2), free(path));
	if (!S_ISDIR(info.st_mode))
		return (ft_putstr_fd("cd: Not a directory\n", 2), free(path));
	if (cmds_size > 1)
		return free(path);
	choldpwd(env, exprt, getcwd(NULL, 0));
	chdir(path);
	free(path);
	chpwd(env, exprt, getcwd(NULL, 0));
	return ;
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
