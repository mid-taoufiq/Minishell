
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
	return(0);
}

static int	expand_dollar(char *str, t_vars *vars, t_env *envs)
{
	if (str[vars->i + 1] == '\"' && !vars->dquotes)
	{
		vars->i += 2;
		while (str[vars->i] && str[vars->i] != '\"')
		{
			push_char(&vars->res, str[vars->i]);
			if (!vars->res)
				return (perror("malloc"), 1);
			vars->i++;
		}
		if (str[vars->i] == '\"')
			vars->i++;
	}
	else
	{
		vars->res = exctract_dollar(str, &vars->i, vars->res, envs);
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
			if (expand_dollar(str, vars, envs) == 1)
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
	t_vars vars;

	init(&vars);
	if (!vars.res)
		return (perror("malloc"), NULL);
	if (expandparse_loop(str, envs, &vars))
        return (NULL);
	return (vars.res);
}
