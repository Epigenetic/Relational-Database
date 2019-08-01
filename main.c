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
	/*
	int id[] = {123456};
	table t2 = table_select("Student ID",id,t);
	//printf("end");
	table_print(t2);
	table_free(t2);
	
	char* address = "4 Main St.";
	table t3 = table_select("Address",address,t);
	table_print(t3);
	
	table_free(t3);
	free(labels);
	free(types);
	free(is_index);
	*/
	
	char** plabel = (char**)malloc(sizeof(char*)*2);
	plabel[0] = "Student ID";
	plabel[1] = "Address";
	table t4 = table_project(plabel,2,t);
	free(plabel);
	table_print(t4);
	table_free(t4);
	table_free(t);
	
	/*
	generic g = generic_new();
	generic_free(g);
	*/
}