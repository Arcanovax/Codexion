/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 10:13:09 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/18 10:28:03 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_active_coders(t_all *all, t_coder *coder)
{
	int		i;
	t_coder	*current_coder;

	i = 0;
	while (i < all->args.nb_coders)
	{
		current_coder = &all->coders[i];
		pthread_mutex_lock(&current_coder->mutex);
		if (current_coder != coder && !current_coder->in_queue)
		{
			if (current_coder->left == coder->left
				|| current_coder->left == coder->right
				|| current_coder->right == coder->left
				|| current_coder->right == coder->right)
			{
				pthread_mutex_unlock(&current_coder->mutex);
				return (0);
			}
		}
		pthread_mutex_unlock(&current_coder->mutex);
		i++;
	}
	return (1);
}

int	check_previous_in_queue(t_all *all, t_coder *coder)
{
	t_coder	*current_coder;
	t_node	*current;

	pthread_mutex_lock(&all->queue.mutex);
	current = all->queue.head;
	while (current && current->coder != coder)
	{
		current_coder = current->coder;
		if (current_coder->left == coder->left
			|| current_coder->left == coder->right
			|| current_coder->right == coder->left
			|| current_coder->right == coder->right)
		{
			pthread_mutex_unlock(&all->queue.mutex);
			return (0);
		}
		current = current->next;
	}
	pthread_mutex_unlock(&all->queue.mutex);
	return (1);
}

int	has_earlier_deadline(t_all *all, t_coder *coder,
						t_coder *current_coder, long now)
{
	long	coder_dl;
	long	current_dl;

	pthread_mutex_lock(&current_coder->mutex);
	if (current_coder->left == coder->left
		|| current_coder->left == coder->right
		|| current_coder->right == coder->left
		|| current_coder->right == coder->right)
	{
		coder_dl = all->args.tm_burnout - (now - coder->last_compile);
		current_dl = all->args.tm_burnout - (now - current_coder->last_compile);
		if (current_dl < coder_dl)
		{
			pthread_mutex_unlock(&current_coder->mutex);
			return (0);
		}
		else if (current_dl == coder_dl && coder->id > current_coder->id)
		{
			pthread_mutex_unlock(&current_coder->mutex);
			return (0);
		}
	}
	pthread_mutex_unlock(&current_coder->mutex);
	return (1);
}

int	check_priority_by_deadline(t_all *all, t_coder *coder)
{
	t_node	*current;
	t_coder	*current_coder;
	long	now;

	now = get_time(all);
	pthread_mutex_lock(&all->queue.mutex);
	current = all->queue.head;
	while (current)
	{
		current_coder = current->coder;
		if (current_coder != coder)
		{
			if (!has_earlier_deadline(all, coder, current_coder, now))
			{
				pthread_mutex_unlock(&all->queue.mutex);
				return (0);
			}
		}
		current = current->next;
	}
	pthread_mutex_unlock(&all->queue.mutex);
	return (1);
}

int	has_priority(t_all *all, t_coder *coder)
{
	if (!check_active_coders(all, coder))
		return (0);
	if (all->args.scheduler == FIFO && !check_previous_in_queue(all, coder))
	{
		return (0);
	}
	if (all->args.scheduler == EDF && !check_priority_by_deadline(all, coder))
	{
		return (0);
	}
	return (1);
}
