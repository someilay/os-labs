#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


const int N = 3;
const int SLEEP_TIME = 5;


int main(){
	for (int i = 0; i < N; i++){
		fork();
	}
	sleep(SLEEP_TIME);
	wait(NULL);

	return 0;
}
