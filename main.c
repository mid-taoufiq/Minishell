/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:21:14 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/15 11:19:21 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"	

int	g_herdoc_signal = 0;

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

int	main(int argc, char **argv, char **env)
{
	int		i;
	char	**cmds;
	char	**cmd;
	char	*tmp;
	char	*line;
	t_cmd	*all_cmds;
	t_env	*envs;
	t_env	*s_env;
	int		status;

	(void)argc;
	(void)argv;
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		exit(1);
	envs = duplicate_env(env);
	if (!envs)
		return (perror("env"), 1);
	s_env = sort_lst(envs);
	if (!s_env)
		return (free_env(envs), 1);
	status = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	get_pwd(0);
	get_status(envs, s_env, 0);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			status = get_status(NULL, NULL, 1);
			(free_env(envs), free_env(s_env));
			(printf("exit\n"), get_pwd(2), exit(status));
		}
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		if (!validate_input(line))
		{
			free(line);
			continue ;
		}
		cmds = ft_split_pipe(line, '|');
		if (!cmds)
			(freedbl((void **)cmds), perror("error\n"), exit (1));
		free(line);
		i = ft_dstrlen(cmds) + 1;
		all_cmds = malloc(sizeof(t_cmd) * i);
		i = 0;
		while (cmds[i])
		{
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
					exit(1);
				}
				free(tmp);
			}
			cmd = ft_split_input(cmds[i]);
			if (!cmd)
			{
				freencmds(all_cmds, i);
				freedbl((void **)cmds);
				freedbl((void **)cmd);
				get_pwd(2);
				perror("error\n");
				exit(1);
			}
			all_cmds[i].cmd = malloc((exe_arg_len(cmd) + 1) * sizeof(char *));
			if (!all_cmds[i].cmd)
			{
				freencmds(all_cmds, i);
				freedbl((void **)cmds);
				freedbl((void **)cmd);
				get_pwd(2);
				perror("error\n");
				exit(1);
			}
			all_cmds[i].redirection
				= malloc((redirections_len(cmd) + 1) * sizeof(t_redr));
			if (!all_cmds[i].redirection)
			{
				free(all_cmds[i].cmd);
				freencmds(all_cmds, i);
				freedbl((void **)cmds);
				freedbl((void **)cmd);
				get_pwd(2);
				perror("error\n");
				exit(1);
			}
			extract_exe_arg_from_cmd(cmd, all_cmds[i].cmd);
			extract_redirections_from_cmd(cmd, all_cmds[i].redirection);
			all_cmds[i].fd = 0;
			free(cmd);
			i++;
		}
		freedbl((void **)cmds);
		all_cmds[i].cmd = NULL;
		all_cmds[i].redirection = NULL;
		if (expand(all_cmds, 0, 0, envs))
		{
			freencmds(all_cmds, i);
			continue ;
		}
		if (remove_quotes_main(all_cmds))
		{
			freencmds(all_cmds, i);
			continue ;
		}
		status = execute(all_cmds, envs, s_env);
		g_herdoc_signal = 0;
		chexitstatus(status, envs, s_env);
		freencmds(all_cmds, i);
	}
	return (0);
}
