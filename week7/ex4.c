#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


#define MIN(a, b)


void* my_realloc(void * ptrmem, size_t size){
	void* new_ptrmem = malloc(size);

	if (size == 0){
		new_ptrmem = NULL;
	}

	if (ptrmem){
		if (new_ptrmem){
			size_t old_size = malloc_usable_size(ptrmem);
			memcpy(new_ptrmem, ptrmem, old_size < size ? old_size : size);
		}
		free(ptrmem);
	}

	return new_ptrmem;
}


int main(){
	int n1, n2;
	int *arr;
	printf("N1 is ");
	scanf("%d", &n1);

	if (n1 < 0){
		printf("N1 must be non negative!\n");
		return 0;
	}
	arr = (int*)malloc(n1 * sizeof(int));

	for (int i = 0; i < n1; i++){
		arr[i] = 11;
	}

	printf("Array:\n");
	for (int i = 0; i < n1; i++){
                printf("%d ", arr[i]);
        }
	printf("\nN2 is ");
	scanf("%d", &n2);

	if (n2 < 0){
                printf("N2 must be non negative!\n");
                return 0;
        }

	arr = my_realloc(arr, n2 * sizeof(int));

	for (int i = n1; i < n2; i++){
		arr[i] = 0;
	}

	printf("Array:\n");
        for (int i = 0; i < n2; i++){
                printf("%d ", arr[i]);
        }
	printf("\n");

	return 0;
}
