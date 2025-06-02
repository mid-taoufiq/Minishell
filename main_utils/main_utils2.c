/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 11:18:43 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/20 15:34:54 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	extract_redirections_from_cmd(char **cmd, t_redr *redirections)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (cmd[i])
	{
		if (cmd[i][0] == '>' || cmd[i][0] == '<')
		{
			redirections[counter].file = cmd[i + 1];
			redirections[counter].type = get_redirection_type(cmd[i]);
			redirections[counter].expandable = 0;
			redirections[counter].error = 0;
			if (redirections[counter].type == 2)
			{
				if (!ft_strchr(redirections[counter].file, '\'')
					&& !ft_strchr(redirections[counter].file, '\"'))
					redirections[counter].expandable = 1;
			}
			counter++;
			i++;
		}
		i++;
	}
	redirections[counter].file = NULL;
}

void	freedbl(void **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return ;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr[i]);
	free(ptr);
}

void	free_redirections(t_redr *arr)
{
	int	i;

	i = 0;
	while (arr[i].file)
	{
		free(arr[i].file);
		i++;
	}
	free(arr[i].file);
	free(arr);
}

void	freencmds(t_cmd	*all_cmds, int n)
{
	int	i;

	i = 0;
	if (!all_cmds)
		return ;
	while (i < n)
	{
		freedbl((void **)all_cmds[i].cmd);
		free_redirections(all_cmds[i].redirection);
		i++;
	}
	free(all_cmds);
}

int	get_status(t_env *env, t_env *exprt, int option)
{
	static t_env	*envv;
	static t_env	*exp;
	char			*tmp;
	int				ret;

	if (option == 0)
	{
		envv = env;
		exp = exprt;
	}
	else if (option == 1)
	{
		tmp = ft_getenv(envv, "?");
		ret = (int)ft_atol(tmp, &ret);
		free(tmp);
		return (ret);
	}
	else
		chexitstatus(option, envv, exp);
	return (0);
}
