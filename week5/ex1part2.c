#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void* job(void* args){
	int *t = (int*)args;
	printf("Hello from Thread[%d]\n", *t);
	return NULL;
}


int main(){
	const int N = 10;
	pthread_t self = pthread_self();
	pthread_t thread_ids[N];

	printf("Starting creating threads...\n");
	for (int i = 0; i < N; i++){
		printf("Main thread[%lu] creates child thread[%lu]...\n", self, thread_ids[i]);
		pthread_create(&thread_ids[i], NULL, job, &i);
		pthread_join(thread_ids[i], NULL);
	}

	exit(EXIT_SUCCESS);
}
