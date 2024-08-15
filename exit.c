/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:39:06 by bposa             #+#    #+#             */
/*   Updated: 2024/08/15 20:01:20 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ermsg(int status)
{
	if (status == EVAL)
		write(2, "\nUsage\n        ./philo Philos T_die T_eat"
			" T_sleep [optional: Meals]\n\n", 69);
	if (status == EINIT)
		printf("\n\e[31mInitialization failed, please try again\e[0m\n\n");
	if (status == EARG)
		write(2, "\nPhilosophers must be 1-400; other arguments 1-9999\n", 52);
	if (status == EARGC)
		write(2, "\nMust have 4 or 5 arguments.\n", 29);
	if (status == ETHREAD)
		write(2, "\nThreads!\n", 10);
	if (status == EMALMUT)
		write(2, "\nMalloc or mutex!\n", 18);
	if (status == EMUTEX)
		write(2, "\nMutex!\n", 8);
	if (status == ERROR)
		write(2, "\nError!\n", 8);
	if (status == EJOIN)
		write(2, "\nJoin!\n", 7);
	return (ERROR);
}

void	free_philos(t_data *d)
{
	while (--d->n_philos > -1)
	{
		free(d->philo[d->n_philos]);
		d->philo[d->n_philos] = NULL;
	}
	free(d->philo);
	d->philo = NULL;
	free(d->forks);
	d->forks = NULL;
}

int	normal_cleanup(t_data *d)
{
	int	i;

	i = d->n_philos;
	while (--i >= 0)
	{
		if (pthread_join(d->philo[i]->thread, NULL) != SUCCESS)
			return (ERROR);
		pthread_mutex_lock(&d->forks[i]);
		pthread_mutex_unlock(&d->forks[i]);
		pthread_mutex_lock(&d->philo[i]->deadslock);
		pthread_mutex_unlock(&d->philo[i]->deadslock);
		pthread_mutex_destroy(&d->forks[i]);
		pthread_mutex_destroy(&d->philo[i]->deadslock);
	}
	pthread_mutex_destroy(&d->printlock);
	return (SUCCESS);
}

int	mumalth_cleanup(t_data *d, int status, int initialized)
{
	int	i;

	i = d->n_philos;
	if (status == EMUTEX || status == EMALMUT)
	{
		while (--initialized >= 0)
		{
			pthread_mutex_destroy(&d->forks[initialized]);
			if (d->philo[initialized])
				free(d->philo[initialized]);
		}
	}
	else if (status == ETHREAD)
	{
		spread(d, DEATH);
		while (--i >= 0)
		{
			pthread_mutex_destroy(&d->forks[i]);
			pthread_mutex_destroy(&d->philo[i]->deadslock);
		}
		while (--initialized >= 0)
		{
			if (pthread_join(d->philo[initialized]->thread, NULL) != SUCCESS)
				return (ERROR);
		}
	}
	return (SUCCESS);
}

int	cleanerr(t_data *d, int status, int initialized)
{
	if (mumalth_cleanup(d, status, initialized) != SUCCESS)
		return (ermsg(EJOIN));
	else
	{
		if (normal_cleanup(d) != SUCCESS)
			return (ermsg(EJOIN));
	}
	if (pthread_join(d->butler, NULL) != SUCCESS)
		status = EJOIN;
	free_philos(d);
	free(d);
	return (ermsg(status));
}
