#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>


#define TARGET "tmp"
#define MAX_SIZE 200


struct filePair {
	ino_t st_ino;
	char** names;
	unsigned int names_amount;
	unsigned int last;
} typedef FILEPAIR;


void add_name(FILEPAIR* pair, char* name) {
	if (name == NULL) {
		return;
	}

	if (pair->last >= pair->names_amount) {
		pair->names = (char**)realloc(pair->names, pair->names_amount * 2 * sizeof(char*));
		memset(pair->names + pair->names_amount, 0, pair->names_amount * sizeof(char*));
		pair->names_amount *= 2;
	}

	pair->names[pair->last] = (char*)malloc(MAX_SIZE * sizeof(char));
	strcpy(pair->names[pair->last], name);

	pair->last++;
}


FILEPAIR* create_filepair(ino_t st_ino, char* name) {
	FILEPAIR* pair = (FILEPAIR*)malloc(sizeof(FILEPAIR));
	pair->st_ino = st_ino;
	pair->names = (char**)malloc(sizeof(char*));
	memset(pair->names, 0, sizeof(char*));
	pair->names_amount = 1;
	pair->last = 0;
	add_name(pair, name);
	return pair;
}


void destroy_filepair(FILEPAIR* pair) {
	for (int i = 0; i < pair->names_amount; i++){
		if (pair->names[i] != NULL) {
			free(pair->names[i]);
		}
	}
	free(pair->names);
	free(pair);
}


struct linkSaver {
	unsigned int size;
	unsigned int index;
	FILEPAIR** pairs;
} typedef LINK_SAVER;



LINK_SAVER* create_saver() {
	LINK_SAVER* saver = (LINK_SAVER*)malloc(sizeof(LINK_SAVER*));
	saver->pairs = (FILEPAIR**)malloc(sizeof(FILEPAIR*));
	memset(saver->pairs, 0, sizeof(FILEPAIR*));
	saver->size = 1;
	saver->index = 0;
	return saver;
}


void add_link(LINK_SAVER* saver, ino_t st_ino, char* name) {
	for (int i = 0; i < saver->size; i++) {
		if (saver->pairs[i] != NULL && saver->pairs[i]->st_ino == st_ino) {
			add_name(saver->pairs[i], name);
			return;
		}
	}

	if (saver->index >= saver->size) {
		saver->pairs = (FILEPAIR**)realloc(saver->pairs, saver->size * 2 * sizeof(FILEPAIR*));
		memset(saver->pairs + saver->size, 0, saver->size * sizeof(FILEPAIR*));
		saver->size *= 2;
	}
	saver->pairs[saver->index] = create_filepair(st_ino, name);
	saver->index++;
}



void destroy_saver(LINK_SAVER* saver) {
	for (int i = 0; i < saver->size; i++) {
                if (saver->pairs[i] != NULL) {
                        destroy_filepair(saver->pairs[i]);
                }
        }

	free(saver->pairs);
	free(saver);
}



int main() {
	DIR* tmp_dir = opendir(TARGET);
	LINK_SAVER* saver = create_saver();


	if (tmp_dir == NULL) {
		printf("CRYTICAL ERROR: tmp folder is not found!\n");
		destroy_saver(saver);
		exit(EXIT_FAILURE);
	}

	printf("File -- Hard Links\n");
	struct dirent* cur;
	while ((cur = readdir(tmp_dir)) != NULL) {
		struct stat buff;
		char path[100] = TARGET;
		strcat(path, "/");
		strcat(path, cur->d_name);

		if(stat(path, &buff) == -1) {
			printf("CRYTICAL ERROR: cannot obtain %s statistics!\n", path);
			destroy_saver(saver);
                	exit(EXIT_FAILURE);
		}

		add_link(saver, buff.st_ino, cur->d_name);
	}

	for (int i = 0; i < saver->index; i++) {
		if (saver->pairs[i]->last > 1){
			for (int j = 0; j < saver->pairs[i]->names_amount; j++) {
				printf("%s -- ", saver->pairs[i]->names[j]);
				for (int k = 0; k < saver->pairs[i]->names_amount; k++) {
					printf("%s", saver->pairs[i]->names[k]);
					if (k != saver->pairs[i]->names_amount - 1) {
						printf(", ");
					}
				}
				printf("\n");
			}
		}
	}

	if (closedir(tmp_dir)) {
		printf("CRYTICAL ERROR: tmp folder cannot be closed!\n");
		destroy_saver(saver);
                exit(EXIT_FAILURE);
	}

	destroy_saver(saver);
	return 0;
}
