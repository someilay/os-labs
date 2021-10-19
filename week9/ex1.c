#include <stdio.h>
#include <stdlib.h>


#define AMOUNT_OF_PAGES 10000


int is_in_memory(int* memory, int page, int n){
	for (int i = 0; i < n; i++){
		if (memory[i] == page){
			return 1;
		}
	}
	return 0;
}


void insert_page(int* counters, int* r_bits, int* memory, int page, int n, int m){
	counters[page - 1] = 0;
	r_bits[page - 1] = 0;

	// If there is free space
	for (int i = 0; i < n; i++){
		if (memory[i] == 0){
			memory[i] = page;
			return;
		}
	}

	// All cells are occupied
	int min_index = 0;
	int min_counter = counters[memory[min_index] - 1];
	for (int i = 1; i < n; i++){
		if (counters[memory[i] - 1] < min_counter || (counters[memory[i] - 1] == min_counter && memory[i] < memory[min_index])){
			min_index = i;
			min_counter = counters[memory[i] - 1];
		}
	}
	memory[min_index] = page;
}


void simulate(int* ts, int* as, int n, int m){
	int* counters = (int*)malloc(sizeof(int) * AMOUNT_OF_PAGES);
	int* r_bits = (int*)malloc(sizeof(int) * AMOUNT_OF_PAGES);
	int* memory = (int*)malloc(sizeof(int) * n);

	int hit_c = 0;
	int miss_c = 0;

	for (int i = 0; i < n; i++){
		memory[i] = 0;
	}
	for (int i = 0; i < m; i++){
		counters[i] = 0;
		r_bits[i] = 0;
	}

	for (int i = 0; i < m; i++){
		if (i > 0){
			if (ts[i] != ts[i - 1]){
				for (int j = 0; j < n; j++){
					if (memory[j] != 0) {
						counters[memory[j] - 1] = (counters[memory[j] - 1] >> 1) | (r_bits[memory[j] - 1] << 7);
						r_bits[memory[j] - 1] = 0;
					}
				}
			}
		}

		int page_in_memory = is_in_memory(memory, as[i], n);
		printf("%d\n", page_in_memory);

		if (page_in_memory){
			hit_c++;
		} else {
			insert_page(counters, r_bits, memory, as[i], n, m);
			miss_c++;
		}
		r_bits[as[i] - 1] = 1;
	}

	if (miss_c != 0) {
		printf("%lf\n", (double)hit_c / miss_c);
	} else {
		printf("inf\n");
	}

	free(counters);
	free(r_bits);
	free(memory);
}


int main(){
	int n, m;
	scanf("%d %d", &n, &m);
	int* ts = (int*)malloc(sizeof(int) * m);
	int* as = (int*)malloc(sizeof(int) * m);
	for (int i = 0; i < m; i++){
		scanf("%d %d", &ts[i], &as[i]);
	}

	simulate(ts, as, n, m);

	free(ts);
	free(as);
	return 0;
}
