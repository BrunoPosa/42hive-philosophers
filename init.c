/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:33:44 by bposa             #+#    #+#             */
/*   Updated: 2024/08/15 14:05:05 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutex(t_data *d, int i)
{
	if (pthread_mutex_init(d->philo[i]->dlock, NULL) != SUCCESS)
		return (ERROR);
	if (pthread_mutex_init(&d->philo[i]->golock, NULL) != SUCCESS)
	{
		pthread_mutex_destroy(d->philo[i]->dlock);
		return (ERROR);
	}
	if (pthread_mutex_init(&d->philo[i]->readylock, NULL) != SUCCESS)
	{
		pthread_mutex_destroy(d->philo[i]->dlock);
		pthread_mutex_destroy(&d->philo[i]->golock);
		return (ERROR);
	}
	if (pthread_mutex_init(&d->philo[i]->lmeallock, NULL) != SUCCESS)
	{
		pthread_mutex_destroy(d->philo[i]->dlock);
		pthread_mutex_destroy(&d->philo[i]->golock);
		pthread_mutex_destroy(&d->philo[i]->readylock);
		return (ERROR);
	}
	return (SUCCESS);
}

int	init_philo(t_data *d, int i)
{
	d->philo[i] = malloc(sizeof(t_philo));
	if (!d->philo[i])
		return (ERROR);
	memset(d->philo[i], 0, sizeof(t_philo));
	d->philo[i]->id = i + 1;
	d->philo[i]->forkone = &d->forks[i];
	d->philo[i]->forktwo = &d->forks[(i + 1) % d->n_philos];
	if (d->n_philos == 1)
	{
		d->philo[i]->forkone = &d->forks[i];
		d->philo[i]->forktwo = NULL;
	}
	d->philo[i]->die_t = d->die_t;
	d->philo[i]->eat_t = d->eat_t;
	d->philo[i]->sleep_t = d->sleep_t;
	d->philo[i]->prlock = &d->printlock;
	d->philo[i]->dlock = &d->dielock;
	d->philo[i]->death = &d->death;
	d->philo[i]->start_t = &d->starttime;
	d->philo[i]->ready = -1;
	d->philo[i]->meals = d->n_meals;
	if (init_mutex(d, i) != SUCCESS)
		return (ERROR);
	return (SUCCESS);
}

int	init_mu_th(t_data *d)
{
	int	i;

	i = -1;
	while (++i < d->n_philos)
	{
		if (pthread_mutex_init(&d->forks[i], NULL) != SUCCESS)
			return (cleanerr(d, EMUTEX, i));
	}
	if (pthread_mutex_init(&d->printlock, NULL))
		return (cleanerr(d, ERROR, i));
	if (pthread_mutex_init(&d->dielock, NULL))
		return (cleanerr(d, ERROR, i));
	i = -1;
	while (++i < d->n_philos)
	{
		if (init_philo(d, i) != SUCCESS)
			return (cleanerr(d, EMALMUT, i));
		if (pthread_create(&d->philo[i]->thread, NULL,
				(void *)&life, d->philo[i]) != SUCCESS)
			return (cleanerr(d, ETHREAD, i));
	}
	if (pthread_create(&d->butler, NULL, (void *)&butler, d) != SUCCESS)
		return (cleanerr(d, ETHREAD, i));
	return (SUCCESS);
}

int	initor(char **argv, t_data *d)
{
	memset(d, 0, sizeof(t_data));
	d->n_philos = my_atoi(argv[1]);
	d->die_t = my_atoi(argv[2]);
	d->eat_t = my_atoi(argv[3]);
	d->sleep_t = my_atoi(argv[4]);
	d->n_meals = ERROR;
	if (argv[5])
		d->n_meals = my_atoi(argv[5]);
	d->philo = malloc(d->n_philos * sizeof(t_philo *));
	if (!d->philo || !memset(d->philo, 0, d->n_philos * sizeof(t_philo *)))
	{
		free(d);
		return (ERROR);
	}
	d->forks = malloc(d->n_philos * sizeof(pthread_mutex_t));
	if (!d->forks
		|| !memset(d->forks, 0, d->n_philos * sizeof(pthread_mutex_t)))
	{
		free(d->philo);
		free(d);
		return (ERROR);
	}
	if (init_mu_th(d) != SUCCESS)
		return (ERROR);
	return (SUCCESS);
}
