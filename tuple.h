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

#endif