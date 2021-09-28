#include <stdio.h>
#include <stdlib.h>


const int ROWS = 4;
const int COLUMNS = 6;
const int PROCESS_ID_I = 0;
const int ARRIVAL_TIME_I = 1;
const int BURST_TIME_I = 2;
const int COMPLETION_TIME_I = 3;
const int WAITING_TIME_I = 4;
const int TURNAROUND_TIME_I = 5;


void swap(int** a, int** b){
	int* temp = *a;
	*a = *b;
	*b = temp;
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


int main(){
	int** table = (int**)malloc(sizeof(int*) * ROWS);
	float average_waiting = 0;
	float average_turnaround = 0;
	for (int i = 0; i < ROWS; i++){
		table[i] = (int*)malloc(sizeof(int) * COLUMNS);
	}

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
	sort_table(table, ROWS, 1);

	for (int i = 0; i < ROWS; i++){
		table[i][WAITING_TIME_I] = table[0][ARRIVAL_TIME_I] - table[i][ARRIVAL_TIME_I];
		for (int j = 0; j < i; j++){
			table[i][WAITING_TIME_I] += table[j][BURST_TIME_I];
		}
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

	for (int i = 0; i < ROWS; i++){
		printf("P.%d", table[i][PROCESS_ID_I]);
		if (i != ROWS - 1){
			printf(" -----> ");
		}
	}
	printf("\n");

	for (int i = 0; i < ROWS; i++){
		free(table[i]);
	}
	free(table);

	return 0;
}
