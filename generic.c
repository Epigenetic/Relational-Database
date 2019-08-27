#include <stdlib.h>
#include <string.h>

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

/*
* Converts a enum type to a string
*/
char* type_to_string(enum type t){
	switch(t){	
		case character:
		return "Character";
		
		case integer:
		return "Integer";
		
		case floating:
		return "Floating";
		
		case string:
		return "String";
	}
	return "Not a type";
}

/*
 * Tests equality of two generics holding the same type
 */
int generic_equal(generic g1, generic g2, enum type t){
	switch(t){
		case character:
		return g1->c == g2->c;
		
		case integer:
		return g1->i == g2->i;
		
		case floating:
		return g1->f == g2->f;
		
		case string:
		return !strcmp(g1->s,g2->s);
	}
	return 5; //Hopefully, normal operation will never reach here and this value will not be produced, making debugging easy
}

/*
 * Copies the value of one generic for a given type into another
 */
void generic_copy(generic g1, generic g2, enum type t){
	switch(t){
		case character:
		g2->c = g1->c;
		return;
		
		case integer:
		g2->i = g1->i;
		return;
		
		case floating:
		g2->f = g1->f;
		return;
		
		case string:
		g2->s = g1->s;
		return;
	}
}