/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:43:10 by bposa             #+#    #+#             */
/*   Updated: 2024/08/15 23:06:40 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# ifndef ERROR
#  define ERROR -1
# endif

# ifndef SUCCESS
#  define SUCCESS 0
# endif

# ifndef EARGC
#  define EARGC 1
# endif

# ifndef EARG
#  define EARG 2
# endif

# ifndef EINIT
#  define EINIT 3
# endif

# ifndef ETHREAD
#  define ETHREAD 4
# endif

# ifndef EMUTEX
#  define EMUTEX 6
# endif

# ifndef EVAL
#  define EVAL 7
# endif

# ifndef EMALMUT
#  define EMALMUT 8
# endif

# ifndef EJOIN
#  define EJOIN 9999
# endif

# ifndef DEATH
#  define DEATH 616
# endif

# ifndef GO
#  define GO 42
# endif

# ifndef MEAL
#  define MEAL 747
# endif

# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>

typedef enum e_action
{
	THINK,
	FORK,
	FORKEAT,
	SLEEP
}	t_action;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*forkone;
	pthread_mutex_t	*forktwo;
	pthread_mutex_t	*prlock;
	pthread_mutex_t	deadslock;
	pthread_mutex_t	golock;
	pthread_mutex_t	readylock;
	pthread_mutex_t	lmeallock;
	int				die_t;
	int				eat_t;
	int				sleep_t;
	int				meals_had;
	int				meals;
	// int				*death;
	int				dead;
	long long int	last_meal_t;
	long long int	*start_t;
	int				error;
	int				ready;
	int				go;
	int				runs;
	int				end;
}	t_philo;

typedef struct s_data
{
	pthread_t		butler;
	t_philo			**philo;
	pthread_mutex_t	*forks;
	pthread_mutex_t	printlock;
	pthread_mutex_t	dielock;
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
void			life(t_philo *p);
void			butler(t_data *d);
int				routine(t_philo *p);
int				action(t_action act, int arg, char *str, t_philo *p);
void			syncing(t_data *d);
int				spread(t_data *d, int signal);
void			printer(int arg, char *str, t_philo *p);
int				checkeach(t_data *d, int i, long long int t, long long int *fed);
int				checker(t_data *d, int flag);
int				endchecker(t_data *d);
int				getter(int *var, pthread_mutex_t *lock);
void			setter(void	*var, int value, pthread_mutex_t *lock);
long long int	lastmealget(t_philo *p);
int				lastmealset(t_philo *p);
void			increment(int *var, pthread_mutex_t *lock);
void			dropforks(t_philo *p);
int				ifonlyonefork(t_philo *p);
long long int	get_time_ms(void);
int				ft_usleep(long long int mseconds, t_philo *p);
int				my_atoi(char *n);
size_t			my_strlen(const char *s);

#endif