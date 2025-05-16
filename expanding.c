/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:12:31 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/15 14:54:25 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	push_char(char **s, char c)
{
	char	temp[2];
	char	*tmp;

	temp[0] = c;
	temp[1] = '\0';
	tmp = *s;
	*s = ft_strjoin(*s, temp);
	free(tmp);
}

char	*expand_parse_heredoc(char *str, t_env *envs)
{
	int		i[2];
	char	*result;

	i[0] = 0;
	result = ft_strdup("");
	if (!result)
		return (perror("malloc"), NULL);
	while (str[*i])
	{
		if (str[*i] == '$')
		{
			i[1] = 1;
			result = exctract_dollar(str, i, result, envs);
			if (!result)
				return (NULL);
		}
		else
		{
			push_char(&result, str[*i]);
			if (!result)
				return (perror("malloc"), NULL);
			(*i)++;
		}
	}
	return (result);
}

int	expand(t_cmd *all_cmds, int i, int z, t_env *envs)
{
	char	*tmp;
	char	**split;
	int		len;
	char	**ttmp;

	tmp = NULL;
	while (all_cmds[i].cmd)
	{
		z = 0;
		while (all_cmds[i].cmd[z])
		{
			if (ft_strchr(all_cmds[i].cmd[z], '$'))
			{
				tmp = expand_parse(all_cmds[i].cmd[z], envs);
				if (!tmp)
					return (1);
				split = ft_split_input(tmp);
				len = ft_dstrlen(split);
				if (len == 1)
				{
					(free(all_cmds[i].cmd[z]), all_cmds[i].cmd[z] = tmp);
					z++;
					(free(split[0]), free(split));
					continue ;
				}
				ttmp = insert2darray(all_cmds[i].cmd, split, z);
				if (!ttmp)
					return (1);
				free(all_cmds[i].cmd);
				free(split);
				free(tmp);
				all_cmds[i].cmd = ttmp;
				z += len - 1;
			}
			z++;
		}
		z = 0;
		while (all_cmds[i].redirection[z].file)
		{
			if (ft_strchr(all_cmds[i].redirection[z].file, '$')
				&& all_cmds[i].redirection[z].type != 2)
			{
				tmp = expand_parse(all_cmds[i].redirection[z].file, envs);
				if (!tmp)
					return (all_cmds[i].redirection[z].error = 1, 1);
				if (check_empty_string(tmp) || space_separated(tmp))
				{
					all_cmds[i].redirection[z].error = 1;
					free(tmp);
					z++;
					continue ;
				}
				free(all_cmds[i].redirection[z].file);
				all_cmds[i].redirection[z].file = tmp;
			}
			z++;
		}
		i++;
	}
	return (0);
}
