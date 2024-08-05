/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:43:10 by bposa             #+#    #+#             */
/*   Updated: 2024/08/05 23:09:35 by bposa            ###   ########.fr       */
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

#ifndef EMALLOC
# define EMALLOC 8
#endif

#ifndef DEATH
# define DEATH 616
#endif

#ifndef GO
# define GO 42
#endif

#ifndef MAX_PHILOS
# define MAX_PHILOS 400
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

typedef	struct	s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*forkone;
	pthread_mutex_t	*forktwo;
	pthread_mutex_t	*prlock;
	pthread_mutex_t	dlock;
	pthread_mutex_t	golock;
	pthread_mutex_t	readylock;
	pthread_mutex_t	lmeallock;
	int				die_t;
	int				eat_t;
	int				sleep_t;
	int				meals_had;
	int				dead;
	long long int	last_meal_t;
	long long int	*start_t;
	int				error;
	int				ready;
	int				go;
}	t_philo;

typedef struct	s_data
{
	pthread_t		butler;
	t_philo			*philo[MAX_PHILOS];
	pthread_mutex_t	forks[MAX_PHILOS];
	pthread_mutex_t	printlock;
	pthread_mutex_t	dielock;
	int				n_philos;
	int				die_t;
	int				eat_t;
	int				sleep_t;
	int				n_meals;
	long long int	starttime;
	int				death;
}	t_data;

void			routine(t_philo *p);
void			butler(t_data *d);
int				validator(int argc, char **args);
int				initor(char **argv, t_data *d);
int				init_mu_th(t_data *d);
long long int	get_time_ms(void);
int				wait_ms(long long int mseconds, t_philo *p);
int				cleanerr(t_data *d, int status, int initialized);
void			free_philos(t_data *d);
int				ermsg(int status);
void			printer(int arg, char *str, t_philo *p);
int				checker(t_data *d, int flag);
// int				check_var(int *var, int status, pthread_mutex_t *lock);
int				isdead(t_philo *p);
void			spread(t_data *d, int signal);
long long int	lastmealget(t_philo *p);
int				lastmealset(t_philo *p);
int				getter(int *var, pthread_mutex_t *lock);
void			setter(int	*var, int value, pthread_mutex_t *lock);
void			wait_until(int *var, int status, pthread_mutex_t *lock);
int				my_atoi(char *n);
size_t			my_strlen(const char *s);
int				my_strncmp(const char *s1, const char *s2, size_t n);

#endif