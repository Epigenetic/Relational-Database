#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "table.h"
#include "generic.h"

/*
* Creates and returns a new table with the appropriate tuple properties
* Creates a hash_table to index the entries for each nonzero position (there must be at least one index)
* The position in the is_index array corresponds to the label in the scheme at the same position
*/
table table_new(int scheme_size, char* scheme_labels[], enum type scheme_type[], int is_index[]){
	table t = (table)malloc(sizeof(struct table));
	t->scheme_size = scheme_size;
	t->scheme_labels = scheme_labels;
	t->scheme_type = scheme_type;
	int sum = 0;
	for(int i = 0; i < scheme_size; i++)
		if(is_index[i] != 0)
			sum++;
	t->data_size = sum;
	hash_table* data = (hash_table*)malloc(sizeof(hash_table)*sum);
	char** index_labels = (char**)malloc(sizeof(char*)*sum);
	int fill = 0;
	for(int i = 0; i < scheme_size; i++){
		if(is_index[i] != 0){
			index_labels[fill] = scheme_labels[i];
			hash_table ht = hash_table_new(10,i,scheme_type[i]);
			data[fill++] = ht;
		}
		//printf("id: %d\n",data[fill-1]->element);
	}
	t->index_labels = index_labels;
	t->data = data;
	//printf("attempt %d\n",t->data[0]->element);
	return t;
}

/*
* Frees the given table
*/
void table_free(table t){
	for(int i = 0; i < t->data_size; i++)
		hash_table_free(t->data[i]);
	free(t->data);
	free(t->index_labels);
	free(t);
}

/*
* Inserts a given tuple into the table (assumes tuple is of correct scheme)
*/
void table_insert_tuple(tuple tu, table ta){
	for(int j = 0; j < ta->data_size; j++){
		//printf("j: %d\n",j);
		hash_table_insert(tu, ta->data[j]);
	}
}

/*
* Inserts a new tuple into the table, number is number of items in the tuple
* Nothing will happeen if incorrect number of arguments given
*/
void table_insert(int num, table t, ...){
	if(num != t->scheme_size){
		printf("Incorrect number of arguments given for table_insert was expecting %d but found %d",t->scheme_size,num);
		return;
	}
	
	tuple tu = tuple_new(num);
	va_list args;
	va_start(args,t);
	int i = 0;
	while(i < num){
		//printf("i: %d\n",i);
		generic g = generic_new();
		switch(t->scheme_type[i]){
			case character:
			g->c = (char)va_arg(args,int);
			break;
			
			case integer:
			g->i = va_arg(args,int);
			break;
			
			case floating:
			g->f = (float)va_arg(args,double);
			break;
			
			case string:
			g->s = va_arg(args,char*);
			break;
		}
		tu->data[i] = g;
		i++;
	}
	table_insert_tuple(tu,t);
	va_end(args);
	//printf("to here\n");
}

/*
* Gets the appropriate tuple from using the appropriate table
*/
table table_select(char* label, void* key, table t){
	int i = 0;
	table rt = table_copy_scheme(t);
	while(i <= t->scheme_size){
		if(i == t->scheme_size || !(strcmp(label,t->scheme_labels[i]))){
			break;
		}
		i++;
	}
	if(i == t->scheme_size) //Couldn't find the right label to select on
		return NULL;
	for(int j = 0; j < t->data_size; j++){
		if(t->data[j]->element == i){
			//tuple* rl = malloc(sizeof(tuple));//Because we are searching a unique, we will only get one tuple back
			//rl[0] = hash_table_get(t->scheme_type[i],key,t->data[j]);
			table_insert_tuple(hash_table_get(t->scheme_type[i],key,t->data[j]),rt);
			return rt;
		}
	}
	tuple* tl = hash_table_brute(t->scheme_type[i],key,i,t->data[0]);
	i = 0;
	while(tl[i] != NULL){
		table_insert_tuple(tl[i],rt);
		i++;
	}
	return rt;
}

/*
* Copies a table's scheme into a new one
*/
table table_copy_scheme(table t){
	int* is_index = (int*)malloc(sizeof(int)*t->scheme_size);
	int found = 0;
	for(int i = 0; i < t->scheme_size; i++){
		if(found == t->data_size)
			break;
		if(!strcmp(t->scheme_labels[i],t->index_labels[found])){
			found++;
			is_index[i] = 1;
		}else{
			is_index[i] = 0;
		}
	}
	table rt = table_new(t->scheme_size, t->scheme_labels, t->scheme_type, is_index);
	free(is_index);
	return rt;
}

/*
* Prints a given table (prints the scheme, the index labels, and all the tuples)
*/
void table_print(table t){
	printf("scheme labels: (");
	for(int i = 0; i < t->scheme_size; i++){
		if(i + 1 == t->scheme_size)
			printf("%s", t->scheme_labels[i]);
		else
			printf("%s, ", t->scheme_labels[i]);
	}
	
	printf(")\nscheme types: (");
	for(int i = 0; i < t->scheme_size; i++){
		switch(t->scheme_type[i]){
			case character:
			if(i + 1 == t->scheme_size)
				printf("character");
			else
				printf("character, ");
			break;
			
			case integer:
			if(i + 1 == t->scheme_size)
				printf("integer");
			else
				printf("integer, ");
			break;
			
			case floating:
			if(i + 1 == t->scheme_size)
				printf("floating");
			else
				printf("floating, ");
			break;
			
			case string:
			if(i + 1 == t->scheme_size)
				printf("string");
			else
				printf("string, ");
			break;
		}
	}
	
	printf(")\nindex labels: (");
	for(int i = 0; i < t->data_size; i++){
		if(i+1 == t->data_size)
			printf("%s", t->index_labels[i]);
		else
			printf("%s, ", t->index_labels[i]);
	}
	printf(")\n");
	int found = 0;
	for(int i = 0; i < t->data[0]->size; i++){
		if(t->data[0]->data[i] != NULL)
			tuple_print(t->data[0]->data[i], t->scheme_type, t->scheme_size);
		
		if(found == t->data[0]->fill) //If we have found all the tuples before going through every item, stop
			break;
	}
}