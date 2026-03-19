/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:47:24 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/19 13:33:24 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	do_compile(t_coder *coder)
{
	take_dongles(coder);
	print_msg("is compiling", coder);
	pthread_mutex_lock(&coder->mutex);
	coder->nb_compile++;
	pthread_mutex_unlock(&coder->mutex);
}

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
	do_compile(coder);
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

void	*threading(void *arg)
{
	t_coder	*coder;
	int		in_queue;

	coder = (t_coder *)arg;
	if (!wait_go(coder->all))
		return (NULL);
	if (coder->id % 2)
		usleep(1500);
	while (is_active(coder))
	{
		usleep(1);
		pthread_mutex_lock(&coder->mutex);
		in_queue = coder->in_queue;
		pthread_mutex_unlock(&coder->mutex);
		if (coder->left == coder->right)
			continue ;
		if (in_queue && !has_priority(coder->all, coder))
			continue ;
		try_use_dongles(coder);
	}
	return (NULL);
}
