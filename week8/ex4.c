#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>


int main(){
	size_t size = 50 * 1024 * 1024;
	int times = 10;

	for (int i = 0; i < times; i++){
		struct rusage usage;
		void* ptr = malloc(size);
		memset(ptr, 0, size);
		getrusage(RUSAGE_SELF, &usage);
		printf("ru_maxrss: %lu, ru_ixrss: %lu, ru_idrss: %lu, ru_isrss: %lu, ru_minflt: %lu, ru_majflt: %lu, ru_nswap: %lu, ru_inblock: %lu, ru_oublock: %lu, ru_msgsnd: %lu, ru_msgrcv: %lu, ru_nsignals: %lu, ru_nvcsw: %lu, ru_nivcsw: %lu\n",
			usage.ru_maxrss, usage.ru_ixrss, usage.ru_idrss, usage.ru_isrss, usage.ru_minflt, usage.ru_majflt, usage.ru_nswap, usage.ru_inblock, usage.ru_oublock, usage.ru_msgsnd, usage.ru_msgrcv,
			usage.ru_nsignals, usage.ru_nvcsw, usage.ru_nivcsw);
		sleep(1);
	}

	return 0;
}
