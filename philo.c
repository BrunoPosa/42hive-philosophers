/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:28:33 by bposa             #+#    #+#             */
/*   Updated: 2024/08/11 11:50:12 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	-Philo dies after some time with 200 130 60 60
	-it still sometimes prints "taken a fork" after death, e.g. with 200 130 60 60
	-try testing w/ fast debug optimization flag
	-./philo 200 599 200 200 should not die at 600 i think, but it often does. 
		AND rarely has some prints after death. 199 599 200 200 SHOUOLD die though.
	-Fix Helgrind
	-198 6 200 200 when giving small die_t, the printing continues a while after death
	-test 200 800 200 200 (none should die) Arthur fixed with every
		odd taking one fork first and even the other - BUT only on first run through. 
		Also waiting for sleep_t/2 to look for forks.
*/
void	routine(t_philo *p)
{
	pthread_mutex_lock(p->serlock);
	*p->served = *p->served + 1;
	pthread_mutex_unlock(p->serlock);
	while (getter(&p->go, p->serlock) != GO)
		usleep(200);
	while (1)
	{
		printer(p->id, "is thinking", p);
		if (getter(&p->firstrun, p->serlock) && p->id % 2 == 0)
			wait_ms(p->sleep_t / 2, p);
		pthread_mutex_lock(p->forkone);
		printer(p->id, "has taken a fork", p);
		if (!p->forktwo && lastmealset(p))
			break ;
		pthread_mutex_lock(p->forktwo);
		printer(p->id, "has taken a fork", p);
		printer(p->id, "is eating", p);
		lastmealset(p);
		wait_ms(p->eat_t, p);
		pthread_mutex_unlock(p->forkone);
		pthread_mutex_unlock(p->forktwo);
		printer(p->id, "is sleeping", p);
		if (getter(p->deathwatch, p->dlock) == DEATH || wait_ms(p->sleep_t, p))
			break ;
		setter(&p->firstrun, 0, p->serlock);
	}
}

void	butler(t_data *d)
{
	int				i;
	int				death;

	death = 0;
	while (getter(&d->served_n, &d->servedlock) != d->n_philos)
		usleep(200);
	d->starttime = get_time_ms();
	spread(d, GO);
	while (!death)
	{
		i = -1;
		death = getter(&d->death, &d->dielock);
		while (!death && ++i < d->n_philos && d->philo[i]->last_meal_t != 0)
		{
			if (get_time_ms() - lastmealget(d->philo[i]) >= d->die_t
				|| checker(d, MEAL) == SUCCESS)
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

void	printer(int arg, char *str, t_philo *p)
{
	if (getter(p->deathwatch, p->dlock) == DEATH && my_strncmp(str, "died", 4) != SUCCESS)
		return ;
	pthread_mutex_lock(p->prlock);
	printf("%lld %d %s\n", get_time_ms() - *p->start_t, arg, str);
	if (my_strncmp(str, "died", 4) != SUCCESS)
		pthread_mutex_unlock(p->prlock);
	if (my_strncmp(str, "is eating", 9) == 0)
		p->meals_had++;
}

/*
	-Implement mutex lock/unlock protections..
	-fix validator() to work using macros/enums
	-reorganize initialization
	-Limit philos in validation to 2000
	-ensure when someone dies, NOTHING gets ever printed after that
*/
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
	while (getter(&d->death, &d->dielock) != DEATH)
		usleep(400);
	return (cleanerr(d, SUCCESS, d->n_philos));
}
