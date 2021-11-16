#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>


#define SOURCE "input.txt"
#define TEMPLATE "Vector of E\n-new line-\nVector of A\n-new line-\nMatrix C\n-new line-\n-new line-\nMatrix R\n"
#define NO_ERRORS 0
#define PARSING_ERROR 1
#define INVALID_FORMAT 2
#define FILE_ERROR 3
#define INVALID_VALUES 4


#define DEADLOCK_STR "Deadlock occurred on process"
#define NO_DEADLOCK_STR "No deadlock!"


struct parse_error {
	unsigned int code;
	unsigned int line;
	unsigned int index;
	char* description;
} typedef PARSE_ERROR;


struct s_data {
	int* e;
	int* a;
	int** c;
	int** r;
	unsigned int amount_of_r;
	unsigned int amount_of_p;
} typedef DATA;


PARSE_ERROR* create_parse_error() {
	PARSE_ERROR* error = (PARSE_ERROR*)malloc(sizeof(PARSE_ERROR));
	error->code = NO_ERRORS;
	error->description = NULL;
	return error;
}


void destroy_parse_error(PARSE_ERROR* error) {
	if (error->description) {
		free(error->description);
	}
	free(error);
}


DATA* create_data() {
	DATA* data = (DATA*)malloc(sizeof(DATA));
	data->e = NULL;
	data->a = NULL;
	data->c = NULL;
	data->r = NULL;
	data->amount_of_r = 0;
	data->amount_of_p = 0;
	return data;
}


void destroy_data(DATA* data) {
	free(data->e);
	free(data->a);

	for (int i = 0; i < data->amount_of_p; i++) {
		free(data->c[i]);
		free(data->r[i]);
	}

	free(data->c);
	free(data->r);
	free(data);
}


char* readline(FILE* file, unsigned int* is_end) {
	unsigned int size = 1;
	unsigned int index = 0;
	char* result = (char*)malloc(size * sizeof(char));

	int c_int;
	int c_char;

	*is_end = 1;
	while ((c_int = fgetc(file)) != EOF) {
		*is_end = 0;
                c_char = (char)c_int;

		if (index >= size) {
			size *= 2;
			result = (char*)realloc(result, size * sizeof(char));
		}

		result[index] = c_char;
		index++;

		if (c_char == '\n') {
			break;
		}
        }
	result[index] = '\0';

	return result;
}


int* readints(FILE* file, unsigned int* is_end, unsigned int* amount, PARSE_ERROR* error) {
	unsigned int size = 1;
        unsigned int index = 0;
	const int MAX_INT_LEN = 10;
	char* newline = readline(file, is_end);
	int* newints = (int*)malloc(size * sizeof(int));

	if (!(*is_end)) {
		unsigned int str_size = strlen(newline);
		unsigned int buffered_index = 0;
		char buffer[MAX_INT_LEN + 1];
        	long long buffered_int;

		*amount = 0;
		for (unsigned int global_index = 0; global_index < str_size; global_index++) {
			if (newline[global_index] <= '9' && newline[global_index] >= '0') {
				if (buffered_index < MAX_INT_LEN + 1) {
					buffer[buffered_index] = newline[global_index];
					buffered_index++;
				} else {
					error->description = "Too long number for parse";
					error->index = global_index - buffered_index;
					error->code = PARSING_ERROR;
					free(newline);
        				return newints;
				}
			} else if (newline[global_index] == '-' && buffered_index == 0) {
				buffer[buffered_index] = newline[global_index];
                                buffered_index++;
			} else if ((newline[global_index] == ' ' || newline[global_index] == '\n') && buffered_index != 0) {
				if (buffer[buffered_index - 1] == '-') {
					error->description = "Token recognition error!";
                                        error->index = global_index - 1;
					error->code = PARSING_ERROR;
                                        free(newline);
                                        return newints;
				}
				buffer[buffered_index] = '\0';
				buffered_int = strtoll(buffer, NULL, 10);

				if (buffered_int > INT_MAX || buffered_int < INT_MIN) {
					error->description = "Too big/small number for int32!";
                                        error->index = global_index - buffered_index;
                                        error->code = PARSING_ERROR;
                                        free(newline);
                                        return newints;
				}

				if (index >= size) {
					size *= 2;
					newints = (int*)realloc(newints, size * sizeof(int));
				}
				newints[index] = (int)buffered_int;
				buffered_index = 0;
				(*amount)++;
				index++;
			} else if (!(newline[global_index] == ' ' || newline[global_index] == '\n')) {
				error->description = "Token recognition error!";
                                error->index = global_index;
				error->code = PARSING_ERROR;
                                free(newline);
                                return newints;
			}
		}
	}
	free(newline);
	return newints;
}


int** readbatch(FILE* file, unsigned int* is_end, unsigned int* columns, unsigned int* rows, unsigned int* line, PARSE_ERROR* error) {
	unsigned int size = 1;
	unsigned int amount = 1;
	int** result = (int**)malloc(size * sizeof(int*));

	*rows = 0;
	while (amount != 0 && !(*is_end)) {
		if (*rows >= size) {
			size *= 2;
			result = (int**)realloc(result, size * sizeof(int*));
		}
		int* temp_row = readints(file, is_end, &amount, error);
		if (*rows == 0) {
			*columns = amount;
		} else if (*columns != amount && amount != 0) {
			error->code = INVALID_FORMAT;
			error->description = "It's not a matrix!";
			return result;
		}

		if (amount != 0) {
			result[*rows] = temp_row;
	        	(*rows)++;
		} else {
			free(temp_row);
		}

		if (error->code) {
			error->line = *line;
			return result;
		}
		(*line)++;
	}

	return result;
}


int check_error(PARSE_ERROR* error, int line, int invalid_condition, char* invalid_format_description, unsigned int is_end, char* end_description) {
	if (error->code) {
                error->line = line;
                return 1;
        }
	if (invalid_condition) {
		error->description = invalid_format_description;
                error->code = INVALID_FORMAT;
		return 1;
	}
	if (is_end) {
		error->description = end_description;
                error->code = INVALID_FORMAT;
		return 1;
	}
	return 0;
}


DATA* readdata(char* source, PARSE_ERROR* error) {
	FILE* file = fopen(source, "r");
	DATA* data = create_data();

	if (file == NULL) {
                error->description = "Cannot open source file!";
		error->code = FILE_ERROR;
                return data;
        }

	unsigned int is_end = 0;
        unsigned int amount = 0;
	int* unused;

	data->e = readints(file, &is_end, &amount, error);
	if (check_error(error, 0, (amount == 0), "Cannot find vector E at the line 0!", is_end, "Unexpected end at line 0!")) {
		return data;
	}
	data->amount_of_r = amount;

	unused = readints(file, &is_end, &amount, error);
	if (check_error(error, 1, (amount != 0), "Line 1 must be empty!", is_end, "Unexpected end at line 1!")) {
		free(unused);
                return data;
        }

	data->a = readints(file, &is_end, &amount, error);
        if (check_error(error, 2, (amount == 0), "Cannot find vector A at the line 2!", is_end, "Unexpected end at line 2!") || \
	    check_error(error, 2, (amount != data->amount_of_r), "Vector A must have the same length as vector E!", is_end, "Unexpected end at line 2!")) {
                return data;
        }

	unused = readints(file, &is_end, &amount, error);
        if (check_error(error, 3, (amount != 0), "Line 3 must be empty!", is_end, "Unexpected end at line 3!")) {
                free(unused);
                return data;
        }

	unsigned int columns;
	unsigned int rows;
	unsigned int line = 4;
	data->c = readbatch(file, &is_end, &columns, &rows, &line, error);
	if (check_error(error, line, (columns == 0), "Cannot find matrix C at the line 4!", is_end, "Unexpected end!") || \
	    check_error(error, line, (columns != data->amount_of_r), "Matrix C must have the same number of columns as vector E and A!", is_end, "Unexpected end!")) {
                free(unused);
                return data;
        }
	data->amount_of_p = rows;

	line += 1;
	data->r = readbatch(file, &is_end, &columns, &rows, &line, error);
        if (check_error(error, line, (columns == 0), "Cannot find matrix R!", is_end, "Unexpected end!") || \
	    check_error(error, line, (columns != data->amount_of_r || rows != data->amount_of_p), "Matrix R must have the same dimensions as matrix C!", is_end, "Unexpected end!")) {
		free(unused);
                return data;
        }


	for (int i = 0; i < data->amount_of_r; i++) {
		if (data->e[i] < 0) {
			error->code = INVALID_VALUES;
			error->description = "E should not contain negative numbers!";
			error->line = 0;
			error->index = i;
			free(unused);
                	return data;
		}
		if (data->a[i] < 0) {
			error->code = INVALID_VALUES;
                        error->description = "A should not contain negative numbers!";
                        error->line = 2;
                        error->index = i;
                        free(unused);
                        return data;
                }
                for (int j = 0; j < data->amount_of_p; j++) {
                        if (data->c[j][i] < 0) {
                       		error->code = INVALID_VALUES;
                        	error->description = "C should not contain negative numbers!";
                        	error->line = j + 4;
                        	error->index = i;
                        	free(unused);
                        	return data;
                	}
                	if (data->r[j][i] < 0) {
                        	error->code = INVALID_VALUES;
                        	error->description = "R should not contain negative numbers!";
                        	error->line = j + data->amount_of_p + 5;
                        	error->index = i;
                        	free(unused);
                        	return data;
                	}
                }
        }

	for (int i = 0; i < data->amount_of_r; i++) {
		int occupied = data->a[i];
		for (int j = 0; j < data->amount_of_p; j++) {
			occupied += data->c[j][i];
		}
		if (occupied != data->e[i]) {
			error->code = INVALID_VALUES;
                        error->description = "Invalid vector E respect to C and A!";
                        error->line = 0;
                        error->index = i;
                        free(unused);
                	return data;
		}
	}

	return data;
}


int main() {
	PARSE_ERROR* error = create_parse_error();
	DATA* data = readdata(SOURCE, error);

	if (error->code != NO_ERRORS) {
		if (error->code == FILE_ERROR) {
			printf("FILE ERROR: %s\n", error->description);
		} else if (error->code == PARSING_ERROR) {
			printf("PARSING ERROR [%d, %d]: %s\n", error->line, error->index, error->description);
		} else if (error->code == INVALID_FORMAT) {
			printf("INVALID INPUT: %s\n", error->description);
			printf("\nTEMPLATE:\n%s", TEMPLATE);
		} else if (error->code == INVALID_VALUES) {
			printf("INVALID INPUT VALUES [%d, %d]: %s\n", error->line, error->index, error->description);
		}
		return EXIT_FAILURE;
	}

	int start = 0;
	int sutisfied_num = 0;
	int* sutisfied_p = (int*)malloc(data->amount_of_p * sizeof(int));
	int all_sutisfied = 1;
	int deadlock_num = 0;

	for (int i = 0; i < data->amount_of_p; i++) {
		sutisfied_p[i] = 0;
	}

	while (sutisfied_num < data->amount_of_p && all_sutisfied) {
		int k = 0;
		for (int i = 0; i < data->amount_of_p; i++) {
			if (!sutisfied_p[i]) {
				sutisfied_p[i] = 1;
				for (int j = 0; j < data->amount_of_r; j++) {
					if (data->r[i][j] > data->a[j]) {
						sutisfied_p[i] = 0;
						break;
					}
				}

				if (sutisfied_p[i]) {
					k++;
					sutisfied_num++;
					for (int j = 0; j < data->amount_of_r; j++) {
                                        	data->a[j] += data->c[i][j];
                                	}
				} else {
					deadlock_num = i;
				}
			}
		}

		if (k == 0) {
			all_sutisfied = 0;
		}
	}
	free(sutisfied_p);

	if (all_sutisfied) {
		printf("%s\n", NO_DEADLOCK_STR);
	} else {
		printf("%s %d!\n", DEADLOCK_STR, deadlock_num);
	}

	destroy_parse_error(error);
	destroy_data(data);
	return 0;
}
