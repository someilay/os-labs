#include <stdio.h>
#include <stdlib.h>


const int AMOUNT_OF_PROCESSES = 4;


const int ROWS = AMOUNT_OF_PROCESSES;
const int COLUMNS = 7;
const int PROCESS_ID_I = 0;
const int ARRIVAL_TIME_I = 1;
const int BURST_TIME_I = 2;
const int COMPLETION_TIME_I = 3;
const int WAITING_TIME_I = 4;
const int TURNAROUND_TIME_I = 5;
const int REMAINING_BURST_TIME_I = 6;


void swap_with_shift(int** table, int i, int j){
	int* temp = table[j];

	for (int k = j - 1; k >= i; k--){
		table[k + 1] = table[k];
	}

	table[i] = temp;
}


void sort_table(int** table, int rows, int column){
	if (rows > 1){
		sort_table(table, rows / 2, column);
		sort_table(table + rows / 2, rows - rows / 2, column);

		int** temp = (int**)malloc(sizeof(int*) * rows);
		int i = 0, j = rows / 2, k = 0;
		while (i < rows / 2 && j < rows){
			if (table[i][column] > table[j][column]){
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
	int quantum = 0;
	int c_time = 0;
	int is_done = 0;

	int c_slot = 0;
	int flow_size = ROWS;
	int* flow = (int*)malloc(flow_size * sizeof(int));

	printf("N = %d\nQuantum is ", ROWS);
	scanf("%d", &quantum);
	for (int i = 0; i < ROWS; i++){
		printf("==================================================\nProcess # %d\nArrival Time is ", i + 1);
		scanf("%d", &table[i][ARRIVAL_TIME_I]);
		printf("Burst Time is ");
		scanf("%d", &table[i][BURST_TIME_I]);
		table[i][REMAINING_BURST_TIME_I] = table[i][BURST_TIME_I];
		table[i][PROCESS_ID_I] = i + 1;
	}

	printf("					                     Statistics			                                          \n");
	printf("==================================================================================================================================\n");
	printf("	Process id	  Arrival Time	      Burst Time        Completion Time        Waiting Time        Turnaround Time        \n");
	sort_table(table, ROWS, ARRIVAL_TIME_I);
	c_time = table[0][ARRIVAL_TIME_I];

	while (!is_done){
		is_done = 1;
		for (int i = 0; i < ROWS; i++){
			if (table[i][REMAINING_BURST_TIME_I]){
				if (table[i][REMAINING_BURST_TIME_I] > quantum){
					c_time += quantum;
					table[i][REMAINING_BURST_TIME_I] -= quantum;
				} else {
					c_time += table[i][REMAINING_BURST_TIME_I];
					table[i][WAITING_TIME_I] = c_time - table[i][BURST_TIME_I] - table[i][ARRIVAL_TIME_I];
					table[i][REMAINING_BURST_TIME_I] = 0;
				}

				if (!(c_slot < flow_size)){
					flow_size *= 2;
					flow = (int*)realloc(flow, flow_size * sizeof(int));
				}
				flow[c_slot] = table[i][PROCESS_ID_I];
				c_slot++;

				is_done = 0;
			}
		}
	}

	for (int i = 0; i < ROWS; i++){
		table[i][TURNAROUND_TIME_I] = table[i][BURST_TIME_I] + table[i][WAITING_TIME_I];
		table[i][COMPLETION_TIME_I] = table[i][TURNAROUND_TIME_I] + table[i][ARRIVAL_TIME_I];
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

	for (int i = 0; i < c_slot; i++){
		printf("P.%d", flow[i]);
		if (i != c_slot - 1){
			printf(" -----> ");
		}
	}
	printf("\n");

	free_table(table, ROWS);
	free(flow);

	return 0;
}
