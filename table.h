#ifndef _table_h
#define _table_h

#include "hash_table.h"
#include "tuple.h"
#include "generic.h"

typedef struct table* table;

/*
* data_size is number of hash tables
* data is the hash tables
* scheme_size is the size of the scheme
* scheme_type is an array indicating the type of data held in each position of the tuple
* scheme_labels is the corresponding labels to the each item in the tuple
*/
struct table{
	int data_size;
	hash_table* data;
	int scheme_size;
	enum type* scheme_type;
	char** scheme_labels;
};

/*
* Creates and returns a new table with the appropriate tuple properties
* Creates a hash_table to index the entries for each nonzero position (there must be at least one index)
* The position in the is_index array corresponds to the label in the scheme at the same position
*/
table table_new(int scheme_size, char* scheme_labels[], enum type scheme_type[], int is_index[]);

/*
* Frees the given table
*/
void table_free(table t);

/*
* Inserts a new tuple into the table, number is number of items in the tuple
* Nothing will happeen if incorrect number of arguments given
*/
void table_insert(int num, table t, ...);

/*
* Gets the appropriate tuple(s) from using the appropriate table
*/
tuple* table_select(char* label, void* key, table t);

#endif