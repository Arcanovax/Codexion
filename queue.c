/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:53:40 by mthetcha          #+#    #+#             */
/*   Updated: 2026/02/25 11:53:31 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int	init_queue(t_queue *queue)
{
	queue->head = NULL;
	queue->tail = NULL;
	if (pthread_mutex_init(&queue->mutex, NULL))
		return (1);
	return (0);
}
int	queue_append(t_queue *queue, t_coder *coder)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (1);
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
	return (0);
}

int queue_pop(t_queue *queue)
{
	t_node	*tmp;

	pthread_mutex_lock(&queue->mutex);

	if (!queue->head)
	{
		pthread_mutex_unlock(&queue->mutex);
		return 0;
	}

	tmp = queue->head;
	queue->head = tmp->next;

	if (!queue->head)
		queue->tail = NULL;

	free(tmp);
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

int	has_priority(t_all *all, t_coder *coder)
{
	int		i;
	t_coder	*other;
	t_node	*current;

	i = 0;

	while (i < all->args.nb_coders)
	{
		other = &all->coders[i];

		pthread_mutex_lock(&other->mutex);

		if (other != coder && !other->in_queue)
		{
			if (other->left == coder->left || other->left == coder->right
				|| other->right == coder->left || other->right == coder->right)
			{
				pthread_mutex_unlock(&other->mutex);
				return (0);
			}
		}

		pthread_mutex_unlock(&other->mutex);
		i++;
	}
	pthread_mutex_lock(&all->queue.mutex);
	current = all->queue.head;
	while (current && current->coder != coder)
	{
		other = current->coder;
		if (other->left == coder->left || other->left == coder->right
			|| other->right == coder->left || other->right == coder->right)
		{
			pthread_mutex_unlock(&all->queue.mutex);
			return (0);
		}
		current = current->next;
	}
	pthread_mutex_unlock(&all->queue.mutex);
	return (1);
}

void	print_queue(t_queue *queue)
{
	t_node	*current;

	pthread_mutex_lock(&queue->mutex);
	current = queue->head;
	printf("Queue: ");

	while (current)
	{
		printf("%i", current->coder->id);
		if (current->next)
			printf(" -> ");
		current = current->next;
	}

	printf("\n");
	pthread_mutex_unlock(&queue->mutex);
}
