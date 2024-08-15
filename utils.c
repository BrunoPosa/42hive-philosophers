/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:29:27 by bposa             #+#    #+#             */
/*   Updated: 2024/08/15 13:24:45 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	increment(int *var, pthread_mutex_t *lock)
{
	pthread_mutex_lock(lock);
	*var = *var + 1;
	if (*var == 2147483647)
		*var = 3;
	pthread_mutex_unlock(lock);
}

int	ifonlyonefork(t_philo *p)
{
	if (!p->forktwo)
	{
		pthread_mutex_unlock(p->forkone);
		ft_usleep(p->die_t, p);
		setter(p->death, DEATH, p->dlock);
		return (DEATH);
	}
	return (ERROR);
}

void	maybeprint(t_data *d, int i, long long int t)
{
	if (checker(d, MEAL) != SUCCESS)
	{
		pthread_mutex_lock(&d->printlock);
		// pthread_mutex_lock(&d->dielock);
		printf("%lld %d died\n", t - d->starttime, i + 1);
		// pthread_mutex_unlock(&d->dielock);
		pthread_mutex_unlock(&d->printlock);
	}
}

void	timecorrection(t_philo *p)
{
	long long int	t;

	t = get_time_ms();
	if (p->runs)
		return ;
	pthread_mutex_lock(p->dlock);
	if (t != *p->start_t)
		*p->start_t = t;
	pthread_mutex_unlock(p->dlock);
}

void	timesetter(long long int *var, long long int *value, pthread_mutex_t *m)
{
	pthread_mutex_lock(m);
	*var = *value;
	pthread_mutex_unlock(m);
}

void	swapforks(t_philo *p)
{
	pthread_mutex_t	*temp;

	temp = p->forkone;
	p->forkone = p->forktwo;
	p->forktwo = temp;
}

void	dropforks(t_philo *p)
{
	pthread_mutex_unlock(p->forkone);
	pthread_mutex_unlock(p->forktwo);
}

int	ft_usleep(long long int mseconds, t_philo *p)
{
	long long int start = get_time_ms();
	long long int elapsed;

	if (mseconds < 5)
		mseconds = 5;
	while (1)
	{
		if (getter(p->death, p->dlock))
			return (ERROR);
		elapsed = get_time_ms() - start;
		if (elapsed >= mseconds)
			break;
		if (mseconds - elapsed > 1000)
			usleep(1000);
		else
			usleep((mseconds - elapsed) * 1000);
	}
	return (SUCCESS);
}

int	getter(int *var, pthread_mutex_t *lock)
{
	int	value;

	value = 0;
	if (!var || !lock)
		return (ERROR);
	pthread_mutex_lock(lock);
	value = *var;
	pthread_mutex_unlock(lock);
	return (value);
}

long long int	lastmealget(t_philo *p)
{
	long long int	value;

	value = 0;
	pthread_mutex_lock(&p->lmeallock);
	value = p->last_meal_t;
	pthread_mutex_unlock(&p->lmeallock);
	return (value);
}

int	lastmealset(t_philo *p)
{
	pthread_mutex_lock(&p->lmeallock);
	p->last_meal_t = get_time_ms();
	pthread_mutex_unlock(&p->lmeallock);
	return (42);
}

int	checker(t_data *d, int flag)
{
	int	i;

	i = -1;
	if (flag == MEAL && d->n_meals != ERROR)
	{
		while (++i < d->n_philos)
		{
			if (getter(&d->philo[i]->meals_had, &d->philo[i]->lmeallock) < d->n_meals)
				return (ERROR);
		}
		return (SUCCESS);
	}
	else if (flag == GO)
	{
		while (++i < d->n_philos)
		{
			if (getter(&d->philo[i]->ready, &d->philo[i]->readylock) != SUCCESS)
				return (ERROR);
		}
		return (GO);
	}
	return (ERROR);
}

int	endchecker(t_data *d)
{
	int	i;

	i = -1;
	while (++i < d->n_philos)
	{
		if (!getter(&d->philo[i]->end, &d->philo[i]->readylock))
			return (ERROR);
	}
	return (SUCCESS);
}

int	my_atoi(char *n)
{
	int	res;

	res = 0;
	while (n && *n >= '0' && *n <= '9')
		res = res * 10 + *n++ - '0';
	return (res);
}

int	my_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
	{
		return (0);
	}
	while (i + 1 < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
		{
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		}
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
