#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>


int read_ints(int *out, short amount){
	const size_t MAX_STR = 12 * amount;
	const int MAX_INT_LEN = 10;

	char *input = (char*)malloc(MAX_STR * sizeof(char));
	char buffer[MAX_INT_LEN + 1];
	int input_len = (int)getline(&input, (size_t*)&MAX_STR, stdin);

	int i = 0;
	int j = 0;
	int k = 0;
	int has_minus = 0;
	long long res = 0;

	if (amount <= 0){
		printf("Quantity of numbers must be positive!\n");
		free(input);
		return 0;
	}

	while (i < input_len){
		if ((input[i] >= '0' && input[i] <= '9' && !((j >= 1 && !has_minus && buffer[0] == '0') || (j >= 2 && has_minus && buffer[1] == '0'))) || (input[i] == '-' && j == 0)){
			if (input[i] == '-')
				has_minus = 1;

			buffer[j] = input[i];
			j++;
		}
		else if ((input[i] == ' ' || input[i] == '\n') && ((j > 0 && !has_minus) || (j > 1 && has_minus))){
			if (k >= amount){
				printf("To many numbers are entered!\n");
				free(input);
				return 0;
			}

			buffer[j] = '\0';
			res = strtoll(buffer, NULL, 10);
			out[k] = (int)res;
			j = 0;
			has_minus = 0;
			k++;
		}
		else {
			printf("Unexpected symbol '%c' in pos %d!\n", input[i], i);
			free(input);
			return 0;
		}

		if ((j > MAX_INT_LEN && !has_minus) || (j > MAX_INT_LEN + 1 && has_minus) || res > INT_MAX || res < INT_MIN){
			printf("To big or too small number for int32!\n");
			free(input);
			return 0;
		}

		i++;
	}

	if (k < amount){
		printf("Too few numbers are entered!\n");
		free(input);
		return 0;
	}

	free(input);
	return 1;
}


const int MAX_ARRAY_SIZE = 25;
const int MIN_ARRAY_SIZE = 5;


const int MAX_ARRAY_VALUE = 50;
const int MIN_ARRAY_VALUE = -50;


void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}


void qSort(int* arr, int length){
	if (length > 2){
		int pivot_i = rand() % length;
		int pivot = arr[pivot_i];

		swap(&arr[pivot_i], &arr[length - 1]);
		pivot_i = length - 1;

		int left = 0;
		int right = length - 2;

		while (left <= right){
			while (left <= right && arr[left] <= pivot){
				left++;
			}
			while (left <= right && arr[right] >= pivot){
				right--;
			}

			if (left < right){
				swap(&arr[left], &arr[right]);
			}
		}

		swap(&arr[pivot_i], &arr[left]);

		qSort(arr, left);
		qSort(arr + left + 1, length - left - 1);
	} else if (length == 2){
		if (arr[0] > arr[1]){
			swap(&arr[0], &arr[1]);
		}
	}
}


void generateArray(int** arr, int* length){
	int _length = rand() % (MAX_ARRAY_SIZE - MIN_ARRAY_SIZE + 1) + MIN_ARRAY_SIZE;
	*length = _length;
	*arr = (int*)malloc(sizeof(int) * _length);

	for (int i = 0; i < _length; i++){
		(*arr)[i] = rand() % (MAX_ARRAY_VALUE - MIN_ARRAY_VALUE + 1) + MIN_ARRAY_VALUE;
	}
}


void printArray(int* arr, int length){
	for (int i = 0; i < length; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
}


int main(){
	srand(time(NULL));
	int length;
	int* array;
	int arr[1];

	printf("Choose option:\n1 - generate and sort random array\n2 - enter array and sort it\n");
	int result_of_parsing = read_ints((int*)&arr, 1);

	if (result_of_parsing){
		int option = arr[0];

		switch(option){
			case 1:
				generateArray(&array, &length);
				printf("Generated array:\n");
				printArray(array, length);

				qSort(array, length);
				printf("After sorting:\n");
				printArray(array, length);
				free(array);
				break;
			case 2:
				printf("Enter size of array:\n");
				result_of_parsing = read_ints((int*)&arr, 1);

				if (result_of_parsing){
					length = arr[0];
					if (length > 0){
						printf("Enter values:\n");
						array = (int*)malloc(length * sizeof(int));
						result_of_parsing = read_ints(array, length);

						if (result_of_parsing){
							qSort(array, length);
							printf("After sorting:\n");
							printArray(array, length);
						}

						free(array);
					} else {
						printf("Length of array must be a positive number!\n");
					}
				}
				break;
			default:
				printf("The command was not recognized!\n");
				break;
		}
	}

	return 0;
}
