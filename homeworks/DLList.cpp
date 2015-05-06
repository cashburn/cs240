#include <stdio.h>
#include <string.h>
#include "DLList.h"

DLList::DLList() {
	head = new DLListEntry;
	head->name = NULL;
	head->value = NULL;
	head->next = head;
	head->prev = head;
}

int DLList::add (char * name, char * value) {
	DLListEntry * iter = head->next;

	while (iter->name != NULL) {
		if (!strcmp(name, iter->name)) {
			iter->value = strdup(value);
			return 1;
		}
		iter = iter->next;
	}
	DLListEntry * e = new DLListEntry;
	e->name = strdup(name);
	e->value = strdup(value);
	e->next = head;
	e->prev = head->prev;
	e->prev->next = e;
	head->prev = e;
	return 0;
}

int DLList::remove (char * name) {
	DLListEntry * iter = head->next;
	while (iter->name != NULL) {
		if (!strcmp(name, iter->name)) {
			free(iter->name);
			free(iter->value);
			iter->next->prev = iter->prev;
			iter->prev->next = iter->next;
			return 1;
		}
		iter = iter->next;
	}
	return 0;
}

void DLList::concatenate (DLList * list) {
	DLListEntry * temp;
	head->prev->next = list->head->next;
	list->head->prev->next = head;
	temp = list->head->prev;
	list->head->prev = head->prev;
	head->prev = temp;
}

void DLList::print() {
	DLListEntry * iter = head->next;
	while (iter != head) {
		printf("%s: %s\n", iter->name, iter->value);
		iter = iter->next;
	}
}

DLList::~DLList() {
	//DLListEntry * iter = head;
	while (head->next != head) {
		free(head->next->name);
		free(head->next->value);
		head->next = head->next->next;
	}
	delete head;
}

int main() {
	DLList list;
	list.add("Name 1", "Value 1");
	list.add("Name 2", "Value 2");
	list.add("Name 3", "Value 3");

	list.print();

	list.remove("Name 2");

	list.print();

	DLList list2;
	list2.add("2Name 1", "2Value 1");
	list2.add("2Name 2", "2Value 2");
	list2.add("2Name 3", "2Value 3");

	list.concatenate(&list2);

	list.print();

}