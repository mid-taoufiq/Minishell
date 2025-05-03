/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:12:13 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/03 14:21:30 by ayel-arr         ###   ########.fr       */
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
	int				fd;
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
void	builtin_cd(char **args, int cmds_size, t_env *env, t_env *exprt);
void	builtin_echo(char **args);
void	builtin_exit(char **args, int cmds_size);
t_env	*new_env(char *env);
void	push_env(t_env *head, t_env *new);
char	*ft_getenv(t_env *envs, char *key);
t_env	*duplicate_env(char **env);
void	free_env(t_env *env);
int		export(t_env *env, t_env *exprt, char **cmd);
int		execute(t_cmd *all_cmds, t_env *env, t_env *exprt, char **o_env);
int		ft_dstrlen(char **str);
void	chpwd(t_env *env, t_env *exprt, char *new);
int		remove_quotes_main(t_cmd *cmds);
int		unset(char **cmd, t_env *env);
void	display_env(t_env *env);
t_env	*sort_lst(t_env *lst);
void	push_export(t_env *env, t_env *new);
void	append_export(t_env *env, t_env *new);
int		redirect(t_cmd all_cmds, int pfd[2], int nth, int  no_cmds);
void	freencmds(t_cmd	*all_cmds, int n);
char	*check_commands(t_env *env, char *cmd);
void	choldpwd(t_env *env, t_env *exprt, char *new);
int		open_heredoc(char *lim);
int		write_in_file(int fd, char *lim);
void	chexitstatus(int status, t_env *env, t_env *exprt);

#endif