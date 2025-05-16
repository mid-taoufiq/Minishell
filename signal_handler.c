/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:51:37 by tibarike          #+#    #+#             */
/*   Updated: 2025/05/13 10:24:25 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	if (g_herdoc_signal == 0)
	{
		get_status(NULL, NULL, 130);
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	child_sigint(int sig)
{
	(void) sig;
	printf("\n");
	exit(130);
}

void	sigquit_handler(int sig)
{
	(void)sig;
}
