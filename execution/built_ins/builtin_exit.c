/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 11:26:23 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/24 19:59:41 by ayel-arr         ###   ########.fr       */
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

void	builtin_exit_helper(char **args, int cmds_size)
{
	int		i;
	int		success;

	i = argslen(args);
	success = 1;
	if (cmds_size == 1)
		ft_putstr_fd("exit\n", 2);
	if (i >= 2)
	{
		ft_atol2(args[1], &success);
		if (!success)
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			exit(2);
		}
	}
}

int	builtin_exit(t_arg *arg, int cmds_size, int n, int i)
{
	long	exit_value;
	int		success;
	char	**args;

	exit_value = 0;
	args = arg->all_cmds[n].cmd;
	success = 1;
	i = argslen(args);
	builtin_exit_helper(args, cmds_size);
	if (i > 2)
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	exit_value = ft_atol2(args[1], &success);
	if (success == 0)
		(ft_putstr_fd("exit: numeric argument required\n", 2), exit(2));
	if (i == 1)
		exit_value = get_status(NULL, NULL, 1);
	(freencmds(arg->all_cmds, cmds_size), free_env(arg->env),
		free_env(arg->export));
	exit(exit_value % 256);
}
