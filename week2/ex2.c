#include <stdio.h>
#include <stdlib.h>
#include <string.h>


size_t STR_MAX = 256;


int main(){
	char *str = (char*)malloc(sizeof(char) * STR_MAX);
	int str_size;

	printf("Enter string (Max length is %ld):\n", STR_MAX);
	str_size = (int)(getline(&str, &STR_MAX, stdin) - 1);
	str[str_size] = '\0';

	for (int i = 0; i < str_size / 2; i++){
		char buf = str[i];
		str[i] = str[str_size - i - 1];
		str[str_size - i - 1] = buf;
	}

	printf("Your reversed string: %s; Len of your string: %d\n", str, str_size);

	return 0;
}
