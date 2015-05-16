#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define CNT 10
#define DELAY 10000000

sem_t stick[CNT];

int ate[CNT];

void *data(void *arg) {
	int delay = DELAY;	
	while(1) {
		int i;

		if(delay != 0) {
			delay--;
			continue;
		}

		for(i = 0; i < CNT; i++) {
			printf("%7x", ate[i]);
		}
		printf("\n");
		delay = DELAY;
	}
	return NULL;
}

void *person(void *arg) {
	int left = (int)arg;
	int right = (left + 1) % CNT;
	int l=0, r=0;
	
	ate[left] = 0;

	while(1) {
		l = sem_trywait(&stick[left]);
		r = sem_trywait(&stick[right]);
		if((l = -1) && (r == 0)){
			sem_post(&stick[right]);
		}
		if ((l = 0) && (r == -1)){
			sem_post(&stick[left]);
		}
		else if ((l + r) == 0){	
			ate[left]++;
			sem_post(&stick[left]);
			sem_post(&stick[right]);
		}
	}
	return NULL;
}


int main(int argc, char *argv[]) {

	pthread_t roger[CNT];
	pthread_t print;
	int i;
	
	for(i = 0; i < CNT; i++) {
		sem_init(&stick[i], 0, 1);
	}

	for(i = 0; i < CNT; i++) {
		pthread_create(&roger[i], NULL, person, (void*)i);
	}
	pthread_create(&print, NULL, data, NULL);

	for(i = 0; i < CNT; i++) {
		pthread_join(roger[i], NULL);
	}
	pthread_join(print, NULL);

	for(i = 0; i < CNT; i++) {
		sem_destroy(&stick[i]);
	}

	return EXIT_SUCCESS;
}