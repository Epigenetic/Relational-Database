#ifndef _tuple_h
#define _tuple_h

#include "generic.h"

typedef struct tuple* tuple;

struct tuple{
	int size;
	generic data[];
};

/*
* Returns a new tuple of the given size
*/
tuple tuple_new(int size);

/*
* Frees the given tuple
*/
void tuple_free(tuple t);

/*
* Converts given tuple into string based on given scheme
* The string is malloc'd, so make sure you free it!
*/
char* tuple_string(tuple t, enum type scheme[], int len);

/*
* Prints a given tuple based on given scheme
*/
void tuple_print(tuple t, enum type scheme[], int len);

#endif