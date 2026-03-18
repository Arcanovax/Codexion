/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:47:24 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/18 11:06:21 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	do_debug(t_coder *coder)
{
	int	result;

	result = 0;
	pthread_mutex_lock(&coder->all->mutex);
	if (coder->all->active)
	{
		print_msg("is debugging", coder);
		result = 1;
	}
	pthread_mutex_unlock(&coder->all->mutex);
	return (result);
}

int	do_refactor(t_coder *coder)
{
	int	result;

	result = 0;
	pthread_mutex_lock(&coder->all->mutex);
	if (coder->all->active)
	{
		print_msg("is refactoring", coder);
		result = 1;
	}
	pthread_mutex_unlock(&coder->all->mutex);
	return (result);
}

int	routine(t_coder *coder)
{
	take_dongles(coder);
	print_msg("is compiling", coder);
	pthread_mutex_lock(&coder->mutex);
	coder->nb_compile++;
	pthread_mutex_unlock(&coder->mutex);
	ft_sleep(coder->all->args.tm_compile, coder->all);
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile = get_time(coder->all);
	pthread_mutex_unlock(&coder->mutex);
	leave_dongles(coder);
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
	if (!do_debug(coder))
		return (0);
	ft_sleep(coder->all->args.tm_debug, coder->all);
	if (!do_refactor(coder))
		return (0);
	ft_sleep(coder->all->args.tm_refactor, coder->all);
	return (1);
}

void	wait_go(t_all *all)
{
	while (1)
	{
		pthread_mutex_lock(&all->start_mutex);
		if (all->go == 1)
		{
			pthread_mutex_unlock(&all->start_mutex);
			break ;
		}
		pthread_mutex_unlock(&all->start_mutex);
		usleep(100);
	}
}

void	*threading(void *arg)
{
	t_coder	*coder;
	int		in_queue;
	long	now;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->all->start_mutex);
	coder->all->ready_count++;
	pthread_mutex_unlock(&coder->all->start_mutex);
	wait_go(coder->all);
	if (coder->id % 2)
		usleep(1500);
	while (1)
	{
		usleep(1);
		pthread_mutex_lock(&coder->all->mutex);
		if (coder->all->active == 0)
		{
			pthread_mutex_unlock(&coder->all->mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->all->mutex);
		pthread_mutex_lock(&coder->mutex);
		in_queue = coder->in_queue;
		pthread_mutex_unlock(&coder->mutex);
		if (coder->left == coder->right)
			continue ;
		if (in_queue && !has_priority(coder->all, coder))
			continue ;
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
		now = get_time(coder->all);
		if (!coder->left->is_used && !coder->right->is_used
			&& (coder->left->last_use == 0
				|| (now - coder->left->last_use) >= coder->all->args.dongle_cooldown)
			&& (coder->right->last_use == 0
				|| (now - coder->right->last_use) >= coder->all->args.dongle_cooldown))
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
	return (NULL);
}
