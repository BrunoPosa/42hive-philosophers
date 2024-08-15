/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:28:33 by bposa             #+#    #+#             */
/*   Updated: 2024/08/15 13:22:31 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	action(t_action act, int arg, char *str, t_philo *p)
{
	pthread_mutex_lock(p->dlock);
	if (*p->death)
	{
		if (act == FORK)
			pthread_mutex_unlock(p->forkone);
		else if (act == FORKEAT)
		{
			pthread_mutex_unlock(p->forkone);
			pthread_mutex_unlock(p->forktwo);
		}
		pthread_mutex_unlock(p->dlock);
		return (DEATH);
	}
	printer(arg, str, p);
	if (act == FORKEAT)
	{
		if (p->meals)
		{
			printer(arg, "is eating", p);
			increment(&p->meals_had, &p->lmeallock);
			lastmealset(p);
		}
	}
	pthread_mutex_unlock(p->dlock);
	return (SUCCESS);
}

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
	return (SUCCESS);
}

void	life(t_philo *p)
{
	setter(&p->ready, SUCCESS, &p->readylock);
	while (getter(&p->go, &p->golock) != GO)
		usleep(200);
	if (p->forkone > p->forktwo)
		swapforks(p);
	timecorrection(p);
	action(THINK, p->id, "is thinking", p);
	while (!getter(p->death, p->dlock))
	{
		if (routine(p) || (getter(&p->meals, &p->lmeallock) != -1
			&& getter(&p->meals_had, &p->lmeallock) >= p->meals))
		{
			setter(p->death, p->id, p->dlock);
			break ;
		}
	}
	setter(&p->end, 1, &p->readylock);
}

void	butler(t_data *d)
{
	int				i;
	long long int	t;

	i = -1;
	t = 0;
	syncing(d);
	while (!getter(&d->death, &d->dielock))
	{
		i = -1;
		t = get_time_ms();
		while (++i < d->n_philos)
		{
			if ((lastmealget(d->philo[i]) != 0
					&& t - lastmealget(d->philo[i]) >= d->philo[i]->die_t)
				|| checker(d, MEAL) == SUCCESS)
			{
				setter(&d->death, DEATH, &d->dielock);
				break ;
			}
		}
		if (i != d->n_philos)
			break ;
	}
	maybeprint(d, i, t);
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
		usleep(200);
	return (cleanerr(d, SUCCESS, d->n_philos));
}
