#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

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

int call_vsprintf(char* buffer, char* format, ...);

/*
* Converts given tuple into string based on given scheme
* The string is malloc'd, so make sure you free it!
*/
char* tuple_string(tuple t, enum type scheme[], int len){
	char* final_string = (char*)malloc(500);
	for(int i = 0; i < len; i++){
		char* to_append = (char*)malloc(100);
		switch(scheme[i]){
			case character:
			if(call_vsprintf(to_append,"%c",t->data[i]->c) < 0)
				return "Error, make sure scheme and length correspond to given tuple";
			break;
			
			case integer:
			if(call_vsprintf(to_append,"%d",t->data[i]->i) < 0)
				return "Error, make sure scheme and length correspond to given tuple";
			break;
			
			case floating:
			if(call_vsprintf(to_append,"%f",t->data[i]->f) < 0)
				return "Error, make sure scheme and length correspond to given tuple";
			break;
			
			case string:
			if(call_vsprintf(to_append,"%s",t->data[i]->s) < 0)
				return "Error, make sure scheme and length correspond to given tuple";
			break;
		}
		if(i == 0){
			strcat(final_string,"(");
			strcat(final_string,to_append);
		}else{
			strcat(final_string,",");
			strcat(final_string,to_append);
		}
		free(to_append);
	}
	strcat(final_string,")");
	return final_string;
}

/*
* Prints a given tuple based on given scheme
*/
void tuple_print(tuple t, enum type scheme[], int len){
	char* to_print = tuple_string(t,scheme,len);
	printf("%s\n", to_print);
	free(to_print);
}

/*
* Helper function for calling vsprintf
*/
int call_vsprintf(char* buffer, char* format, ...){
	va_list vl;
	int ret;
	va_start(vl,format);
	ret = vsprintf(buffer,format,vl);
	va_end(vl);
	
	return ret;
}
