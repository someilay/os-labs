#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


const int STR_MAX = 256;


int main(){
	char command[STR_MAX];
	int stoped = 0;

	while(!stoped){
		printf("> ");
		scanf("%s", command);

		if (strcmp(command, "exit") == 0){
			printf("Buy\n");
			break;
		}

		system(command);
	}
}
