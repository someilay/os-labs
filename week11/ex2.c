#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>


#define TARGET "/"


int main() {
	DIR* root_dir = opendir(TARGET);

	if (root_dir == NULL) {
		printf("CRYTICAL ERROR: %s folder is not found!\n", TARGET);
		exit(EXIT_FAILURE);
	}

	struct dirent* cur;
	while ((cur = readdir(root_dir)) != NULL) {
		printf("Entry: %s\n", cur->d_name);
	}

	return 0;
}


