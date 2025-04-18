/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:30:40 by tibarike          #+#    #+#             */
/*   Updated: 2025/04/17 16:53:27 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **env)
{
	int		i;
	int		j;
	char	**cmds;
	char	**cmd;
	char	*tmp;

	(void) ac;
	(void) av;
	(void) env;
	while (1)
	{
		char *line = readline("minishell> ");
		add_history(line);
		if (!validate_input(line))
			continue;
		cmds = ft_split_pipe(line, '|');
		i = 0;
		while (cmds[i])
		{
			printf("input : %s\n", cmds[i]);
			if (ft_strchr(cmds[i], '>') || ft_strchr(cmds[i], '<'))
			{
				tmp = cmds[i];
				cmds[i] = seperate_redirections(tmp);
				free(tmp);
			}
			cmd = ft_split_input(cmds[i], ' ');
			printf("%i:", i);
			j = 0;
			while (cmd[j])
			{
				printf("[%s]", cmd[j]);
				j++;
			}
			printf("\n");
			i++;
		}
	}
	return 0;
}
