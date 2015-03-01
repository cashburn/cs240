#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LinkedList.h"

//
// Initialize a linked list
//
void llist_init(LinkedList * list)
{
	list->head = NULL;
}

//
// It prints the elements in the list in the form:
// 4, 6, 2, 3, 8,7
//
void llist_print(LinkedList * list) {
	
	ListNode * e;

	if (list->head == NULL) {
		printf("{EMPTY}\n");
		return;
	}

	printf("{");

	e = list->head;
	while (e != NULL) {
		printf("%d", e->value);
		e = e->next;
		if (e!=NULL) {
			printf(", ");
		}
	}
	printf("}\n");
}

//
// Appends a new node with this value at the beginning of the list
//
void llist_add(LinkedList * list, int value) {
	// Create new node
	ListNode * n = (ListNode *) malloc(sizeof(ListNode));
	n->value = value;
	
	// Add at the beginning of the list
	n->next = list->head;
	list->head = n;
}

//
// Returns true if the value exists in the list.
//
int llist_exists(LinkedList * list, int value) {
	ListNode * n = list->head;
	while (n != NULL) {
		if (n->value == value)
			return 1;
		else
			n = n->next;
	}
	return 0;
}

//
// It removes the entry with that value in the list.
//
int llist_remove(LinkedList * list, int value) {
	ListNode * n = list->head;
	ListNode * next = n->next;

	while (next != NULL) {
		if (next->value == value) {
			n->next = next->next;
			free(next);
			return 1;
		}
		else {
			n = n->next;
			next = next->next;
		}
	}
	return 0;
}

//
// It stores in *value the value that correspond to the ith entry.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_get_ith(LinkedList * list, int ith, int * value) {
	ListNode * n = list->head;
	for (int i = 0; i < ith; i++) {
		if (n == NULL)
			return 0;
		n = n->next;
	}
	*value = n->value;
	return 1;
}
int llist_get_last(LinkedList * list) {
	ListNode * n = list->head;
	if (n == NULL) {
		return 0;
	}
	while (n->next != NULL)
		n = n->next;
	return n->value;
}
//
// It removes the ith entry from the list.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_remove_ith(LinkedList * list, int ith) {
	ListNode * prev = list->head;
	ListNode * n = prev->next;
	for (int i = 0; i < ith - 1; i++) {
		if (n == NULL)
			return 0;
		n = n->next;
		prev = prev->next;
	}
	prev->next = n->next;
	free(n);
	return 1;
}

//
// It returns the number of elements in the list.
//
int llist_number_elements(LinkedList * list) {
	int i = 0;
	ListNode * n = list->head;
	while (n != NULL) {
		n = n->next;
		i++;
	}
	return i;
}


//
// It saves the list in a file called file_name. The format of the
// file is as follows:
//
// value1\n
// value2\n
// ...
//
int llist_save(LinkedList * list, char * file_name) {
	FILE * fd = fopen(file_name, "w");
	if (fd == NULL) {
		//printf("Could not create file %s\n", file_name);
		return 0;
	}
	ListNode * n = list->head;

	if (n == NULL) {
		fprintf(fd,"{EMPTY}\n");
		return 0;
	}

	while (n != NULL) {
		fprintf(fd,"%d", n->value);
		n = n->next;
		fprintf(fd,"\n");
	}
	fclose(fd);
	return 0;
}

//
// It reads the list from the file_name indicated. If the list already has entries, 
// it will clear the entries.
//
int llist_read(LinkedList * list, char * file_name) {
	FILE * fd = fopen(file_name, "r");
	if (fd == NULL) {
		//printf("Could not open file, %s\n", file_name);
		return 0;
	}
	list->head = NULL;
	int read;
	while (fscanf(fd, "%d", &read) > 0)
		llist_add(list, read);
	fclose(fd);
	return 1;
}


//
// It sorts the list. The parameter ascending determines if the
// order si ascending (1) or descending(0).
//
void llist_sort(LinkedList * list, int ascending) {
	ListNode * n = list->head;
	ListNode * next = n->next;
	ListNode * n1 = list->head;
	int temp;
	int again = 1;
	if (ascending) {
		while (again) {
			again = 0;
			while (next != NULL) {
				if (n->value > next->value) {
					temp = n->value;
					n->value = next->value;
					next->value = temp;
					again = 1;
				}
				n = n->next;
				next = next->next;
			}
			n = list->head;
			next = n->next;
		}
	}
	else {
		while (again) {
			again = 0;
			while (next != NULL) {
				if (n->value < next->value) {
					temp = n->value;
					n->value = next->value;
					next->value = temp;
					again = 1;
				}
				n = n->next;
				next = next->next;
			}
			n = list->head;
			next = n->next;
		}
	}
	
	return 0;
}

//
// It removes the first entry in the list and puts value in *value.
// It also frees memory allocated for the node
//
int llist_remove_first(LinkedList * list, int * value) {
	ListNode * n = list->head;
	if (n == NULL)
		return 0;
	*value = n->value;
	list->head = n->next;
	free(n);
	return 1;
}

//
// It removes the last entry in the list and puts value in *value/
// It also frees memory allocated for node.
//
int llist_remove_last(LinkedList * list, int *value) {
	ListNode * prev = list->head;
	if (prev == NULL)
		return 0;
	ListNode * n = prev->next;
	while (n->next != NULL) {
		prev = prev->next;	
		n = n->next;
	}
	*value = n->value;
	prev->next = NULL;
	free(n);
	return 1;
}

//
// Insert a value at the beginning of the list.
// There is no check if the value exists. The entry is added
// at the beginning of the list.
//
void llist_insert_first(LinkedList * list, int value) {
	ListNode * n = (ListNode *) malloc(sizeof(ListNode));
	n->value = value;
	
	n->next = list->head;
	list->head = n;
}

//
// Insert a value at the end of the list.
// There is no check if the name already exists. The entry is added
// at the end of the list.
//
void llist_insert_last(LinkedList * list, int value) {
	ListNode * n = (ListNode *) malloc(sizeof(ListNode));
	n->value = value;
	n->next = NULL;

	ListNode * i = list->head;
	if (i == NULL) {
		list->head = n;
		return;
	}
	while (i->next != NULL)
		i = i->next;
	i->next = n;
}
//
// Clear all elements in the list and free the nodes
//
void llist_clear(LinkedList *list) {
	ListNode * n = list->head;
	if (n == NULL)
		return 0;
	ListNode * next = n->next;
	while (next != NULL) {
		n = next;	
		next = next->next;
		free(n);
	}
	list->head = NULL;
	//free(n);
	return 1;
}
