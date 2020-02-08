#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tuple.h"
#include "generic.h"
#include "table.h"
#include "hash_table.h"
#include "parser.h"

int main(int argc, char** argv){
	char cmd[2048];
	fgets(cmd,2048,stdin);
	while(strcmp(cmd,"\\q\n")){
		token t = tokenize(cmd);
		node n = parse(t);
		printf("%ld\n",sizeof(n));//temporary to get rid of error about unused variable
		
		fgets(cmd,2048,stdin);
	}
	
	/*
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
	
	/*
	char** plabel = (char**)malloc(sizeof(char*)*2);
	plabel[0] = "Student ID";
	plabel[1] = "Address";
	table t4 = table_project(plabel,2,t);
	free(plabel);
	table_print(t4);
	table_free(t4);

	
	char** labels2 = (char**)malloc(sizeof(char*)*2);
	labels2[0] = "ID";
	labels2[1] = "Class";
	enum type* types2 = (enum type*)malloc(sizeof(enum type)*2);
	types2[0] = integer;
	types2[1] = string;
	int* is_index2 = (int*)malloc(sizeof(int)*2);
	is_index2[0] = 1;
	is_index2[1]= 0;
	table t5 = table_new(2,labels2,types2,is_index2);
	table_insert(2, t5, 1151, "Cooking");
	table_insert(2,t5, 1121, "Charcuterie");
	
	char** p1 = (char**)malloc(sizeof(char*));
	p1[0] = "Student ID";
	char** p2 = (char**)malloc(sizeof(char*));
	p2[0] = "ID";
	char*** pairs= (char***)malloc(sizeof(char**)*2);
	pairs[0] = p1;
	pairs[1] = p2;
	table t6 = table_join(pairs, 1,t,t5);
	table_print(t);
	printf("\n");
	table_print(t5);
	printf("\n");
	table_print(t6);
	printf("\n");
	table_out(t6, "out.txt");
	table t7 = table_in("out.txt");
	table_print(t7);
	table_free(t7);
	table_free(t5);
	table_free(t6);
	table_free(t);
	free(p1);
	free(p2);
	free(pairs);
	free(labels);
	free(types);
	free(is_index);
	free(labels2);
	free(types2);
	free(is_index2);
	

	generic g = generic_new();
	generic_free(g);
	*/
}