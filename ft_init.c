/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:47:24 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/13 09:26:56 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int init_dongles(t_all *all)
{
	int i;

	i = 0;
	all->dongles = malloc(sizeof(t_dongle) * all->args.nb_coders);
	if (!all->dongles)
		return (1);
	while (i < all->args.nb_coders)
	{
		pthread_mutex_init(&all->dongles[i].mutex, NULL);
		all->dongles[i].id = i;
		all->dongles[i].is_used = 0;
		all->dongles[i].last_use = 0;
		i++;
	}
	return (0);
}

int init_monitor(t_all *all)
{
	struct timeval start;

	init_coders(all);
	pthread_mutex_lock(&all->start_mutex);
	gettimeofday(&start, NULL);
	all->start = (start.tv_sec * 1000L) + (start.tv_usec / 1000L);
	all->go = 1;
	pthread_mutex_unlock(&all->start_mutex);
	pthread_mutex_init(&all->monitor.mutex, NULL);
	pthread_create(&all->monitor.thread, NULL, monitoring, all);
	pthread_join(all->monitor.thread, NULL);
	return (0);

}
int	init_coders(t_all *all)
{
	int i;
	pthread_t thread;
	int nb_coders;

	nb_coders = all->args.nb_coders;

	i = 0;
	all->coders = malloc(sizeof(t_coder) * nb_coders);
	if (!all->coders)
		return (1);
	all->active = 1;
	all->ready_count = 0;
	all->go = 0;
	pthread_mutex_init(&all->mutex, NULL);
	pthread_mutex_init(&all->start_mutex, NULL);
	pthread_mutex_init(&all->printf, NULL);
	while (i < nb_coders)
	{
		pthread_mutex_lock(&all->mutex);
		all->coders[i].id = i;
		all->coders[i].nb_compile = 0;
		all->coders[i].last_compile = 0;
		all->coders[i].in_queue = 0;
		all->coders[i].left = &all->dongles[(i - 1 + nb_coders) % nb_coders];
		all->coders[i].right = &all->dongles[i];
		all->coders[i].all = all;
		pthread_mutex_unlock(&all->mutex);
		pthread_mutex_init(&all->coders[i].mutex, NULL);
		pthread_create(&thread, NULL, threading, &all->coders[i]);
		all->coders[i].thread_id = thread;
		i++;
	}
	return (1);
}
