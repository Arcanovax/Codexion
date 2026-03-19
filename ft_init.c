/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:47:24 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/19 13:33:02 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_mutex_track(t_all *all)
{
	all->track.all = 0;
	all->track.coders = 0;
	all->track.dongles = 0;
	all->track.monitor = 0;
	all->track.printf = 0;
	all->track.queue = 0;
	all->track.start = 0;
	return (1);
}

int	init_dongles(t_all *all)
{
	int	i;

	i = 0;
	all->dongles = malloc(sizeof(t_dongle) * all->args.nb_coders);
	if (!all->dongles)
		return (0);
	while (i < all->args.nb_coders)
	{
		if (pthread_mutex_init(&all->dongles[i].mutex, NULL))
			return (0);
		all->dongles[i].id = i;
		all->dongles[i].is_used = 0;
		all->dongles[i].last_use = 0;
		i++;
		all->track.dongles++;
	}
	return (1);
}

int	init_monitor(t_all *all)
{
	struct timeval	start;
	int				i;

	i = 0;
	if (!create_monitor_mutex(all))
		return (0);
	if (!init_all_coders(all))
		return (0);
	pthread_mutex_lock(&all->start_mutex);
	gettimeofday(&start, NULL);
	all->start = (start.tv_sec * 1000L) + (start.tv_usec / 1000L);
	all->go = 1;
	pthread_mutex_unlock(&all->start_mutex);
	if (pthread_mutex_init(&all->monitor.mutex, NULL))
		return (0);
	all->track.monitor = 1;
	if (pthread_create(&all->monitor.thread, NULL, monitoring, all))
		return (0);
	return (1);
}

int	init_coder(t_all *all, int nb_coders, int i)
{
	pthread_mutex_lock(&all->mutex);
	all->coders[i].id = i;
	all->coders[i].nb_compile = 0;
	all->coders[i].last_compile = 0;
	all->coders[i].in_queue = 0;
	all->coders[i].left = &all->dongles[(i - 1 + nb_coders) % nb_coders];
	all->coders[i].right = &all->dongles[i];
	all->coders[i].all = all;
	all->coders[i].malloc_error = 0;
	pthread_mutex_unlock(&all->mutex);
	if (pthread_mutex_init(&all->coders[i].mutex, NULL))
		return (0);
	if (pthread_create(&all->coders[i].thread_id,
			NULL, threading, &all->coders[i]))
		return (0);
	return (1);
}

int	init_all_coders(t_all *all)
{
	int	i;
	int	nb_coders;

	nb_coders = all->args.nb_coders;
	all->coders = malloc(sizeof(t_coder) * nb_coders);
	if (!all->coders)
		return (0);
	i = 0;
	all->active = 1;
	all->ready_count = 0;
	all->go = 0;
	while (i < nb_coders)
	{
		if (!init_coder(all, nb_coders, i))
		{
			pthread_mutex_lock(&all->start_mutex);
			all->go = -1;
			all->active = 0;
			pthread_mutex_unlock(&all->start_mutex);
			return (0);
		}
		i++;
		all->track.coders++;
	}
	return (1);
}
