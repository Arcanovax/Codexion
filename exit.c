/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 12:32:43 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/19 13:10:50 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_queue(t_all *all)
{
	t_node	*current;
	t_node	*next;

	current = all->queue.head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	all->queue.head = NULL;
	all->queue.tail = NULL;
	pthread_mutex_destroy(&all->queue.mutex);
}

void	free_all(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->track.coders)
	{
		pthread_join(all->coders[i].thread_id, NULL);
		i++;
	}
	pthread_join(all->monitor.thread, NULL);
	destroy_mutex(all);
	if (all->dongles)
		free(all->dongles);
	if (all->track.printf)
		free(all->coders);
	if (all->queue.head)
		free_queue(all);
}

void	destroy_mutex(t_all *all)
{
	int	i;

	i = 0;
	if (all->track.printf)
		pthread_mutex_destroy(&all->printf);
	while (i < all->track.coders)
	{
		pthread_mutex_destroy(&all->coders[i].mutex);
		i++;
	}
	i = 0;
	while (i < all->track.dongles)
	{
		pthread_mutex_destroy(&all->dongles[i].mutex);
		i++;
	}
	if (all->track.start)
		pthread_mutex_destroy(&all->start_mutex);
	if (all->track.monitor)
		pthread_mutex_destroy(&all->monitor.mutex);
	if (all->track.all)
		pthread_mutex_destroy(&all->mutex);
}
