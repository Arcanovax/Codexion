/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:02:07 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/19 13:27:14 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_args(t_all *all, char **argv)
{
	all->args.nb_coders = atoi(argv[0]);
	all->args.tm_burnout = atoi(argv[1]);
	all->args.tm_compile = atoi(argv[2]);
	all->args.tm_debug = atoi(argv[3]);
	all->args.tm_refactor = atoi(argv[4]);
	all->args.nb_compiles = atoi(argv[5]);
	all->args.dongle_cooldown = atoi(argv[6]);
	if (strcmp(argv[7], "fifo") == 0)
		all->args.scheduler = FIFO;
	else
		all->args.scheduler = EDF;
	return (1);
}

static int	is_safe(char *str)
{
	int		i;
	char	*int_max;

	i = 0;
	int_max = "2147483647";
	while (str[i] && str[i] >= '0' && str[i] <= '9')
		i++;
	if (i > 10 || i == 0)
		return (0);
	if (i == 10)
	{
		i = 0;
		while (i < 10 && str[i] <= int_max[i])
			i++;
		if (i < 10)
			return (0);
	}
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	check_args(char **argv)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		if (!is_safe(argv[i]) || atoi(argv[i]) < 1)
		{
			printf("Error: Argument %i is invalid", i + 1);
			return (0);
		}
		i++;
	}
	if (strcmp(argv[i], "fifo") != 0 && strcmp(argv[i], "edf") != 0)
	{
		printf("Error: Argument %i is invalid", i + 1);
		return (0);
	}
	return (1);
}

void	print_msg(char *msg, t_coder *coder)
{
	pthread_mutex_lock(&coder->all->printf);
	printf("%li %i %s\n", get_time(coder->all), coder->id + 1, msg);
	pthread_mutex_unlock(&coder->all->printf);
}

int	main(int argc, char **argv)
{
	t_all	all;

	if (argc != 9)
	{
		printf("Number of arguments invalid\n");
		return (0);
	}
	if (!check_args(argv + 1) || !get_args(&all, argv + 1))
		return (0);
	if (!init_mutex_track(&all) || !init_queue(&all))
		return (0);
	if (!init_dongles(&all))
	{
		destroy_mutex(&all);
		free(all.dongles);
		return (0);
	}
	if (!init_monitor(&all))
	{
		free_all(&all);
		return (0);
	}
	free_all(&all);
	return (1);
}
