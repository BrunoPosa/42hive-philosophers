/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:29:27 by bposa             #+#    #+#             */
/*   Updated: 2024/08/09 23:46:09 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_ms(long long int mseconds, t_philo *p)
{
	long long int	start;
	long long int	current;

	current = 0;
	start = get_time_ms();
	if (mseconds < 5)
		mseconds = 5;
	while (current - start < mseconds)
	{
		current = get_time_ms();
		usleep(400);
		if (getter(p->deathwatch, p->dlock) == DEATH)
			return (ERROR);
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
			if (d->philo[i]->meals_had < d->n_meals)
				return (ERROR);
		}
		return (SUCCESS);
	}
	return (ERROR);
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
