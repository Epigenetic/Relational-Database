#include <stdlib.h>
#include <stdio.h>

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

/*
* Converts given tuple into string based on given scheme
*/
char* tuple_string(tuple t, enum type scheme[], int len){
	char* final_string;
	for(int i = 0; i < len; i++){
		char* to_append;
		switch(scheme[i]){
			case character:
			if(asprintf(&to_appen,"%c",t->data[i]->c) < 0)
				return "Error, make sure scheme and length correspond to given tuple";
			break;
			
			case integer:
			if(asprintf(&to_appen,"%d",t->data[i]->i) < 0)
				return "Error, make sure scheme and length correspond to given tuple";
			break;
			
			case floating:
			if(asprintf(&to_appen,"%f",t->data[i]->f) < 0)
				return "Error, make sure scheme and length correspond to given tuple";
			break;
			
			case string:
			if(asprintf(&to_appen,"%s",t->data[i]->s) < 0)
				return "Error, make sure scheme and length correspond to given tuple";
			break;
		}
		if(i == 0)
			asprintf(&final_string,"(%s",to_append);
		else
			asprintf(&final_string,"%s,%s",final_string,to_append);
	}
	asprintf(&final_string,"%s)",final_string);
	return final_string;
}
