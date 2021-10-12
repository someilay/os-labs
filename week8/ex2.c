#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(){
	int times = 10;
	size_t size = 150 * 1024 * 1024;

	for (int i = 0; i < times; i++){
		// I use rented VDS for running this code. My virtual machine
		// has only 1GB of RAM. When the program reaches the limit of memory,
		// VDS instantly kills the process. Therefore si and so fields
		// always equal 0.
		void* ptr = malloc(size);
		memset(ptr, 0, size);
		sleep(1);
	}
}
