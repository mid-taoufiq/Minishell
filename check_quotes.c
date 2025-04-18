/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 17:38:35 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/04/13 19:41:07 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pop_back(t_list *head)
{
	while (head->next->next)
	{
		head = head->next;
	}
	free(head->next);
	head->next = NULL;
}

void	nop(void *nothing)
{
	nothing = nothing + 1 - 1;
}

int	valid_quotes(char	*str)
{
	int		i;
	t_list	*head;
	char	*last;

	i = 0;
	head = ft_lstnew("0");
	while (str[i])
	{
		last = ft_lstlast(head)->content;
		if (str[i] == '\'')
		{
			if (*last == '0')
				ft_lstadd_back(&head, ft_lstnew(str + i));
			if (*last == '\'')
				pop_back(head);
		}
		else if (str[i] == '\"')
		{
			if (*last == '0')
				ft_lstadd_back(&head, ft_lstnew(str + i));
			if (*last == '\"')
				pop_back(head);
		}
		i++;
	}
	if (ft_lstsize(head) == 1)
		return (ft_lstclear(&head, nop), 1);
	return (ft_lstclear(&head, nop), 0);
}
