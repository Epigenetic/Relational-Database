#include <stdio.h>

#include "tuple.h"
#include "generic.h"
#include "table.h"
#include "hash_table.h"

int main(){
	printf("start\n");
	char *labels[] = {"Student ID","Name", "Address", "Phone Number"};
	enum type types[] = {integer,string,string,integer};
	int is_index[] = {1,1,0,0};
	table t = table_new(4,labels,types,is_index);
	printf("To here\n");
	
	table_insert(4,t,123456,"George Ferguson","500 Joseph C. Wilson Blvd.",654321);
	int id[] = {123456};
	tuple* tu = table_get("Student ID",id,t);
	printf("end");
	printf("ID: %d, Name %s, Address: %s, Phone: %d\n",tu[0]->data[0]->i,tu[0]->data[1]->s,tu[0]->data[2]->s,tu[0]->data[3]->i);
	
	table_free(t);
	
	/*
	generic g = generic_new();
	generic_free(g);
	*/
}