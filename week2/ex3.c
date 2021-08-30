#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


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
		if ((input[i] >= '0' && input[i] <= '9' && !((j > 1 && !has_minus && buffer[0] == '0') || (j > 2 && has_minus && buffer[1] == '0'))) || (input[i] == '-' && j == 0)){
			if (input[i] == '-')
				has_minus = 1;

			buffer[j] = input[i];
			j++;
		}
		else if ((input[i] == ' ' || input[i] == '\n') && ((j > 0 && !has_minus) || (j > 1 && has_minus))){
			if (k >= amount){
				printf("To manny numbers are entered!\n");
				free(input);
				return 0;
			}

			buffer[j] = '\0';
			res = strtoll(buffer, NULL, 10);
			out[k] = (int)res;
			j = 0;
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

	free(input);
	return 1;
}


void print_triangle(int height){
	for (int i = 0; i < height; i++){
		int spaces = height - 1 - i;
		for (int j = 0; j < spaces; j++){
			printf(" ");
		}
		for (int j = 0; j < 2 * i + 1; j++){
			printf("*");
		}
		printf("\n");
	}
}


int main(){
	printf("Enter triangle's height:\n");
	int arr[1];
	int result_of_parsing = read_ints((int*)&arr, 1);

	if (result_of_parsing){
		int height = arr[0];

		if (height > 0){
			print_triangle(height);
		}
		else {
			printf("Height must be positive number!\n");
		}
	}

	return 0;
}
