/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:19:08 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/19 13:34:05 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	all_done(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->args.nb_coders)
	{
		pthread_mutex_lock(&all->coders[i].mutex);
		if (all->coders[i].nb_compile < all->args.nb_compiles)
		{
			pthread_mutex_unlock(&all->coders[i].mutex);
			return (0);
		}
		pthread_mutex_unlock(&all->coders[i].mutex);
		i++;
	}
	return (1);
}

int	one_burn(t_all *all)
{
	int			i;
	long int	time;
	long int	tm_burnout;

	tm_burnout = all->args.tm_burnout;
	time = get_time(all);
	i = 0;
	while (i < all->args.nb_coders)
	{
		pthread_mutex_lock(&all->coders[i].mutex);
		if (time - all->coders[i].last_compile > tm_burnout)
		{
			print_msg("burned out", &all->coders[i]);
			pthread_mutex_unlock(&all->coders[i].mutex);
			return (1);
		}
		pthread_mutex_unlock(&all->coders[i].mutex);
		i++;
	}
	return (0);
}

int	one_fail(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->args.nb_coders)
	{
		pthread_mutex_lock(&all->coders[i].mutex);
		if (all->coders[i].malloc_error)
		{
			pthread_mutex_unlock(&all->coders[i].mutex);
			return (1);
		}
		pthread_mutex_unlock(&all->coders[i].mutex);
		i++;
	}
	return (0);
}

int	create_monitor_mutex(t_all *all)
{
	if (pthread_mutex_init(&all->mutex, NULL))
		return (0);
	all->track.all = 1;
	if (pthread_mutex_init(&all->start_mutex, NULL))
		return (0);
	all->track.start = 1;
	if (pthread_mutex_init(&all->printf, NULL))
		return (0);
	all->track.printf = 1;
	return (1);
}

void	*monitoring(void *arg)
{
	t_all	*all;

	all = (t_all *)arg;
	while (1)
	{
		pthread_mutex_lock(&all->mutex);
		if (all_done(all) || one_burn(all) || one_fail(all))
		{
			all->active = 0;
			pthread_mutex_unlock(&all->mutex);
			break ;
		}
		pthread_mutex_unlock(&all->mutex);
		usleep(1);
	}
	return (NULL);
}
