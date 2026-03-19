/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:33:08 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/19 11:07:13 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_active(t_coder *coder)
{
	pthread_mutex_lock(&coder->all->mutex);
	if (coder->all->active == 0)
	{
		pthread_mutex_unlock(&coder->all->mutex);
		return (0);
	}
	pthread_mutex_unlock(&coder->all->mutex);
	return (1);
}

void	lock_left_right(t_coder *coder)
{
	if (coder->left < coder->right)
	{
		pthread_mutex_lock(&coder->left->mutex);
		pthread_mutex_lock(&coder->right->mutex);
	}
	else
	{
		pthread_mutex_lock(&coder->right->mutex);
		pthread_mutex_lock(&coder->left->mutex);
	}
}

int	can_use_dongles(t_coder *coder, long now)
{
	return (!coder->left->is_used && !coder->right->is_used
		&& (coder->left->last_use == 0
			|| (now - coder->left->last_use)
			>= coder->all->args.dongle_cooldown)
		&& (coder->right->last_use == 0
			|| (now - coder->right->last_use)
			>= coder->all->args.dongle_cooldown)
	);
}

int	wait_go(t_all *all)
{
	pthread_mutex_lock(&all->start_mutex);
	all->ready_count++;
	pthread_mutex_unlock(&all->start_mutex);
	while (1)
	{
		pthread_mutex_lock(&all->start_mutex);
		if (all->go == 1)
		{
			pthread_mutex_unlock(&all->start_mutex);
			return (1);
		}
		if (all->go == -1)
		{
			pthread_mutex_unlock(&all->start_mutex);
			return (0);
		}
		pthread_mutex_unlock(&all->start_mutex);
		usleep(1);
	}
}

void	try_use_dongles(t_coder *coder)
{
	long	now;

	lock_left_right(coder);
	now = get_time(coder->all);
	if (can_use_dongles(coder, now))
	{
		if (coder->in_queue)
			queue_remove(&coder->all->queue, coder);
		pthread_mutex_lock(&coder->mutex);
		coder->in_queue = 1;
		pthread_mutex_unlock(&coder->mutex);
		routine(coder);
		if (!queue_append(&coder->all->queue, coder))
		{
			pthread_mutex_lock(&coder->mutex);
			coder->malloc_error = 1;
			pthread_mutex_unlock(&coder->mutex);
		}
	}
	else
	{
		pthread_mutex_unlock(&coder->left->mutex);
		pthread_mutex_unlock(&coder->right->mutex);
	}
}
