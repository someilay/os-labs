#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main(){
	const int n = 10;

	pid_t new_pid = fork();
	if (new_pid == 0){
		printf("Hello from child [%d - %d]\n", getpid(), n);
	} else {
		printf("Hello from parent [%d - %d]\n", new_pid, n);
		wait(NULL);
	}

	return 0;
}
