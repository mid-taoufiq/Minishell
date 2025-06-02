/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 15:16:09 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/24 20:06:07 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static void	pre(const char **ptr, int *sign)
{
	while (**ptr == ' ' || (**ptr >= 9 && **ptr <= 13))
	{
		*ptr = *ptr + 1;
	}
	if (**ptr == '-' || **ptr == '+')
	{
		if (**ptr == '-')
			*sign = -1;
		*ptr = *ptr + 1;
	}
}

static void	ft_atol_ending(const char *ptr, int *success)
{
	while (*ptr)
	{
		if (!is_whitespace(*ptr))
		{
			*success = 0;
			return ;
		}
		ptr++;
	}
}

long	ft_atol2(const char *ptr, int *success)
{
	int			sign;
	long		total;
	long		last;

	sign = 1;
	total = 0;
	last = 0;
	if (ptr == NULL)
		return (total);
	pre(&ptr, &sign);
	if (!(*ptr >= '0' && *ptr <= '9'))
		return (*success = 0, 0);
	while (*ptr >= '0' && *ptr <= '9')
	{
		total = (total * 10) + (*ptr - '0');
		if (sign == -1 && (total * sign) > (last * sign))
			return (*success = 0, 0);
		if (sign == 1 && total < last)
			return (*success = 0, 0);
		last = total;
		ptr++;
	}
	ft_atol_ending(ptr, success);
	return (total * sign);
}

long	ft_atol(const char *ptr, int *success)
{
	int			sign;
	long		total;
	long		last;

	sign = 1;
	total = 0;
	last = 0;
	if (ptr == NULL)
		return (total);
	pre(&ptr, &sign);
	if (!(*ptr >= '0' && *ptr <= '9'))
		return (*success = 0, 0);
	while (*ptr >= '0' && *ptr <= '9')
	{
		total = (total * 10) + (*ptr - '0');
		if (sign == -1 && (total * sign) > (last * sign))
			return (*success = 0, 0);
		if (sign == 1 && total < last)
			return (*success = 0, 0);
		last = total;
		ptr++;
	}
	return (total * sign);
}
