/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:12:13 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/15 14:43:04 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/stat.h>
# include <stdio.h>
# include <limits.h>
# include <stdlib.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>
# include <unistd.h>
# include "libft/libft.h"
# include <fcntl.h>
# include <wait.h>

extern int	g_herdoc_signal;

typedef struct s_vars
{
	int		i;
	bool	dquotes;
	bool	squotes;
	char	*res;
}	t_vars;

typedef struct s_redr
{
	char	*file;
	char	type;
	char	expandable;
	char	error;
}	t_redr;

typedef struct s_cmd
{
	char			**cmd;
	struct s_redr	*redirection;
	int				fd;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	char			empty;
	char			i;
	struct s_env	*next;
}	t_env;

typedef struct s_arg
{
	t_cmd	*all_cmds;
	t_env	*env;
	t_env	*export;
}	t_arg;

int		valid_quotes(char	*str);
bool	validate_input(char *input);
char	**ft_split_input(char *str);
char	**ft_split_pipe(char const *s, char c);
char	*seperate_redirections(char *s, int i, int j, char c);
int		expand(t_cmd *all_cmds, int i, int z, t_env *envs);
char	*expand_parse(char *str, t_env *envs);
char	*exctract_dollar(char *str, int *i, char *res, t_env *envs);
void	push_char(char **s, char c);
void	builtin_pwd(void);
int		builtin_cd(char **args, int cmds_size, t_env *env, t_env *exprt);
void	builtin_echo(char **args);
int		builtin_exit(t_arg *arg, int cmds_size, int n);
t_env	*new_env(char *env);
t_env	*push_env(t_env *head, t_env *new);
char	*ft_getenv(t_env *envs, char *key);
t_env	*duplicate_env(char **env);
void	free_env(t_env *env);
int		export(t_env *env, t_env *exprt, char **cmd);
int		execute(t_cmd *all_cmds, t_env *env, t_env *exprt);
int		ft_dstrlen(char **str);
void	chpwd(t_env *env, t_env *exprt, char *new);
int		remove_quotes_main(t_cmd *cmds);
int		unset(char **cmd, t_env *env, t_env *export);
void	display_env(t_env *env);
t_env	*sort_lst(t_env *lst);
void	push_export(t_env *env, t_env *new);
void	append_export(t_env *env, t_env *new);
int		redirect(t_cmd all_cmds, int pfd[2], int nth, int no_cmds);
void	freencmds(t_cmd	*all_cmds, int n);
char	*check_commands(t_env *env, char *cmd);
void	choldpwd(t_env *env, t_env *exprt, char *new);
int		open_heredoc(char *lim, int p_fd[3], int args[2], t_arg *arg);
void	sigint_handler(int sig);
void	child_sigint(int sig);
void	chexitstatus(int status, t_env *env, t_env *exprt);
int		execute_echo(t_arg *arg, int i, int no_cmds, int p_fd[3]);
int		execute_pwd(t_arg *arg, int i, int no_cmds, int p_fd[3]);
int		execute_exit(t_arg *arg, int i, int no_cmds, int p_fd[3]);
int		execute_unset(t_arg *arg, int i, int p_fd[3]);
int		execute_env(t_arg *arg, int i, int p_fd[3]);
int		execute_cd(t_arg *arg, int i, int p_fd[3]);
int		execute_export(t_arg *arg, int i, int p_fd[3]);
int		count_cmds(t_cmd *cmds);
int		execute_others_main(t_arg *arg, int i, int p_fd[3]);
void	sigquit_handler(int sig);
char	*expand_parse_heredoc(char *str, t_env *envs);
char	**insert2darray(char **src, char **new, int i);
int		check_empty_string(char *s);
int		space_separated(char *str);
char	*get_pwd(char option);
int		errno_to_estatus(void);
int		get_status(t_env *env, t_env *exprt, int option);
int		remove_quotes(char **cmds, char *str, int n);
void	change_quotes(char c, int *q);
void	freedblchar(char **dbl, int size);
int		fill_split_string(char *s, char *ret);
void	close_heredocs(t_arg *arg);
void	close_heredocs3(t_cmd *all_cmds, int cmd);
int		pre_execution(t_arg *arg, int *no_cmds, int p_fd[3]);
int		pipe_shit(int i, int no_cmds, int p_fd[3]);
int		execute_builtins(t_arg *arg, int i, int *status, int p_fd[3]);
int		here_doc(t_arg *arg, int p_fd[3], int no_cmds);
void	execution_epilogue(int no_cmds, int p_fd[3], int *status);

#endif