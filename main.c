/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:21:14 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/28 15:38:13 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_herdoc_signal = 0;

int	get_t_cmd_arr(char	**cmds, int i, t_cmd *all_cmds);

char	**get_cmds(t_env *envs, t_env *s_env)
{
	char	*line;
	int		status;
	char	**cmds;

	line = readline("minishell> ");
	if (!line)
	{
		status = get_status(NULL, NULL, 1);
		(free_env(envs), free_env(s_env));
		(printf("exit\n"), get_pwd(2), exit(status));
	}
	if (!validate_input(line, envs, s_env))
		return (free(line), NULL);
	cmds = ft_split_pipe(line, '|');
	if (!cmds)
	{
		(get_pwd(2), free(line));
		(free_env(envs), free_env(s_env));
		(perror("error\n"), exit (1));
	}
	free(line);
	return (cmds);
}

int	redirections(char	**cmds, int i, t_cmd *all_cmds)
{
	char	*tmp;

	if (ft_strchr(cmds[i], '>') || ft_strchr(cmds[i], '<'))
	{
		tmp = cmds[i];
		cmds[i] = seperate_redirections(tmp, 0, 0, 0);
		if (!cmds[i])
		{
			cmds[i] = tmp;
			freencmds(all_cmds, i);
			freedbl((void **)cmds);
			get_pwd(2);
			perror("error\n");
			return (1);
		}
		free(tmp);
	}
	return (0);
}

int	fill_all_cmds(char	**cmds, t_env *envs, t_env *s_env, t_cmd *all_cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		if (redirections(cmds, i, all_cmds))
			(free_env(envs), free_env(s_env), exit(1));
		if (get_t_cmd_arr(cmds, i, all_cmds))
			(free_env(envs), free_env(s_env), exit(1));
		i++;
	}
	freedbl((void **)cmds);
	all_cmds[i].cmd = NULL;
	all_cmds[i].redirection = NULL;
	return (i);
}

void	main_loop(t_env *envs, t_env *s_env)
{
	char	**cmds;
	t_cmd	*all_cmds;
	int		status;
	int		i;

	while (1)
	{
		cmds = get_cmds(envs, s_env);
		if (!cmds)
			continue ;
		all_cmds = malloc(sizeof(t_cmd) * (ft_dstrlen(cmds) + 1));
		if (!all_cmds)
			(freedbl((void **)cmds), exit(1));
		i = fill_all_cmds(cmds, envs, s_env, all_cmds);
		if (expand(all_cmds, 0, 0, envs) || remove_quotes_main(all_cmds))
			(freencmds(all_cmds, i), free_env(envs), free_env(s_env), exit(1));
		status = execute(all_cmds, envs, s_env);
		g_herdoc_signal = 0;
		(chexitstatus(status, envs, s_env), freencmds(all_cmds, i));
	}
}

int	main(int argc, char **argv, char **env)
{
	t_env	*envs;
	t_env	*s_env;

	(void)argv;
	(void)argc;
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		exit(1);
	envs = duplicate_env(env);
	if (!envs)
		return (perror("env"), 1);
	s_env = sort_lst(envs);
	if (!s_env)
		return (free_env(envs), 1);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	get_pwd(0);
	get_status(envs, s_env, 0);
	while (1)
	{
		main_loop(envs, s_env);
	}
	return (0);
}
