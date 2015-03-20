
#include <stdio.h>
#include <stdlib.h>

#include "slist.h"

void
sllist_init(SLList * list)
{
	list->head = NULL;
}

// Add int value to the end of list. The values may be repeated.
void
sllist_add_end( SLList *list, int value )  {
	SLEntry * e = list->head;
	SLEntry * n = (SLEntry *) malloc(sizeof(SLEntry));
	n->value = value;
	if (e == NULL) {
		list->head = n;
		return;
	}
	while (e->next != NULL) {
		e = e->next;
	}
	e->next = n;
}


// Remove first occurrence of value in the list. It returns 1 if value is found or 0 otherwise.
int sllist_remove(SLList *list, int value) {
	SLEntry * e = list->head;
	SLEntry * last = NULL;
	while ((e != NULL) && (e->value != value)) {
		last = e;
		e = e->next;
	}
	if (e == NULL)
		return 0;
	if (last == NULL) {
		list->head = e->next;
	}
	else
		last->next = e->next;
	free(e);
	return 1;
}


// Removes from the list and frees the nodes of all the items that are min <= value <= max
void sllist_remove_interval(SLList *list, int min, int max) {
	SLEntry * e = list->head;
	SLEntry * last = NULL;
	while ((e != NULL)) {
	 	if ((e->value >= min) && (e->value <= max)) {
			if (last == NULL) {
				list->head = e->next;
				last = list->head;
			}
			else
				last->next = e->next;
			free(e);
			e = last;
		}
		last = e;
		e = e->next;
	}
}


// It allocates and returns a new list with the intersection of the two lists, that is the 
// list of all the items that are common to the two lists. The items in the intersection are
// unique. 
SLList * sllist_intersection(SLList * a, SLList * b) {
	SLList * intersection = (SLList *) malloc(sizeof(SLList));
	sllist_init(intersection);
	SLEntry * entrya = b->head;
	SLEntry * entryb = a->head;
	SLEntry * entryi = intersection->head;
	SLEntry * last = entryi;
	while (entrya != NULL) {
		entryi = intersection->head;
		while (entryi != NULL) {
			if (entrya->value == entryi->value)
				break;
			last = entryi;
			entryi = entryi->next;

		}
		if (entryi != NULL) {
			entrya = entrya->next;
			entryi = intersection->head;
			continue;
		}
		entryb = b->head;
		while (entryb != NULL) {
			if (entrya->value == entryb->value) {
				entryi = (SLEntry *) malloc(sizeof(SLEntry));
				entryi->value = entrya->value;
				if (last == NULL) {
					intersection->head = entryi;
					last = intersection->head;
				}
				else {
					last->next = entryi;
					last = entryi;
				}
				break;
			}
			entryb = entryb->next;
		}
		entrya = entrya->next;
	}
	return intersection;
}

void sllist_print(SLList * list)
{
	// Move to the end
	SLEntry * e = list->head;

	printf("--- List ---\n");
	while (e!= NULL) {
		printf("val=%d\n", e->value);
		e = e->next;
	}
}

