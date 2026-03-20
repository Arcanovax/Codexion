/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:29:05 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/20 11:09:27 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

typedef enum t_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_args
{
	int			nb_coders;
	int			tm_burnout;
	int			tm_compile;
	int			tm_debug;
	int			tm_refactor;
	int			nb_compiles;
	int			dongle_cooldown;
	t_scheduler	scheduler;
}	t_args;

typedef struct s_monitor
{
	pthread_t		thread;
	pthread_mutex_t	mutex;
	struct s_all	*all;

}	t_monitor;

typedef struct s_dongle
{
	int				id;
	int				is_used;
	pthread_mutex_t	mutex;
	int long		last_use;

}	t_dongle;

typedef struct s_mutex_init
{
	int	queue;
	int	dongles;
	int	monitor;
	int	coders;
	int	all;
	int	start;
	int	printf;

}	t_mutex_init;

typedef struct s_coder
{
	int				id;
	t_dongle		*left;
	t_dongle		*right;
	pthread_t		thread_id;
	pthread_mutex_t	mutex;
	struct s_all	*all;
	int				nb_compile;
	int				in_queue;
	int long		last_compile;
	int				malloc_error;
}	t_coder;

typedef struct s_node
{
	t_coder			*coder;
	struct s_node	*next;
}	t_node;

typedef struct s_queue
{
	t_node			*head;
	t_node			*tail;
	pthread_mutex_t	mutex;
}	t_queue;

typedef struct s_all
{
	t_coder			*coders;
	t_dongle		*dongles;
	t_args			args;
	pthread_mutex_t	mutex;
	int				active;
	long long		start;
	int				go;
	int				ready_count;
	pthread_mutex_t	start_mutex;
	pthread_mutex_t	printf;
	t_monitor		monitor;
	t_queue			queue;
	t_mutex_init	track;
}	t_all;

int			init_all_coders(t_all *args);
int			init_dongles(t_all *all);
int			init_mutex_track(t_all *all);
int			init_queue(t_all *all);
int			init_monitor(t_all *all);
int			take_dongles(t_coder *coder);
int			leave_dongles(t_coder *coder);
void		try_use_dongles(t_coder *coder);
int			can_use_dongles(t_coder *coder, long now);
int			queue_append(t_queue *queue, t_coder *coder);
int			queue_remove(t_queue *queue, t_coder *coder);
int			queue_append(t_queue *queue, t_coder *coder);
int			queue_remove(t_queue *queue, t_coder *coder);
int			has_priority(t_all *all, t_coder *coder);
void		ft_sleep(long long sleep_time, t_all *all);
long int	get_time(t_all *all);
void		*monitoring(void *arg);
void		*threading(void *arg);
int			routine(t_coder *coder);
int			has_priority(t_all *all, t_coder *coder);
void		lock_left_right(t_coder *coder);
void		print_msg(char *msg, t_coder *coder);
void		free_all(t_all *all);
int			is_active(t_coder *coder);
int			wait_go(t_all *all);
void		destroy_mutex(t_all *all);
int			create_monitor_mutex(t_all *all);
#endif
