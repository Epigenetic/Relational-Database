#include <stdlib.h>

#include "generic.h"

/*
* Returns a new generic
*/
generic generic_new(){
	return (generic)malloc(sizeof(union generic));
}

/*
* Frees the given generic
*/
void generic_free(generic g){
	free(g);
}