#include <stdio.h>
#include <stdlib.h>


const int AMOUNT_OF_PROCESSES = 4;


const int ROWS = AMOUNT_OF_PROCESSES;
const int COLUMNS = 6;
const int PROCESS_ID_I = 0;
const int ARRIVAL_TIME_I = 1;
const int BURST_TIME_I = 2;
const int COMPLETION_TIME_I = 3;
const int WAITING_TIME_I = 4;
const int TURNAROUND_TIME_I = 5;


void swap_with_shift(int** table, int i, int j){
	int* temp = table[j];

	for (int k = j - 1; k >= i; k--){
		table[k + 1] = table[k];
	}

	table[i] = temp;
}


void sort_table_by_two_columns(int** table, int rows, int p_column, int s_column){
	if (rows > 1){
		sort_table_by_two_columns(table, rows / 2, p_column, s_column);
		sort_table_by_two_columns(table + rows / 2, rows - rows / 2, p_column, s_column);

		int** temp = (int**)malloc(sizeof(int*) * rows);
		int i = 0, j = rows / 2, k = 0;
		while (i < rows / 2 && j < rows){
			if (table[i][p_column] > table[j][p_column]){
				temp[k] = table[j];
				j++;
			} else if (table[i][p_column] < table[j][p_column]){
				temp[k] = table[i];
				i++;
			} else if (table[i][s_column] > table[j][s_column]){
				temp[k] = table[j];
                                j++;
			} else {
				temp[k] = table[i];
                                i++;
			}
			k++;
		}

		while (i < rows / 2){
			temp[k] = table[i];
			i++; k++;
		}

		while (j < rows){
			temp[k] = table[j];
			j++; k++;
		}

		for (i = 0; i < rows; i++){
			table[i] = temp[i];
		}

		free(temp);
	}
}


int** init_table(int rows, int columns){
	int** table = (int**)malloc(sizeof(int*) * rows);
	for (int i = 0; i < rows; i++){
                table[i] = (int*)malloc(sizeof(int) * columns);
        }
	return table;
}


void free_table(int** table, int rows){
	for (int i = 0; i < rows; i++){
                free(table[i]);
        }
        free(table);
}


int main(){
	int** table = init_table(ROWS, COLUMNS);
	float average_waiting = 0;
	float average_turnaround = 0;
	int c_time = 0;

	printf("N = %d\n", ROWS);
	for (int i = 0; i < ROWS; i++){
		printf("==================================================\nProcess # %d\nArrival Time is ", i + 1);
		scanf("%d", &table[i][ARRIVAL_TIME_I]);
		printf("Burst Time is ");
		scanf("%d", &table[i][BURST_TIME_I]);
		table[i][PROCESS_ID_I] = i + 1;
	}

	printf("					                     Statistics			                                          \n");
	printf("==================================================================================================================================\n");
	printf("	Process id	  Arrival Time	      Burst Time        Completion Time        Waiting Time        Turnaround Time        \n");
	sort_table_by_two_columns(table, ROWS, ARRIVAL_TIME_I, BURST_TIME_I);
	c_time = table[0][ARRIVAL_TIME_I];

	for (int i = 0; i < ROWS; i++){
		c_time += table[i][BURST_TIME_I];
		table[i][WAITING_TIME_I] = table[0][ARRIVAL_TIME_I] - table[i][ARRIVAL_TIME_I];
                for (int j = 0; j < i; j++){
                        table[i][WAITING_TIME_I] += table[j][BURST_TIME_I];
                }
		table[i][TURNAROUND_TIME_I] = table[i][BURST_TIME_I] + table[i][WAITING_TIME_I];
		table[i][COMPLETION_TIME_I] = table[i][TURNAROUND_TIME_I] + table[i][ARRIVAL_TIME_I];

		if (i != ROWS - 1){
			int min_index = i + 1;
			for (int j = i + 1; j < ROWS; j++){
				if (table[j][ARRIVAL_TIME_I] <= c_time && table[j][BURST_TIME_I] < table[min_index][BURST_TIME_I]){
					min_index = j;
				}
			}
			swap_with_shift(table, i + 1, min_index);
		}
	}

	for (int i = 0; i < ROWS; i++){
		average_waiting += table[i][WAITING_TIME_I];
		average_turnaround += table[i][TURNAROUND_TIME_I];
	}
	average_waiting = average_waiting / ROWS;
	average_turnaround = average_turnaround / ROWS;

	for (int i = 0; i < ROWS; i++){
		printf("        %10d        %12d        %10d        %15d        %12d        %15d\n",
		       table[i][PROCESS_ID_I], table[i][ARRIVAL_TIME_I], table[i][BURST_TIME_I], table[i][COMPLETION_TIME_I], table[i][WAITING_TIME_I], table[i][TURNAROUND_TIME_I]);
	}

	printf("==================================================================================================================================\n");
	printf("        %10s                                                                     %12f        %15f\n", "Average", average_waiting, average_turnaround);
	printf("==================================================================================================================================\n");
	printf("%18s\n", "Execution Flow");
	printf("==================================================================================================================================\n");

	for (int i = 0; i < ROWS; i++){
		printf("P.%d", table[i][PROCESS_ID_I]);
		if (i != ROWS - 1){
			printf(" -----> ");
		}
	}
	printf("\n");

	free_table(table, ROWS);

	return 0;
}
