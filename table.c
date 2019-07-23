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
	hash_table* data = malloc(sizeof(hash_table)*sum);
	int fill = 0;
	for(int i = 0; i < scheme_size; i++){
		if(is_index[i] != 0){
			hash_table ht = hash_table_new(10,i,scheme_type[i]);
			data[fill++] = ht;
		}
		//printf("id: %d\n",data[fill-1]->element);
	}
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
	free(t);
}

/*
* Inserts a new tuple into the table, number is number of items in the tuple
* Nothing will happeen if incorrect number of arguments given
*/
void table_insert(int num, table t, ...){
	if(num != t->scheme_size)
		return;
	
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
	for(int j = 0; j < t->data_size; j++){
		//printf("j: %d\n",j);
		hash_table_insert(tu, t->data[j]);
	}
	va_end(args);
	//printf("to here\n");
}

/*
* Gets the appropriate tuple from using the appropriate table
*/
tuple* table_select(char* label, void* key, table t){
	int i = 0;
	while(i <= t->scheme_size){
		if(i == t->scheme_size || !(strcmp(label,t->scheme_labels[i]))){
			break;
		}
		i++;
	}
	if(i == t->scheme_size)
		return NULL;
	for(int j = 0; j < t->data_size; j++){
		if(t->data[j]->element == i){
			tuple* rl = malloc(sizeof(tuple));//Because we are searching a unique, we will only get one tuple back
			rl[0] = hash_table_get(t->scheme_type[i],key,t->data[j]);
			return rl;
		}
	}
	return hash_table_brute(t->scheme_type[i],key,i,t->data[0]);
}