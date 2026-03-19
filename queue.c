/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:53:40 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/19 12:45:49 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_queue(t_all *all)
{
	all->queue.head = NULL;
	all->queue.tail = NULL;
	if (pthread_mutex_init(&all->queue.mutex, NULL))
		return (0);
	all->track.queue = 1;
	return (1);
}

int	queue_append(t_queue *queue, t_coder *coder)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (0);
	new->coder = coder;
	new->next = NULL;
	pthread_mutex_lock(&queue->mutex);
	if (!queue->tail)
	{
		queue->head = new;
		queue->tail = new;
	}
	else
	{
		queue->tail->next = new;
		queue->tail = new;
	}
	pthread_mutex_unlock(&queue->mutex);
	return (1);
}

int	queue_remove(t_queue *queue, t_coder *coder)
{
	t_node	*current;
	t_node	*prev;

	prev = NULL;
	pthread_mutex_lock(&queue->mutex);
	current = queue->head;
	while (current)
	{
		if (current->coder == coder)
		{
			if (prev)
				prev->next = current->next;
			else
				queue->head = current->next;
			if (!current->next)
				queue->tail = prev;
			free(current);
			pthread_mutex_unlock(&queue->mutex);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	pthread_mutex_unlock(&queue->mutex);
	return (0);
}
