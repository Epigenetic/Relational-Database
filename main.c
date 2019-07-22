#include <stdio.h>
#include <stdlib.h>

#include "tuple.h"
#include "generic.h"
#include "table.h"
#include "hash_table.h"

int main(){
	//printf("start\n");
	char **labels = (char**)malloc(sizeof(char*)*4);
	labels[0] = "Student ID";
	labels[1] = "Name";
	labels[2] = "Address";
	labels[3] = "Phone Number";
	enum type* types = (enum type*)malloc(sizeof(enum type)*4);
	types[0] = integer;
	types[1] = string;
	types[2] = string;
	types[3] = integer;
	int* is_index = (int*)malloc(sizeof(int)*4);
	is_index[0] = 1;
	is_index[1] = 1;
	is_index[2] = 0;
	is_index[3] = 0;
	table t = table_new(4,labels,types,is_index);
	//printf("id: %d\n", t->data[0]->element);
	//printf("%s\n",t->scheme_labels[0]);
	
	table_insert(4,t,123456,"George Ferguson","500 Joseph C. Wilson Blvd.",654321);
	table_insert(4,t,1121,"Charlie Brown","4 Main St.",2083);
	table_insert(4,t,1151,"John Smith","4 Main St.",2083);
	int id[] = {123456};
	tuple* tu = table_get("Student ID",id,t);
	//printf("end");
	char* converted_tuple = tuple_string(tu[0],types,4);
	printf("%s\n",converted_tuple);
	free(converted_tuple);
	tuple_free(tu[0]);
	free(tu);
	
	char* address = "4 Main St.";
	tuple* tu2 = table_get("Address",address,t);
	converted_tuple = tuple_string(tu2[0],types,4);
	printf("%s\n",converted_tuple);
	free(converted_tuple);
	
	converted_tuple = tuple_string(tu2[1],types,4);
	printf("%s\n",converted_tuple);
	free(converted_tuple);
	
	tuple_free(tu2[0]);
	tuple_free(tu2[1]);
	free(tu2);
	table_free(t);
	free(labels);
	free(types);
	free(is_index);
	
	/*
	generic g = generic_new();
	generic_free(g);
	*/
}