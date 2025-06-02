/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 11:18:40 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/18 12:59:45 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_dstrlen(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	exe_arg_len(char **cmd)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (cmd[i])
	{
		if (cmd[i][0] != '>' && cmd[i][0] != '<')
			counter++;
		else if (cmd[i][0] == '>' || cmd[i][0] == '<')
			i++;
		i++;
	}
	return (counter);
}

int	redirections_len(char **cmd)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (cmd[i])
	{
		if (cmd[i][0] == '>' || cmd[i][0] == '<')
		{
			counter++;
			i++;
		}
		i++;
	}
	return (counter);
}

void	extract_exe_arg_from_cmd(char **cmd, char **dst)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (cmd[i])
	{
		if (cmd[i][0] != '>' && cmd[i][0] != '<')
		{
			dst[counter] = cmd[i];
			counter++;
		}
		else if (cmd[i][0] == '>' || cmd[i][0] == '<')
			i++;
		i++;
	}
	dst[counter] = NULL;
}

char	get_redirection_type(char *str)
{
	if (!ft_strcmp(str, ">>"))
		return (free(str), 3);
	else if (!ft_strcmp(str, "<<"))
		return (free(str), 2);
	else if (!ft_strcmp(str, ">"))
		return (free(str), 1);
	else if (!ft_strcmp(str, "<"))
		return (free(str), 0);
	return (-1);
}
