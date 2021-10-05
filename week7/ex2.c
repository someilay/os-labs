#include <stdio.h>
#include <stdlib.h>


int main(){
	int n;
	printf("N is ");
	scanf("%d", &n);

	int* arr = (int*)calloc(n, sizeof(int));

	for (int i = 0; i < n; i++){
		arr[i] = i;
	}

	printf("Array:\n");
	for (int i = 0; i < n; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");

	free(arr);

	return 0;
}
