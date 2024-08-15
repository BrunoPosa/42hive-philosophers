/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 12:47:59 by bposa             #+#    #+#             */
/*   Updated: 2024/08/15 13:16:59 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long int	get_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return(ERROR);
	return ((long long int)(time.tv_sec * 1000LL + time.tv_usec / 1000));
}

void	setter(void *var, int value, pthread_mutex_t *lock)
{
	if (lock && var)
	{
		pthread_mutex_lock(lock);
		*(int *)var = value;
		pthread_mutex_unlock(lock);
	}
	else
		printf("setter: ERROR with accessing mutex or struct's variable!\n");
}

void	printer(int arg, char *str, t_philo *p)
{
	pthread_mutex_lock(p->prlock);
	pthread_mutex_lock(p->dlock);
	printf("%lld %d %s\n", get_time_ms() - *p->start_t, arg, str);
	pthread_mutex_unlock(p->dlock);
	pthread_mutex_unlock(p->prlock);
}

void	syncing(t_data *d)
{
	int	i;
	long long int	t;

	i = -1;
	t = get_time_ms();
	while (checker(d, GO) != GO)
		usleep(200);
	timesetter(&d->starttime, &t, &d->dielock);
	while (++i < d->n_philos)
		setter(&d->philo[i]->go, GO, &d->philo[i]->golock);
}
