/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_more.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:08:58 by bposa             #+#    #+#             */
/*   Updated: 2024/08/15 22:39:50 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
			if (getter(&d->philo[i]->meals_had, &d->philo[i]->lmeallock)
				< d->n_meals)
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
