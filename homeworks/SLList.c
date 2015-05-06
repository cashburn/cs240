#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SLList.h"


SLList * sllist_create() {
	SLList * list = (SLList *) malloc(sizeof(SLList));
	list->head = NULL;
	return list;
}

int sllist_insert(SLList * sllist, char * name, char * value) {
	SLListEntry * s = (SLListEntry *) malloc(sizeof(SLListEntry));
	SLListEntry * iter = sllist->head;
	SLListEntry * last = NULL;
	while (iter != NULL) {
		if (!strcmp(name, iter->name)) {
			free(s);
			free(iter->value);
			iter->value = strdup(value);
			return 1;
		}
		last = iter;
		iter = iter->next;
	}
	s->name = strdup(name);
	s->value = strdup(value);
	s->next = NULL;
	
	if (last == NULL) {
		sllist->head = s;
		return 0;
	}
	last->next = s;
	return 0;
}

int sllist_remove (SLList * sllist, char * name) {
	SLListEntry * s = sllist->head;
	SLListEntry * last = NULL;
	while (s != NULL) {
		if (!strcmp(name, s->name))
			break;
		last = s;
		s = s->next;
	}
	if(s == NULL)
		return 0;
	if (last == NULL) {
		sllist->head = s->next;
		free(s);
		return 1;
	}
	last->next = s->next;
	free(s);
	return 1;
}

int sllist_last(SLList * sllist, char ** pname, char ** pvalue) {
	SLListEntry * s = sllist->head;
	if (s == NULL)
		return 0;
	while (s->next != NULL)
		s = s->next;
	pname = &(s->name);
	pvalue = &(s->value);
	return 1;
}

void sllist_print(SLList * sllist) {
	SLListEntry * s = sllist->head;
	int count = 0;
	while (s != NULL) {
		printf("%d: name=%s value=%s\n", count, s->name, s->value);
		count++;
		s = s->next;
	}
}

void sllist_reverse(SLList * sllist) {
	SLListEntry * t = NULL;
	SLListEntry * s = sllist->head;
	SLListEntry * next;
	while (s != NULL) {
		next = s->next;
		s->next = t;
		t = s;
		s = next;
	}
	sllist->head = t;
}

int main() {
	char * string = (char *) malloc(20 * sizeof(char));
	SLList * list = sllist_create();
	for (int i = 0; i < 10; i++) {
		sprintf(string, "Number %d\n", i);
		sllist_insert(list, string, string);
	}
	sllist_print(list);
	sllist_reverse(list);
	sllist_print(list);
}