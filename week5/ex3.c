#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


char ITEM = 'I';
char EMPTY = 'E';

int PRODUCER_MAX_DELAY = 100;
int PRODUCER_MIN_DELAY = 50;
int CONSUMER_MAX_DELAY = 150;
int CONSUMER_MIN_DELAY = 20;


struct items {
	unsigned int number;
	unsigned int count;
	int consumer_active;
	int producer_active;
	int using_rand_sleep;
	int print_telemetry;
} typedef Items;


Items* create_items(int number, int using_rand_sleep, int print_telemetry){
	Items* items = (Items*)malloc(sizeof(Items));
	items->number = number;
	items->count = 0;
	items->consumer_active = 0;
	items->producer_active = 1;
	items->using_rand_sleep = using_rand_sleep;
	items->print_telemetry = print_telemetry;
	return items;
}


char* get_items(Items* items){
	char* f_items = (char*)malloc(sizeof(char) * (items->number + 1));
	for (int i = 0; i < items->count; i++){
		f_items[i] = ITEM;
	}
	for (int i = items->count; i < items->number; i++){
		f_items[i] = EMPTY;
	}
	f_items[items->number] = '\0';
	return f_items;
}


int rand_sleep(int min_delay, int max_delay){
	int rand_delay = rand() % (max_delay - min_delay + 1) + min_delay;
	usleep(rand_delay * 1000);
	return rand_delay;
}


void* produce(void* args){
	Items* items = (Items*)args;
	while(1){
		int rand_delay = -1;
		if (items->using_rand_sleep){
			rand_delay = rand_sleep(PRODUCER_MIN_DELAY, PRODUCER_MAX_DELAY);
		}
		if (items->producer_active){
			if (items->count < items->number){
				items->count++;

				if (items->count == 1){
					items->consumer_active = 1;
				}

				if (items->print_telemetry){
                                        char* f_items = get_items(items);
                                        printf("Items: %s. Produced new item! Elapsed time: %d ms\n", f_items, rand_delay);
                                        free(f_items);
                                }
			} else {
				items->producer_active = 0;
			}
		}
	}
	return NULL;
}


void* consume(void* args){
	Items* items = (Items*)args;
	while(1){
		int rand_delay = -1;
		if (items->using_rand_sleep){
                        rand_delay = rand_sleep(CONSUMER_MIN_DELAY, CONSUMER_MAX_DELAY);
                }
		if (items->consumer_active){
			if (items->count > 0){
				items->count--;

				if (items->count == items->number - 1){
					items->producer_active = 1;
				}

				if (items->print_telemetry){
					char* f_items = get_items(items);
                                	printf("Items: %s. Consumed new item! Elapsed time: %d ms\n", f_items, rand_delay);
                                	free(f_items);
				}
			} else {
				items->consumer_active = 0;
			}
		}
	}
	return NULL;
}


int main(){
	srand(time(NULL));

	Items* items = create_items(25, 0, 0);
	pthread_t consumer_id;
	pthread_t producer_id;
	pthread_create(&consumer_id, NULL, consume, items);
	pthread_create(&producer_id, NULL, produce, items);
	pthread_join(consumer_id, NULL);
	pthread_join(producer_id, NULL);

	free(items);
	exit(EXIT_SUCCESS);
}
