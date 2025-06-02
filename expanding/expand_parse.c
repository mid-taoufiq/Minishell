/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:38:16 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/22 10:04:37 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init(t_vars *vars)
{
	vars->i = 0;
	vars->dquotes = false;
	vars->squotes = false;
	vars->res = ft_strdup("");
}

static int	pshandchng(char c, t_vars *vars, bool *flag, int ch)
{
	push_char(&vars->res, c);
	if (!vars->res)
		return (perror("malloc"), 1);
	if (ch)
		*flag = !*flag;
	vars->i++;
	return (0);
}

static int	expand_dollar(char *str, t_vars *vars, t_env *envs, char *tmp)
{
	if (str[vars->i] == '?')
	{
		vars->res = question_mark(envs, vars->res, &vars->i);
		if (!vars->res)
			return (1);
		return (0);
	}
	else if (ft_isalpha(str[vars->i]) || str[vars->i] == '_')
	{
		vars->res = exctract_dollar(str, &vars->i, vars->res, envs);
		if (!vars->res)
			return (1);
	}
	else if ((str[vars->i] == '\"' && !vars->dquotes)
		|| (str[vars->i] == '\'' && !vars->squotes && !vars->dquotes))
		return (0);
	else if (!ft_isalpha(str[vars->i]) && str[vars->i] != '_')
	{
		tmp = vars->res;
		vars->res = push_char2(vars->res, '$');
		free(tmp);
		if (!vars->res)
			return (1);
	}
	return (0);
}

static int	expandparse_loop(char *str, t_env *envs, t_vars *vars)
{
	while (str[vars->i])
	{
		if (str[vars->i] == '\'' && !vars->dquotes)
		{
			if (pshandchng(str[vars->i], vars, &vars->squotes, 1) == 1)
				return (1);
		}
		else if (str[vars->i] == '\"' && !vars->squotes)
		{
			if (pshandchng(str[vars->i], vars, &vars->dquotes, 1) == 1)
				return (1);
		}
		else if (str[vars->i] == '$' && !vars->squotes)
		{
			vars->i++;
			if (expand_dollar(str, vars, envs, NULL) == 1)
				return (1);
		}
		else
		{
			if (pshandchng(str[vars->i], vars, &vars->dquotes, 0) == 1)
				return (1);
		}
	}
	return (0);
}

char	*expand_parse(char *str, t_env *envs)
{
	t_vars	vars;

	init(&vars);
	if (!vars.res)
		return (perror("malloc"), NULL);
	if (expandparse_loop(str, envs, &vars))
		return (NULL);
	return (vars.res);
}
