/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:28:33 by bposa             #+#    #+#             */
/*   Updated: 2024/08/14 14:54:37 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	butler(t_data *d)
{
	int				i;
	long long int	t;

	i = -1;
	t = 0;
	while (1)//!getter(&d->death, &d->dielock)
	{
		i = -1;
		t = get_time_ms();
		while (++i < d->n_philos)
		{
			if ((lastmealget(d->philo[i]) != 0
				&& t - lastmealget(d->philo[i]) >= d->die_t)
				|| checker(d, MEAL) == SUCCESS)
			{
				setter(&d->death, DEATH, &d->dielock);
				break ;
			}
		}
		if (i != d->n_philos)
			break ;
	}
	if (checker(d, MEAL) != SUCCESS)
		printf("%lld %d died\n", t - d->starttime, i + 1);//d.death should also have the philo ID
}

/*
	-if all fails, give philo a pointer to global death and have him set it
	-fix long sleep time short death - it shuld end instantly after death
	-delete bloat
	-norm pass
	-mock eval
*/
void	life(t_philo *p)
{
	pthread_mutex_lock(p->dlock);
	pthread_mutex_unlock(p->dlock);
	while (!getter(p->death, p->dlock))
	{
		if (routine(p) != SUCCESS || (p->meals != -1
			&& getter(&p->meals_had, &p->lmeallock) >= p->meals))
		{
			// setter(p->death, p->id, &p->dlock);
			break ;
		}
	}
	setter(&p->end, 1, &p->readylock);
}

int	routine(t_philo *p)
{
	if (action(THINK, p->id, "is thinking", p))
		return (DEATH);
	if (p->id % 2 == 0 && !getter(&p->run, &p->readylock) && wait_ms(p->sleep_t / 3, p))
		return (DEATH);
	pthread_mutex_lock(p->forkone);
	if (action(FORK, p->id, "has taken a fork", p))
		return (DEATH);
	if (!p->forktwo && lastmealset(p))
		return (ifonlyonefork(p));
	pthread_mutex_lock(p->forktwo);
	if (action(FORKEAT, p->id, "has taken a fork", p))
		return (DEATH);
	if (wait_ms(p->eat_t, p) != SUCCESS)
	{
		dropforks(p);
		return (DEATH);
	}
	dropforks(p);
	if (action(SLEEP, p->id, "is sleeping", p) || wait_ms(p->sleep_t, p))
		return (DEATH);
	p->run++;
	return (SUCCESS);
}

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
		setter(p->death, DEATH, p->dlock);
		return (DEATH);
	}
	return (ERROR);
}

void	dropforks(t_philo *p)
{
	pthread_mutex_unlock(p->forkone);
	pthread_mutex_unlock(p->forktwo);
}

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

void	printer(int arg, char *str, t_philo *p)
{
	pthread_mutex_lock(p->prlock);
	printf("%lld %d %s\n", get_time_ms() - *p->start_t, arg, str);
	pthread_mutex_unlock(p->prlock);
}

int	spreadgo(t_data *d)
{
	int	i;

	i = -1;
		while (++i < d->n_philos)
			setter(&d->philo[i]->go, GO, &d->philo[i]->golock);
	return (SUCCESS);
}

int main(int argc, char **argv)
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
		usleep(400);
	return (cleanerr(d, SUCCESS, d->n_philos));
}
