/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigmsg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:47:59 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/05/22 13:49:10 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_msg(int sig, int last_sig)
{
	if (last_sig)
	{
		if (last_sig == 1)
			ft_putstr_fd("\n", 2);
		else if (last_sig == 2)
			ft_putstr_fd("Quit (core dumped)\n", 2);
	}
	else if (sig)
	{
		if (last_sig == 1)
			ft_putstr_fd("\n", 2);
		else if (last_sig == 2)
			ft_putstr_fd("Quit (core dumped)\n", 2);
	}
}
