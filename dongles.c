/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:02:48 by mthetcha          #+#    #+#             */
/*   Updated: 2026/02/26 09:38:13 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int take_dongles(t_coder *coder)
{

	coder->left->is_used = 1;
	pthread_mutex_lock(&coder->all->printf);
	printf("%li %i has taken a dongle\n",get_time(coder->all), coder->id);
	pthread_mutex_unlock(&coder->all->printf);

	coder->right->is_used = 1;
	pthread_mutex_lock(&coder->all->printf);
	printf("%li %i has taken a dongle\n",get_time(coder->all),coder->id);
	pthread_mutex_unlock(&coder->all->printf);
	return (1);
}
int leave_dongles(t_coder *coder)
{

	coder->left->is_used = 0;
	coder->left->last_use = get_time(coder->all);
	coder->right->is_used = 0;
	coder->right->last_use = get_time(coder->all);

	return (1);
}
