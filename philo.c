/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:28:33 by bposa             #+#    #+#             */
/*   Updated: 2024/08/15 23:50:04 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	routine(t_philo *p)
{
	if (p->runs && action(THINK, p->id, "is thinking", p))
		return (DEATH);
	if (!p->runs && p->id % 2 == 0)
		ft_usleep(p->sleep_t / 3, p);
	pthread_mutex_lock(p->forkone);
	if (action(FORK, p->id, "has taken a fork", p))
		return (DEATH);
	if (!p->forktwo && lastmealset(p))
		return (ifonlyonefork(p));
	pthread_mutex_lock(p->forktwo);
	if (action(FORKEAT, p->id, "has taken a fork", p))
		return (DEATH);
	if (ft_usleep(p->eat_t, p) != SUCCESS)
	{
		dropforks(p);
		return (DEATH);
	}
	dropforks(p);
	if (action(SLEEP, p->id, "is sleeping", p) || ft_usleep(p->sleep_t, p))
		return (DEATH);
	p->runs++;
	if (p->runs == 2147483647)
		p->runs = 3;
	return (SUCCESS);
}

void	life(t_philo *p)
{
	setter(&p->ready, SUCCESS, &p->readylock);
	while (getter(&p->go, &p->golock) != GO)
		usleep(200);
	action(THINK, p->id, "is thinking", p);
	while (1)
	{
		if (routine(p) != SUCCESS)
			break ;
	}
	// pthread_mutex_lock(&p->deadslock);
	// pthread_mutex_unlock(&p->deadslock);
	setter(&p->end, 1, &p->readylock);
}

int	checkeach(t_data *d, int i, long long int t, long long int *fed)
{
	long long int	lm;
	int				meals;

	lm = 0;
	meals = 0;
	pthread_mutex_lock(&d->philo[i]->lmeallock);
	lm = d->philo[i]->last_meal_t;
	if (lm != 0 && t - lm >= d->philo[i]->die_t)
	{
		pthread_mutex_unlock(&d->philo[i]->lmeallock);
		return (ERROR);
	}
	pthread_mutex_unlock(&d->philo[i]->lmeallock);
	meals = getter(&d->philo[i]->meals_had, &d->philo[i]->lmeallock);
	if (d->philo[i]->meals != -1 && meals == d->philo[i]->meals)
		*fed += 1;
	return (SUCCESS);
}

void	butler(t_data *d)
{
	int				i;
	long long int	t;
	long long int	fed;

	i = -1;
	t = 0;
	fed = 0;
	syncing(d);
	while (1)
	{
		i = -1;
		t = get_time_ms();
		while (++i < d->n_philos)
		{
			if (checkeach(d, i, t, &fed) != SUCCESS)
				break ;
			if (fed == d->n_philos)
				break ;
		}
		if (i != d->n_philos || fed == d->n_philos)
			break ;
	}
	spread(d, DEATH);
	if (fed != -1 && fed != d->n_philos)
		printf("%lld %d died\n", get_time_ms() - d->starttime, d->philo[i]->id);
}

int	main(int argc, char **argv)
{
	t_data	*d;

	if (validator(argc, argv) == ERROR)
		return (ermsg(EVAL));
	d = malloc(sizeof(t_data));
	if (!d)
		return (ermsg(EMALMUT));
	if (initor(argv, d) == ERROR)
		return (ermsg(EINIT));
	d->initdone = 1;
	while (endchecker(d) != SUCCESS)
		usleep(400);//maybe have a mutex locked and unlocked before and after this while for sync of thread deaths
	return (cleanerr(d, SUCCESS, d->n_philos));
}
