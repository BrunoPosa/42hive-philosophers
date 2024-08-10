/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:43:10 by bposa             #+#    #+#             */
/*   Updated: 2024/08/10 21:10:30 by bposa            ###   ########.fr       */
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

//I have an array of philo structs, each of which has a thread and 5 mutex pointers
typedef	struct	s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	lmeallock;
	pthread_mutex_t	*forkone;
	pthread_mutex_t	*forktwo;
	pthread_mutex_t	*prlock;
	pthread_mutex_t	*dlock;
	pthread_mutex_t	*serlock;
	int				*deathwatch;
	long long int	*start_t;
	int				*served;
	int				die_t;
	int				eat_t;
	int				sleep_t;
	int				meals_had;
	long long int	last_meal_t;
	int				go;
	int				firstrun;
}	t_philo;

typedef struct	s_data
{
	pthread_t		butler;
	t_philo			**philo;
	pthread_mutex_t	*forks;
	pthread_mutex_t	printlock;
	pthread_mutex_t	dielock;
	pthread_mutex_t	servedlock;
	int				served_n;
	int				n_philos;
	int				die_t;
	int				eat_t;
	int				sleep_t;
	int				n_meals;
	long long int	starttime;
	int				death;
	int				initdone;
}	t_data;

int				validator(int argc, char **args);
int				initor(char **argv, t_data *d);
int				init_mu_th(t_data *d);
int				cleanerr(t_data *d, int status, int initialized);
int				normal_cleanup(t_data *d);
int				mumalth_cleanup(t_data *d, int status, int initialized);
void			free_philos(t_data *d);
int				ermsg(int status);
void			routine(t_philo *p);
void			butler(t_data *d);
void			printer(int arg, char *str, t_philo *p);
int				checker(t_data *d, int flag);
int				spread(t_data *d, int signal);
long long int	lastmealget(t_philo *p);
int				lastmealset(t_philo *p);
int				getter(int *var, pthread_mutex_t *lock);
void			setter(int	*var, int value, pthread_mutex_t *lock);
long long int	get_time_ms(void);
int				wait_ms(long long int mseconds, t_philo *p);
int				my_atoi(char *n);
size_t			my_strlen(const char *s);
int				my_strncmp(const char *s1, const char *s2, size_t n);

#endif