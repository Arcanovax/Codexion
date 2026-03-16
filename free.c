/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 12:32:43 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/16 12:54:25 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_queue(t_all *all)
{
	t_node	*current;
	t_node	*next;

	pthread_mutex_lock(&all->queue.mutex);
	current = all->queue.head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	all->queue.head = NULL;
	all->queue.tail = NULL;
	pthread_mutex_unlock(&all->queue.mutex);
}

void	free_all(t_all *all)
{
	free(all->coders);
	free(all->dongles);
	free_queue(all);
}
