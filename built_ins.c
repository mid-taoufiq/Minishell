/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 08:37:41 by tibarike          #+#    #+#             */
/*   Updated: 2025/04/22 17:08:30 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(void)
{
	char	*path;
	
	path = getcwd(NULL, 0);
	printf("%s\n", path);
	if (!path)
		return ;
}

int	cd(char **args)
{
	char	*old_pwd;
	char	*path;
	char	*tmp;

	if ((old_pwd = getcwd(NULL, 0)) == NULL)
		return (1);
	if (args[2])
		return (1);
	if (args[1] == NULL)
	{
		if ((path = ft_strdup(getenv("HOME"))) == NULL)
		return (1);	
	}
	else if (ft_strcmp(args[1], ".") == 0 || ft_strcmp(args[1], "..") == 0)
		path = args[1];
	else if (args[1][0] == '/')
		path = ft_strjoin(old_pwd, args[1]);
	else
	{
		tmp = ft_strjoin(old_pwd, "/");
		if (!tmp)
			return (free(old_pwd), 1);
		free(old_pwd);
		path = ft_strjoin(tmp, args[1]);
		if (!path)
			return (free(tmp), 1);
	}
	if (chdir(path) != 0)
		return (perror("cd"), free(path), 1);
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

void	echo(char **args)
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
}
int main(int argc, char **argv)
{
	echo(argv);
	return 0;
}
