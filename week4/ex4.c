#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


const size_t MAX_COMMAND_LENGTH = 512;
const size_t MAX_NUMBER_OF_TOKENS = 128;


char* readline(int* length){
	char* input = (char*)malloc(sizeof(char) * MAX_COMMAND_LENGTH);
	*length = (int)getline(&input, (size_t*)&MAX_COMMAND_LENGTH, stdin);

	char* new_line = strstr(input, "\n");
	if (new_line){
		*new_line = '\0';
		(*length)--;
	}

	return input;
}


char** split(char* str, int* amount_of_tokens, char* separators){
	char** result = (char**)malloc(sizeof(char*) * MAX_NUMBER_OF_TOKENS);
	int* i = amount_of_tokens;
	*i = 0;

	char* token = strtok(str, separators);

	while (token != NULL){
		result[*i] = token;
		token = strtok(NULL, separators);
		(*i)++;
	}

	return result;
}


int main(){
	int stoped = 0;


	while (!stoped){
		printf("> ");
		int length;
		char* command = readline(&length);

		if (length > MAX_COMMAND_LENGTH){
			printf("Too large statement for executing!\n");
			continue;
		}


		if (strcmp(command, "exit") == 0){
			printf("Buy\n");
			break;
		}

		int amount_of_tokens;
		char** tokens = split(command, &amount_of_tokens, " ");

		pid_t new_pid = fork();
		if (new_pid == 0){
			char bin_path[MAX_COMMAND_LENGTH];
			strcpy(bin_path, "/bin/");

			tokens[0] = strcat(bin_path, tokens[0]);
			execve(tokens[0], tokens, NULL);
			return 0;
		} else {
			wait(NULL);
		}

		free(command);
	}

	return 0;
}
