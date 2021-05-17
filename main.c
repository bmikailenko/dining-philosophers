// Ben Mikailenko

#include "random.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

int thinkTime[5];
const char *names[] = {"Plato", "Aristotle", "Diogenes", "Socrates", "Descartes"};
pthread_mutex_t chopsticks[5];
pthread_t philosophers[5];

struct args {
    int i;
};

void get_chopsticks(int philosopher) {
	pthread_mutex_lock(&chopsticks[philosopher]);
	pthread_mutex_lock(&chopsticks[(philosopher+1)%5]);
	printf("%s, I am taking chopsticks %d and %d\n",
		names[philosopher], philosopher, (philosopher+1)%5);
}

void put_away_chopsticks(int philosopher) {
	pthread_mutex_unlock(&chopsticks[philosopher]);
	pthread_mutex_unlock(&chopsticks[(philosopher+1)%5]);
	printf("%s, I am putting down chopsticks %d and %d\n",
		names[philosopher], philosopher, (philosopher+1)%5);
}

void wait_for_chopsticks(int philosopher){
	printf("%s, I am waiting\n", names[philosopher]);
}

void eat(int philosopher){
	printf("%s, I am eating\n", names[philosopher]);

	int time = randomGaussian(9, 3);
	if (time < 0) time = 0;
	sleep(time);

}

void think(int philosopher){
	printf("%s, I am thinking\n", names[philosopher]);

	int time = randomGaussian(9, 7);
	if (time < 0) time = 0;
	sleep(time);
	thinkTime[philosopher] += time;

	printf("%s, I have thought for %d seconds\n", names[philosopher], thinkTime[philosopher]);
}

void *philosopher(void *i){
	while (thinkTime[(intptr_t) i] < 100){

		// wait at mutex
		wait_for_chopsticks((intptr_t)i);

		// try to get chopsticks
		get_chopsticks((intptr_t)i);

		// eat if got chopsticks
		eat((intptr_t)i);

		// put away chopsticks
		put_away_chopsticks((intptr_t)i);

		// think
		think((intptr_t)i);

  }
	printf("%s, I am leaving the table. bye!\n", names[(intptr_t)i]);
	return 0;
}

int main(int argc, char *argv[]){

	intptr_t i;

	for (i = 0; i < 5; i++)
		if(pthread_mutex_init(&chopsticks[i], NULL) != 0) {
			printf("%d, Couldn't initialize mutex\n", errno);
			exit(1);
		}

	for (i = 0; i < 5; i++)
		pthread_create(&philosophers[i], NULL, philosopher, (void *)i);

	for (i = 0; i < 5; i++)
		pthread_join(philosophers[i], NULL);

	for (i = 0; i < 5; i++)
		pthread_mutex_destroy(&chopsticks[i]);

	return 0;
}
