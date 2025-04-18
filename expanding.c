
#include "minishell.h"

// static char	*expand_parse(char *str, int exit_status)
// {
// 	int		i;
// 	bool	in_single;
// 	bool	in_double;
// 	char	*result;
// 	char	temp[2] = {str[i++], 0};

// 	i = 0;
// 	in_double = false;
// 	in_single = false;
// 	result = ft_calloc(1, 1);
// 	while (str[i])
// 	{
// 		if (str[i] == '\'' && !in_double)
// 		{
// 			in_single = true;
// 			result = ft_strjoin(result, "'");
// 			i++;
// 		}
// 		else if (str[i] == '"' && !in_single)
// 		{
// 			in_double = true;
// 			result = ft_strjoin(result, "\"");
// 			i++;
// 		}
// 		else if (str[i] == '$' && !in_single)
// 			result = expand(str, &i, result, exit_status);
// 		else
// 			result = ft_strjoin(result, temp);
// 	}
// 	return(result);
// }

char	*expand(char *str, int *i, char *res, int exit_status)
{
	char	*var;
	char	*val;
	int		start;
	int		len;

	(*i)++;
	len = 0;
	if (str[*i] == '?')
	{
		val = ft_itoa(exit_status);
		(*i)++;
	}
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		start = *i;
		while (ft_isalnum(str[*i]) || str[*i] == '_')
		{
			len++;
			(*i)++;
		}
		var = ft_substr(str, start, len);
		val = getenv(var);
		free(var);
	}
	if (!val)
		val = ft_strdup("");
	return (ft_strjoin(res, val));
}
