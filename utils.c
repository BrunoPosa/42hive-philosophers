/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:29:27 by bposa             #+#    #+#             */
/*   Updated: 2024/08/11 18:24:16 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_ms(long long int mseconds, t_philo *p)
{
	long long int	start;
	long long int	current;
(void)p;
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
