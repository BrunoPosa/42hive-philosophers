/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:28:33 by bposa             #+#    #+#             */
/*   Updated: 2024/08/11 21:42:34 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philolife(t_data *d)
{
	int				id;
	long long int	t;

	pthread_mutex_lock(&d->lock);
	pthread_mutex_unlock(&d->lock);
	t = get_time_ms();
	if (id % 2 == 0)
		wait_ms(5, d);
	while (!getter(&d->death, &d->dielock) && t - d->tfed[id] < d->tdie)
	{
		if (routine(id, d) || (d->meals != -1 && d->fedcount[id] >= d->meals))
			break ;
		t = get_time_ms();
	}
	increment_id(&d->ids, &d->lock);
}

void	increment_id(int *id, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	*id = *id + 1;
	pthread_mutex_unlock(mutex);
}

int	routine(int id, t_data *d)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	assignforks(id, d, first, second);
	if (action(THINK, id, "is thinking", d))
		return (DEATH);
	pthread_mutex_lock(first);
	if (action(FORK, id, "has taken a fork", d) || !second)
		return (DEATH);
	pthread_mutex_lock(second);
	if (action(FORKTWO, id, "has taken a fork", d))
		return (DEATH);
	if (action(EAT, id, "is eating", d))
		return (DEATH);
	d->tfed[id] = get_time_ms();
	if (getter(&d->death, &d->dielock) || wait_ms(d->teat, d))
		return (DEATH);
	pthread_mutex_unlock(first);
	pthread_mutex_unlock(second);
	if (action(SLEEP, id, "is sleeping", d))
		return (DEATH);
	if (getter(&d->death, &d->dielock) || wait_ms(d->tsleep, d))
		return (DEATH);
	return (SUCCESS);
}

void	assignforks(int id, t_data *d, void *ff, void *sf)
{//maybe needs casting
	ff = &d->fork[id];
	sf = &d->fork[(id + 1) % d->n_philos];
	if (ff == sf)
		sf = NULL;
}

int	action(t_action act, int arg, char *str, t_data *d)
{
	pthread_mutex_lock(&d->dielock);
	if (d->death)
	{
		if (act == )

		pthread_mutex_unlock(&d->dielock);
		return (DEATH);
	}
	printer(arg, str, d);
	pthread_mutex_unlock(&d->dielock);
	return (SUCCESS);
}

void	printer(int arg, char *str, t_data *d)
{
	pthread_mutex_lock(&d->printlock);
	printf("%lld %d %s\n", get_time_ms() - d->tstart, arg, str);
	pthread_mutex_unlock(&d->printlock);
}


void	monitor(t_data *d)
{
	int	i;
	int	death;

	death = 0;
	while (getter(&d->ids, &d->lock) != d->n_philos)
		usleep(200);
	d->tstart = get_time_ms();
	while (!death)
	{
		i = -1;
		death = getter(&d->death, &d->dielock);
		while (!death && ++i < d->n_philos && d->philo[i]->last_meal_t != 0)
		{
			if (get_time_ms() - lastmealget(d->philo[i]) >= d->die_t)
				// || checker(d, MEAL) == SUCCESS)
			{
				death = 1;
				break ;
			}
		}
		if (death)
			setter(&d->death, DEATH, &d->dielock);
	}
	if (checker(d, MEAL) != SUCCESS)
		printer(d->philo[i]->id, "died", d->philo[i]);
}

//Change so each philo has only pointers to a single mutex in the  otherwise its too slow i think, and too many mutexes
int	spread(t_data *d, int signal)
{
	int	i;

	i = -1;
	if (signal == GO)
	{
		pthread_mutex_lock(&d->servedlock);
		while (++i < d->n_philos)
			d->philo[i]->go = GO;
		pthread_mutex_unlock(&d->servedlock);
	}
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
	if (init_all(argv, d) == ERROR)
		return (ermsg(EINIT));
	while (getter(&d->ids, &d->lock) != 2 * d->n_philos)
		wait_ms(1, d);
	return (cleanerr(d, SUCCESS, d->n_philos));
}
