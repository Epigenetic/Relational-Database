#include <stdlib.h>

#include "tuple.h"
#include "generic.h"

/*
* Returns a new tuple of the given size
*/
tuple tuple_new(int size){
	tuple t = (tuple)malloc(sizeof(struct tuple) + size * sizeof(generic));
	t->size = size;
	return t;
}

/*
* Frees the given tuple
*/
void tuple_free(tuple t){
	for(int i = 0; i < t->size; i++){
		generic_free(t->data[i]);
	}
	free(t);
}