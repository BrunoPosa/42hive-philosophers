/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:43:10 by bposa             #+#    #+#             */
/*   Updated: 2024/08/11 21:43:00 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#ifndef ERROR
# define ERROR -1
#endif

#ifndef SUCCESS
# define SUCCESS 0
#endif

#ifndef EARGC
# define EARGC 1
#endif

#ifndef EARG
# define EARG 2
#endif

#ifndef EINIT
# define EINIT 3
#endif

#ifndef ETHREAD
# define ETHREAD 4
#endif

#ifndef EMUTEX
# define EMUTEX 6
#endif

#ifndef EVAL
# define EVAL 7
#endif

#ifndef EMALMUT
# define EMALMUT 8
#endif

#ifndef EJOIN
# define EJOIN 9999
#endif

#ifndef DEATH
# define DEATH 616
#endif

#ifndef GO
# define GO 42
#endif

#ifndef MEAL
# define MEAL 747
#endif

#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef enum	e_action
{
	THINK,
	FORK,
	FORKTWO,
	EAT,
	SLEEP
}	t_action;

typedef struct	s_data
{
	pthread_t		*philo;
	pthread_mutex_t	*fork;
	long long int	*tfed;
	int				*fedcount;
	pthread_mutex_t	printlock;
	pthread_mutex_t	dielock;
	pthread_mutex_t	lock;
	int				ids;
	long long int	tstart;
	int 			death;
	int				n_philos;
	int				tdie;
	int				teat;
	int				tsleep;
	int				meals;
}	t_data;

int				action(t_action act, int arg, char *str, t_data *d);
void			increment_id(int *id, pthread_mutex_t *mutex);
void			assignforks(int id, t_data *d, void *lf, void *rf);
int				validator(int argc, char **args);
int				init_all(char **argv, t_data *d);
int				init_mu_th(t_data *d);
int				cleanerr(t_data *d, int status, int initialized);
int				normal_cleanup(t_data *d);
int				mumalth_cleanup(t_data *d, int status, int initialized);
void			free_philos(t_data *d);
int				ermsg(int status);
int				routine(int id, t_data *d);
void			monitor(t_data *d);
int				printer(int arg, char *str, t_data *d);
int				checker(t_data *d, int flag);
int				spread(t_data *d, int signal);
int				getter(int *var, pthread_mutex_t *lock);
void			setter(int	*var, int value, pthread_mutex_t *lock);
long long int	get_time_ms(void);
int				wait_ms(long long int mseconds, t_data *d);
int				my_atoi(char *n);
size_t			my_strlen(const char *s);
int				my_strncmp(const char *s1, const char *s2, size_t n);

#endif