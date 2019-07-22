#ifndef _generic_h
#define _generic_h

typedef union generic* generic;

enum type{character,integer,floating,string};

union generic{
	char c;
	int i;
	float f;
	char* s;
};

/*
* Returns a new generic
*/
generic generic_new();

/*
* Frees the given generic
*/
void generic_free(generic g);

#endif