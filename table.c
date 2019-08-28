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
	t->scheme_labels = (char**)malloc(sizeof(char*)*scheme_size);
	t->scheme_type = (enum type*)malloc(sizeof(enum type)*scheme_size);
	int sum = 0;
	for(int i = 0; i < scheme_size; i++){
		t->scheme_labels[i] = scheme_labels[i];
		t->scheme_type[i] = scheme_type[i];
		if(is_index[i] != 0)
			sum++;
	}
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
	hash_table_free_complete(t->data[0]);
	for(int i = 1; i < t->data_size; i++)
		hash_table_free(t->data[i]);
	free(t->data);
	free(t->index_labels);
	free(t->scheme_labels);
	free(t->scheme_type);
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
* Projects a table on the given column(s)
* items indexed in one table will be in the other
* If none of the columns are indexed, the first will be
*/
table table_project(char* label[], int num, table t){
	int i = 0, found = 0;
	int* is_index = (int*)malloc(sizeof(int)*t->scheme_size);
	enum type* types = (enum type*)malloc(sizeof(enum type)*num);
	int* locations = (int*)malloc(sizeof(int)*num);
	while(i < t->scheme_size && found < num){
		if(!strcmp(t->scheme_labels[i],label[found])){
			types[found] = t->scheme_type[i];
			locations[found] = i;
			for(int j = 0; j < t->data_size; j++){
				if(!strcmp(t->index_labels[j],label[found])){
					is_index[found] = 1;
				}else{
					is_index[found] = 0;
				}
			}
			found++;
		}
		i++;
	}
	int sum = 0;
	for(int i = 0; i < num; i++) //See how many indices there are, and if there are none, make the first one an index
		sum += is_index[i];
	if(sum == 0)
		is_index[0] = 1;
	table t2 = table_new(num, label, types, is_index);
	for(int i = 0; i < t->data[0]->size; i++){
		if(t->data[0]->data[i] == NULL)
			continue;
		tuple tu = tuple_new(num);
		for(int j = 0; j < num; j++){
			generic g = generic_new();
			switch(types[j]){
				case character:
				g->c = t->data[0]->data[i]->data[locations[j]]->c;
				break;
				
				case integer:
				g->i = t->data[0]->data[i]->data[locations[j]]->i;
				break;
				
				case floating:
				g->f = t->data[0]->data[i]->data[locations[j]]->f;
				break;
				
				case string:
				g->s = t->data[0]->data[i]->data[locations[j]]->s;
				break;
			}
			tu->data[j] = g;
		}
		table_insert_tuple(tu, t2);
	}
	free(is_index);
	free(types);
	free(locations);
	return t2;
}

/*
 * Helper function to test if a given number is in an array
*/
int in_array(int* array, int len, int target){
	for(int i = 0; i<len; i++)
		if(array[i] == target)
			return 1;
	return 0;
}

/*
* Joins two tables on the given item(s)
* Column(s) to join on are given in the label_pairs such that label_pairs[0][0] goes with label_pairs[1][0], [0][1] with [1][1], etc.
* Label in the new table will be the name of the same item in the first table unless it is not present there (in which case it is the label of the second table)
* Tuples will have the following scheme: (JoinedItems, t1Items-JoinedItems,t2Items-JoinedItems)
*/
table table_join(char*** label_pairs, int num, table t1, table t2){
	int i1[t1->scheme_size];
	int i2[t2->scheme_size];
	
	//Find where each label is in the schemes
	for(int i = 0; i < num; i++){
		for(int j = 0; j < t1->scheme_size; j++){
			if(!strcmp(label_pairs[0][i],t1->scheme_labels[j])){
				i1[i] = j;
				break;
			}
		}
	}
	int fill = num;
	//Fill in the non joined cases
	for(int i = 0; i < t1->scheme_size; i++){
		if(in_array(i1,t1->scheme_size,i))
			continue;
		i1[fill++] = i;
	}
	for(int i = 0; i < num; i++){
		for(int j = 0; j < t2->scheme_size; j++){
			if(!strcmp(label_pairs[1][i],t2->scheme_labels[j])){
				i2[i] = j;
				break;
			}
		}
	}
	fill = num;
	for(int i = 0; i < t2->scheme_size; i++){
		if(in_array(i2,t2->scheme_size,i))
			continue;
		i2[fill++] = i;
	}
	
	int len = t1->scheme_size + t2->scheme_size - num;
	char** labels = (char**)malloc(sizeof(char*)*len);
	enum type* types = (enum type*)malloc(sizeof(enum type)*len);
	
	for(int i = 0; i < num; i++){
		if(t1->scheme_type[i1[i]] != t2->scheme_type[i2[i]]){
			printf("Label %s and %s are not of incompatible types %s and %s",label_pairs[0][i],label_pairs[0][i],
				type_to_string(t1->scheme_type[i1[i]]), type_to_string(t2->scheme_type[i2[i]]));
			return NULL;
		}
		types[i] = t1->scheme_type[i1[i]];
		labels[i] = t1->scheme_labels[i1[i]]; //fill in the shared labels while we are checking for type compatibility
	}
	
	int found = 0;
	fill = num;
	for(int i = 0; i < t1->scheme_size; i++){
		if(found < num && i1[found] == i){//NEED TO DO AWAY WITH THIS, CHECK FOR ALREADY FILLED VALUES SOME OTHER WAY, DECLARE FOUND EARLIER?
			found++;
			continue;
		}
		types[fill] = t1->scheme_type[i];
		labels[fill] = t1->scheme_labels[i];
		fill++;
	}
	
	found = 0;
	for(int i = 0; i < t2->scheme_size; i++){
		if(found < num && i2[found] == i){
			found++;
			continue;
		}
		types[fill] = t2->scheme_type[i];
		labels[fill] = t2->scheme_labels[i];
		fill++;
	}
	
	int* is_index = (int*)malloc(sizeof(int)*len);
	
	// Identify the indices in the new table
	for(int i = 0; i < len; i++){
		for(int j = 0; j < t1->data_size; j++){
			if(!strcmp(labels[i],t1->index_labels[j]))
				is_index[j] |= 1;
			else
				is_index[j] |= 0;
		}
		for(int j = 0; j < t2->data_size; j++){
			if(!strcmp(labels[i],t2->index_labels[j]))
				is_index[j] |= 1;
			else
				is_index[j] |= 0;
		}
	}
	
	//Check for at least one index, if there is none, the first item becomes one
	int sum;
	for(int i = 0; i < len; i++)
		sum += is_index[i];
	if(sum == 0)
		is_index[0] = 1;
	
	table rt = table_new(len, labels, types, is_index);
	
	for(int i = 0; i < t1->data[0]->size; i++){
		for(int j = 0; j < t2->data[0]->size; j++){
			if(t1->data[0]->data[i] == NULL || t2->data[0]->data[j] == NULL)
				continue;
			int break_flag = 0;
			for(int k = 0; k < num; k++){
				//If any of the paired itmes are not equal, do not merge them and add them
				break_flag = !generic_equal(t1->data[0]->data[i]->data[i1[k]],t2->data[0]->data[j]->data[i2[k]],types[k]);
				if(break_flag) //If one item is not equal, stop checking if the rest are
					break;
			}
			if(break_flag) //If they are not equal, go to next tuple
				continue;
			
			tuple tu = tuple_new(len);
			for(int k = 0; k < len; k++){
				generic to_copy;
				if(k < t1->scheme_size){
					to_copy = t1->data[0]->data[i]->data[i1[k]];
				}else{//Would be l < len, but this will always be true
					to_copy = t2->data[0]->data[j]->data[i2[k-(t1->scheme_size-1)]];
				}
				generic new = generic_new();
				generic_copy(to_copy, new, types[k]);
				tu->data[k] = new;
			}
			
			table_insert_tuple(tu,rt);
		}
	}
	free(labels);
	free(types);
	free(is_index);
	return rt;
}

/*
* Prints a given table (prints the scheme, the index labels, and all the tuples)
*/
void table_print(table t){
	printf("Scheme Labels: (");
	for(int i = 0; i < t->scheme_size; i++){
		if(i + 1 == t->scheme_size)
			printf("%s", t->scheme_labels[i]);
		else
			printf("%s, ", t->scheme_labels[i]);
	}
	
	printf(")\nScheme Types: (");
	for(int i = 0; i < t->scheme_size; i++){
		if(i+1 == t->scheme_size)
			printf("%s",type_to_string(t->scheme_type[i]));
		else
			printf("%s, ",type_to_string(t->scheme_type[i]));
	}
	
	printf(")\nIndex Labels: (");
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

/*
* Stores a table into a file
* Format is:
* First line has scheme size
* Second line has list of scheme labels
* Third line has list of scheme types
* Fourth line has ist of is_index values
* All remaining lines are tuples in the table
*/
void table_out(table t, char* file_name){
	FILE* out = fopen(file_name,"w");
	
	fprintf(out, "%d\n", t->scheme_size);
	
	for(int i = 0; i < t->scheme_size; i++){
		if(i + 1 == t->scheme_size)
			fprintf(out, "%s", t->scheme_labels[i]);
		else
			fprintf(out, "%s, ", t->scheme_labels[i]);
	}
	
	fputs("\n", out);
	
	for(int i = 0; i < t->scheme_size; i++){
		if(i+1 == t->scheme_size)
			fprintf(out, "%s",type_to_string(t->scheme_type[i]));
		else
			fprintf(out, "%s, ",type_to_string(t->scheme_type[i]));
	}
	
	fputs("\n", out);
	
	for(int i = 0; i < t->scheme_size; i++){
		int is_index = 0;
		for(int j = 0; j < t->data_size; j++){
			if(!strcmp(t->scheme_labels[i],t->index_labels[j])){
				is_index = 1;
				break;
			}
		}
		if(i+1 == t->scheme_size)
			fprintf(out, "%d", is_index);
		else
			fprintf(out, "%d, ", is_index);
	}
	
	fputs("\n", out);
	
	for(int i = 0; i < t->data[0]->size; i++)
		if(t->data[0]->data[i] != NULL){
			char* tuple  = tuple_string(t->data[0]->data[i], t->scheme_type, t->scheme_size);
			fprintf(out, "%s\n", tuple);
			free(tuple);
		}
	
	fclose(out);
}