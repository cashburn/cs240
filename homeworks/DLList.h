#include <stdlib.h>

struct DLListEntry {
	//public:
		char * name;
		char * value;
		DLListEntry * next;
		DLListEntry * prev;
};

class DLList {
	
public:
	DLListEntry * head;

	DLList();

	int add(char * name, char * value);

	int remove(char * name);

	void concatenate(DLList * list);

	void print();

	~DLList();
};