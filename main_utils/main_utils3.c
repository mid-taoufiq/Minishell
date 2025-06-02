/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:31:07 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/20 15:33:37 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fail(char	**cmds, char **cmd, int i, t_cmd *all_cmds)
{
	free(cmd);
	freencmds(all_cmds, i);
	freedbl((void **)cmds);
	freedbl((void **)cmd);
	get_pwd(2);
	perror("error\n");
}

int	get_t_cmd_arr(char	**cmds, int i, t_cmd *all_cmds)
{
	char	**cmd;

	cmd = ft_split_input(cmds[i]);
	if (!cmd)
		return (fail(cmds, cmd, i, all_cmds), 1);
	all_cmds[i].cmd = malloc((exe_arg_len(cmd) + 1) * sizeof(char *));
	if (!all_cmds[i].cmd)
		return (fail(cmds, cmd, i, all_cmds), 1);
	all_cmds[i].redirection
		= malloc((redirections_len(cmd) + 1) * sizeof(t_redr));
	if (!all_cmds[i].redirection)
	{
		freedbl((void **)all_cmds[i].cmd);
		return (fail(cmds, cmd, i, all_cmds), 1);
	}
	extract_exe_arg_from_cmd(cmd, all_cmds[i].cmd);
	extract_redirections_from_cmd(cmd, all_cmds[i].redirection);
	all_cmds[i].fd = 0;
	free(cmd);
	return (0);
}
