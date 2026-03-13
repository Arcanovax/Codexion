/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 16:50:56 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/13 13:35:58 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long int	get_time(t_all *all)
{
	long long		timestamp_ms;
	struct timeval	current;
	long			current_ms;

	timestamp_ms = all->start;
	if (gettimeofday(&current, NULL))
		return (-1);
	current_ms = (long long)current.tv_sec * 1000 + current.tv_usec / 1000;
	return (current_ms - timestamp_ms);
}

void	ft_sleep(long long sleep_time, t_all *all)
{
	long long	start;

	start = get_time(all);
	while ((get_time(all) - start) < sleep_time)
		usleep(100);
}
