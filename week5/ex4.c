#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


typedef unsigned long long big_int;


struct stack {
	big_int* stack;
	unsigned int count;
	unsigned int size;
} typedef Stack;


Stack* create_stack(unsigned int size){
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	stack->count = 0;
	stack->size = size;
	stack->stack = (big_int*)malloc(sizeof(big_int) * size);
	return stack;
}


void push(Stack* stack, big_int value) {
	if (!(stack->count < stack->size)){
		stack->stack = (big_int*)realloc(stack->stack, sizeof(big_int) * 2 * stack->size);
		stack->size *= 2;
	}
	stack->stack[stack->count] = value;
	stack->count++;
}


big_int pop(Stack* stack){
	big_int value = stack->stack[stack->count - 1];
	stack->count--;

	return value;
}


struct data {
        Stack* stack;
        pthread_mutex_t* mutex;
} typedef Data;


Data* create_data(Stack* stack, pthread_mutex_t* mutex){
	Data* data = (Data*)malloc(sizeof(Data));
	data->stack = stack;
	data->mutex = mutex;
	return data;
}


void* core_lcm(void* args){
	Data* data = (Data*)args;
	Stack* stack = data->stack;
	pthread_mutex_t* mutex = data->mutex;

	while(1){
		pthread_mutex_lock(mutex);

		if (stack->count <= 1){
			pthread_mutex_unlock(mutex);
			break;
		}

		big_int a = pop(stack);
		big_int b = pop(stack);
		pthread_mutex_unlock(mutex);

		for (big_int i = a > b ? a : b; i < a * b + 1; i++){
			if (i % a == 0 & i % b == 0){
				pthread_mutex_lock(mutex);

				push(stack, i);

				pthread_mutex_unlock(mutex);
				break;
			}
		}
	}

	free(data);
	return NULL;
}


big_int multi_thread_lcm(unsigned int* array, int length, unsigned short threads_amount){
	pthread_mutex_t mutex;
	pthread_t thread_ids[threads_amount];
	Stack* stack = create_stack(threads_amount);
	pthread_mutex_init(&mutex, NULL);

	if ( pthread_mutex_init( &mutex, NULL) != 0 )
		printf( "mutex init failed\n" );

	for (int i = 0; i < length; i++){
		push(stack, array[i]);
	}

	for (int i = 0; i < threads_amount; i++){
		pthread_create(&thread_ids[i], NULL, core_lcm, create_data(stack, &mutex));
	}

	for (int i = 0; i < threads_amount; i++){
		pthread_join(thread_ids[i], NULL);
	}

	big_int result = pop(stack);
	pthread_mutex_destroy(&mutex);
	free(stack->stack);
	free(stack);
	return result;
}


int main(){
	exit(EXIT_SUCCESS);
}
