/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:29:27 by bposa             #+#    #+#             */
/*   Updated: 2024/08/15 23:06:18 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ifonlyonefork(t_philo *p)
{
	if (!p->forktwo)
	{
		pthread_mutex_unlock(p->forkone);
		ft_usleep(p->die_t, p);
		return (DEATH);
	}
	return (ERROR);
}

void	dropforks(t_philo *p)
{
	pthread_mutex_unlock(p->forkone);
	pthread_mutex_unlock(p->forktwo);
}

int	ft_usleep(long long int mseconds, t_philo *p)
{
	long long int	start;
	long long int	elapsed;

	start = get_time_ms();
	if (mseconds < 5)
		mseconds = 5;
	while (1)
	{
		if (getter(&p->dead, &p->deadslock))
			return (ERROR);
		elapsed = get_time_ms() - start;
		if (elapsed >= mseconds)
			break ;
		if (mseconds - elapsed > 1000)
			usleep(1000);
		else
			usleep((mseconds - elapsed) * 1000); // understand this
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

int	spread(t_data *d, int signal)
{
	int	i;

	i = -1;
	if (signal == DEATH)
	{
		while (++i < d->n_philos)
			setter(&d->philo[i]->dead, DEATH, &d->philo[i]->deadslock);
	}
	else if (signal == GO)
	{
		while (++i < d->n_philos)
			setter(&d->philo[i]->go, GO, &d->philo[i]->golock);
	}
	return (SUCCESS);
}
