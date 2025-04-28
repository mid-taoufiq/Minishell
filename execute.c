/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:27:21 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/04/27 15:43:16 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	structs_size(t_cmd *all_cmds)
{
	int	i;
	
	i = 0;
	while (all_cmds[i].cmd)
		i++;
	return (i);
}


void	execute(t_cmd *all_cmds, t_env *env)
{
	int	i;
	int cmds_size;

	cmds_size = structs_size(all_cmds);
	i = 0;
	while (all_cmds[i].cmd)
	{
		if (!ft_strcmp(all_cmds[i].cmd[0], "export"))
			export(env, all_cmds[i].cmd);
		else if (!ft_strcmp(all_cmds[i].cmd[0], "echo"))
			builtin_echo(all_cmds[i].cmd, i, cmds_size);
		else if (!ft_strcmp(all_cmds[i].cmd[0], "cd"))
			builtin_cd(all_cmds[i].cmd, cmds_size);
		else if (!ft_strcmp(all_cmds[i].cmd[0], "pwd"))
			builtin_pwd();
		else if (!ft_strcmp(all_cmds[i].cmd[0], "exit"))
			builtin_exit(all_cmds[i].cmd, cmds_size);
		else if (!ft_strcmp(all_cmds[i].cmd[0], "unset"))
			unset(all_cmds[i].cmd, env);
		else if (!ft_strcmp(all_cmds[i].cmd[0], "env"))
			display_env(env);
		/*
		else:
			find path and execute.
			then store exit status.
		*/
		i++;
	}
}
