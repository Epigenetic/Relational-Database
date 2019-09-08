#include <stdio.h>
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
	if(g == NULL)
		return;
	free(g);
}

void generic_free_string(generic g){
	if(g == NULL)
		return;
	free(g->s);
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
 * Converts a string to an enum type
 */
enum type string_to_type(const char* s){
	if(!strcmp(s,"Character")){
		return character;
	}else if(!strcmp(s,"Integer")){
		return integer;
	}else if(!strcmp(s,"Floating")){
		return floating;
	}else if(!strcmp(s, "String")){
		return string;
	}else{
		printf("Illegal type %s", s);
		return -1;
	}
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
		;
		char* s = g1->s;
		char* s_ = malloc(sizeof(char)*strlen(s)+1);
		strcpy(s_,s);
		g2->s = s_;
		return;
	}
}