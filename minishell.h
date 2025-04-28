/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarike <tibarike@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:12:13 by tibarike          #+#    #+#             */
/*   Updated: 2025/04/27 15:43:32 by tibarike         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <limits.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>
# include <unistd.h>
# include "libft/libft.h"


extern int g_prompt_statue;

# define PROMPT 0
# define HERDOC 1

typedef struct s_redr
{
	char	*file;
	char	type;
}	t_redr;

typedef struct s_cmd
{
	char			**cmd;
	struct s_redr	*redirection;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	char			empty;
	struct s_env	*next;
}	t_env;

int		valid_quotes(char	*str);
bool	validate_input(char *input);
char	**ft_split_input(char *str);
char	**ft_split_pipe(char const *s, char c);
char	*seperate_redirections(char *s, int i, int j, char c);
int		expand(t_cmd *all_cmds, int i, int z, t_env *envs);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	builtin_pwd(void);
int		builtin_cd(char **args, int cmds_size);
void	builtin_echo(char **args);
void	builtin_exit(char **args, int cmds_size);
t_env	*new_env(char *env);
void	push_env(t_env *head, t_env *new);
char	*ft_getenv(t_env *envs, char *key);
t_env	*duplicate_env(char **env);
void	free_env(t_env *env);
int		export(t_env *env, char **cmd);
void	execute(t_cmd *all_cmds, t_env *env);
int		ft_dstrlen(char **str);
void	chpwd(t_env *env, char *new);
int		remove_quotes_main(t_cmd *cmds);
int		unset(char **cmd, t_env *env);
void	display_env(t_env *env);

#endif