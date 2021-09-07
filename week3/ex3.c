#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int read_ints(int *out, short amount){
	const size_t MAX_STR = 12 * amount;
	const int MAX_INT_LEN = 10;

	char *input = (char*)malloc(MAX_STR * sizeof(char));
	char buffer[MAX_INT_LEN + 1];
	int input_len = (int)getline(&input, (size_t*)&MAX_STR, stdin);

	int i = 0;
	int j = 0;
	int k = 0;
	int has_minus = 0;
	long long res = 0;

	if (amount <= 0){
		printf("Quantity of numbers must be positive!\n");
		free(input);
		return 0;
	}

	while (i < input_len){
		if ((input[i] >= '0' && input[i] <= '9' && !((j >= 1 && !has_minus && buffer[0] == '0') || (j >= 2 && has_minus && buffer[1] == '0'))) || (input[i] == '-' && j == 0)){
			if (input[i] == '-')
				has_minus = 1;

			buffer[j] = input[i];
			j++;
		}
		else if ((input[i] == ' ' || input[i] == '\n') && ((j > 0 && !has_minus) || (j > 1 && has_minus))){
			if (k >= amount){
				printf("To many numbers are entered!\n");
				free(input);
				return 0;
			}

			buffer[j] = '\0';
			res = strtoll(buffer, NULL, 10);
			out[k] = (int)res;
			j = 0;
			has_minus = 0;
			k++;
		}
		else {
			printf("Unexpected symbol '%c' in pos %d!\n", input[i], i);
			free(input);
			return 0;
		}

		if ((j > MAX_INT_LEN && !has_minus) || (j > MAX_INT_LEN + 1 && has_minus) || res > INT_MAX || res < INT_MIN){
			printf("To big or too small number for int32!\n");
			free(input);
			return 0;
		}

		i++;
	}

	if (k < amount){
		printf("Too few numbers are entered!\n");
		free(input);
		return 0;
	}

	free(input);
	return 1;
}


struct node{
	int value;
	struct node* next;
};


typedef struct node Node;


Node* createNode(int value){
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->value = value;
	return new_node;
}


void clearNode(Node* node){
	Node* cur_node = node;

	while(cur_node != NULL){
		Node* next = cur_node->next;
		free(cur_node);
		cur_node = next;
	}
}


struct list{
	Node* head;
	void (*printList)(struct list* self);
	Node* (*insertNode)(struct list* self, Node* new_node, Node* after);
	Node* (*deleteNode)(struct list* self, int value);
};


typedef struct list List;


void _printList(struct list* self){
	Node* cur_node = self->head;

	while(cur_node != NULL){
		printf("%d ", cur_node->value);
		cur_node = cur_node->next;
	}
	printf("\n");
}


Node* _insertNode(struct list* self, Node* new_node, Node* after){
	Node* cur_node = self->head;

	if (after == NULL){
		if (self->head != NULL){
			new_node->next = self->head;
		}
		self->head = new_node;
		return new_node;
	}

        while(cur_node != after){
                cur_node = cur_node->next;
        }

	if (cur_node != NULL){
		new_node->next = cur_node->next;
		cur_node->next = new_node;
		return new_node;
	}

	return NULL;
}


Node* _deleteNode(struct list* self, int value){
        Node* cur_node = self->head;
	Node* prev = NULL;

        while(cur_node != NULL){
                if (cur_node->value == value){
			if (prev != NULL){
				prev->next = cur_node->next;
			}

			if (cur_node == self->head){
				self->head = cur_node->next;
			}

			return cur_node;
		}
		prev = cur_node;
                cur_node = cur_node->next;
        }

        return NULL;
}

List* createList(){
	List* new_list = (List*)malloc(sizeof(List));
	new_list->head = NULL;
	new_list->printList = _printList;
	new_list->insertNode = _insertNode;
	new_list->deleteNode = _deleteNode;
	return new_list;
}


void deleteList(List* list){
	clearNode(list->head);
	list->head = NULL;
	free(list);
}


int main(){
	List* list = createList();
	Node* tail = NULL;
	int stoped = 0;
	int arr[1];

	while(!stoped){
		printf("Enter:\n	1 - add node to the head\n	2 - add node to the end\n	3 - delete node\n	4 - print list\n	5 - exit\n");
		int result_of_parsing = read_ints((int*)&arr, 1);

		if (result_of_parsing){
			int option = arr[0];
			switch(option){
				case 1:
					printf("Enter a node value:\n");
					result_of_parsing = read_ints((int*)&arr, 1);

					if (result_of_parsing){
						Node* new_node = createNode(arr[0]);
						if (!list->head){
							tail = list->insertNode(list, new_node, NULL);
						} else {
							list->insertNode(list, new_node, NULL);
						}
						printf("Inserted!\n");
					}
					break;
				case 2:
					printf("Enter a node value:\n");
                                        result_of_parsing = read_ints((int*)&arr, 1);

                                        if (result_of_parsing){
                                                Node* new_node = createNode(arr[0]);
                                                tail = list->insertNode(list, new_node, tail);
						printf("Inserted!\n");
                                        }
                                        break;
				case 3:
					printf("Enter a node value:\n");
                                        result_of_parsing = read_ints((int*)&arr, 1);

					if (result_of_parsing){
						Node* deleted = list->deleteNode(list, arr[0]);
						if (deleted){
							printf("Deleted!\n");
						} else {
							printf("No such value!\n");
						}
					}
                                        break;
				case 4:
					printf("List:\n");
					list->printList(list);
                                        break;
				case 5:
					printf("Buy!\n");
					stoped = 1;
					break;
				default:
					printf("The command was not recognized!\n");
					break;
			}
		}
	}

	deleteList(list);
	return 0;
}
